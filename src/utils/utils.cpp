#include "utils.h"

using namespace ogs;

void utils::matrixPrint(glm::mat4 matrix, std::string name) {
    logdebug("Utils","matrixPrint", name.c_str());
    logdebug("Utils","matrixPrint", matrix[0][0],matrix[0][1], matrix[0][2], matrix[0][3] );
    logdebug("Utils","matrixPrint", matrix[1][0],matrix[1][1], matrix[1][2], matrix[1][3] );
    logdebug("Utils","matrixPrint", matrix[2][0],matrix[2][1], matrix[2][2], matrix[2][3] );
    logdebug("Utils","matrixPrint", matrix[3][0],matrix[3][1], matrix[3][2], matrix[3][3] );
}

void utils::vector3Print(glm::vec3 vector, std::string name){
    logdebug("Utils", "matrixPrint","\t--------- ----\n", name.c_str());
    logdebug("Utils", "matrixPrint","\t",vector.x, vector.y, vector.z );
}

unsigned int utils::nextpow2(int x) {
    unsigned int val = 1;
    while (val < x) val <<= 1;
    return val;
}


int utils::getRandomInt(int min, int max){
    return min + (std::rand() % (int)(max - min + 1));
}

bool utils::isPointIntersecAABB(float x, float y, float w, float h, float x1, float y1) {
    return x < x1 && (x + w) > x1 && y < y1 && (y + h) > y1;
}

bool utils::isPointIntersecAABB(const glm::vec2& coord, float w, float h, const glm::vec2& point) {
    return isPointIntersecAABB(coord.x, coord.y, w, h, point.x, point.y);
}

bool utils::isPointIntersecAABB(const glm::vec2& coord, float w, float h, float x1, float y1) {
    return isPointIntersecAABB(coord.x, coord.y, w, h, x1, y1);
}
