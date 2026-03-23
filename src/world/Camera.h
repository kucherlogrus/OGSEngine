#ifndef OGSENGINE_CAMERA_H
#define OGSENGINE_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace ogs {

enum class CameraType {
    Undef,
    Ortho,
    Project,
};

struct Frustum {
    // 6 planes: left, right, bottom, top, near, far
    // Each plane: normal (xyz) + distance (w), pointing inward
    std::array<glm::vec4, 6> planes;

    bool containsSphere(const glm::vec3& center, float radius) const {
        for (const auto& plane : planes) {
            if (glm::dot(glm::vec3(plane), center) + plane.w < -radius)
                return false;
        }
        return true;
    }

    bool containsPoint(const glm::vec3& point) const {
        return containsSphere(point, 0.0f);
    }
};

class Camera {
public:
    Camera() { recalculate(); }

    // --- Projection setup ---

    void setPerspective(float fovDeg, float aspect, float nearPlane, float farPlane) {
        type         = CameraType::Project;
        fov          = fovDeg;
        this->aspect = aspect;
        this->nearZ  = nearPlane;
        this->farZ   = farPlane;
        recalculate();
    }

    // size = half-height in world units; aspect determines half-width
    void setOrthographic(float size, float aspect, float nearPlane, float farPlane) {
        type         = CameraType::Ortho;
        orthoSize    = size;
        this->aspect = aspect;
        this->nearZ  = nearPlane;
        this->farZ   = farPlane;
        recalculate();
    }

    // --- View setup ---

    void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = {0,1,0}) {
        position = eye;
        view     = glm::lookAt(eye, target, up);
        recalculate();
    }

    void setPosition(const glm::vec3& pos) {
        position = pos;
        view     = glm::lookAt(pos, pos + forward(), {0,1,0});
        recalculate();
    }

    // --- Getters ---

    glm::vec3 forward() const {
        return -glm::vec3(view[0][2], view[1][2], view[2][2]);
    }

    const glm::mat4& getView()       const { return view; }
    const glm::mat4& getProjection() const { return projection; }
    const glm::mat4& getViewProj()   const { return viewProj; }
    const glm::vec3& getPosition()   const { return position; }
    const Frustum&   getFrustum()    const { return frustum; }
    CameraType       getType()       const { return type; }

    float getNear()     const { return nearZ; }
    float getFar()      const { return farZ; }
    float getAspect()   const { return aspect; }
    float getFov()      const { return fov; }
    float getOrthoSize() const { return orthoSize; }

    void setAspect(float a) { aspect = a; recalculate(); }

private:
    CameraType type = CameraType::Undef;

    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 viewProj   = glm::mat4(1.0f);
    glm::vec3 position   = {0.0f, 0.0f, 3.0f};

    // Perspective params
    float fov    = 60.0f;
    // Orthographic params
    float orthoSize = 5.0f; // half-height in world units

    float aspect = 16.0f / 9.0f;
    float nearZ  = 0.1f;
    float farZ   = 1000.0f;

    Frustum frustum;

    void recalculate() {
        switch (type) {
            case CameraType::Project:
                projection = glm::perspective(glm::radians(fov), aspect, nearZ, farZ);
                break;
            case CameraType::Ortho: {
                float hw = orthoSize * aspect; // half-width
                projection = glm::ortho(-hw, hw, -orthoSize, orthoSize, nearZ, farZ);
                break;
            }
            default:
                // Undef — identity, no culling until type is set
                projection = glm::mat4(1.0f);
                break;
        }
        viewProj = projection * view;
        extractFrustum();
    }

    void extractFrustum() {
        const glm::mat4& m = viewProj;
        // Gribb/Hartmann method — extract planes from view-projection matrix
        frustum.planes[0] = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]); // left
        frustum.planes[1] = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]); // right
        frustum.planes[2] = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]); // bottom
        frustum.planes[3] = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]); // top
        frustum.planes[4] = glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]); // near
        frustum.planes[5] = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]); // far

        // Normalize planes
        for (auto& plane : frustum.planes) {
            float len = glm::length(glm::vec3(plane));
            if (len > 0.0f) plane /= len;
        }
    }
};

} // ogs

#endif //OGSENGINE_CAMERA_H