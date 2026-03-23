#ifndef OGS_TEXTURESPRITEBATH_H
#define OGS_TEXTURESPRITEBATH_H


#include <renderer/Buffers.h>
#include "Texture.h"
#include "renderer/Mesh.h"
#include "scene2d/Sprite2d.h"
#include "FreeTypeManager.h"
#include "renderer/Program.h"


namespace ogs {

using namespace glm;

class FreeTypeFont;
class Sprite2d;
class Mesh;
class Scene;

class SpriteBatch {
    friend class FreeTypeText; // TODO - refactor from this

private:
    mat4 mvp;
    Texture *currentTexture;

    void flush();

    void changeShader();

    bool isDrawing;

    const int maxIndicesSize = 5460;

    Program* program;
    VertexIndexBuffer indexBuffer;
    VertexArrayBuffer arrayBuffer;
    vector<float> vericles_data;
    vector<unsigned int> indexes_data;

    size_t count = 0;
    size_t index_count = 0;
    size_t vertex_count = 0;

    void init(size_t);


public:

    bool inDraw() { return isDrawing; };

    void begin();

    void end();

    void setMatrix(mat4 &cameramat4) {
        mvp = cameramat4;
    }


    void matrixSetup();

    SpriteBatch(size_t);

    SpriteBatch();

    ~SpriteBatch();

    void checkTexture(Texture *);


    // drawing api/
    void draw(Sprite2d *sprite);
    //TODO - add size
    void draw(FreeTypeFont* bitmap, std::string text, float x, float y);

    void draw(Texture *texture, float x, float y, float width, float height);

    void draw(Texture *texture, float x, float y, float width, float height, float scaleX,
              float scaleY, float srcX, float srcY, float srcWidth, float srcHeight,
              float angle, float alpha);
};


}

#endif
