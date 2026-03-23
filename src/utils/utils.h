#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include "monitoring/Logsystem.h"
#include <cstddef>
#include <cstdio>
#include "glm/detail/type_mat4x4.hpp"
#include "glm/detail/type_vec3.hpp"
#include <cstdlib>

namespace ogs::utils {

void matrixPrint(glm::mat4 matrix, std::string name);

void vector3Print(glm::vec3 matrix, std::string name);

int getRandomInt(int, int);

unsigned int nextpow2(int x);

template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

template<typename T = float>
std::vector<T> coordGenerate(int frames, T coord, T add) {
    std::vector<T> coordinates;
    coordinates.reserve(frames);
    while (coordinates.size() < frames) {
        coordinates.push_back(coord);
        coord += add;
    }
    return std::move(coordinates);
}

template<typename T>
void showVector(const std::vector<T> &vec, int linecount = 5) {
    size_t count = 0;
    for (auto elem : vec) {
        std::cout << static_cast<T>(elem) << " ";
        count++;
        if (count >= linecount) {
            std::cout << std::endl;
            count = 0;
        }
    }
    std::cout << std::endl;
}

template<typename T>
void showVector(const std::vector<T> &vec, int linecount, int size) {
    int count = 0;
    int entry = 0;
    while (entry < size) {
        std::cout << static_cast<T>(vec[entry]) << " ";
        entry++;
        if (count == linecount) {
            std::cout << std::endl;
            count = 0;
        } else {
            count++;
        }


    }
    std::cout << std::endl;


}

bool isPointIntersecAABB(float x, float y, float w, float h, float x1, float y1);

bool isPointIntersecAABB(const glm::vec2 &coord, float w, float h, const glm::vec2 &point);

bool isPointIntersecAABB(const glm::vec2 &coord, float w, float h, float x1, float y1);


}

#endif
