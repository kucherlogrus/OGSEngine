#include "Texture.h"
#include "monitoring/Logsystem.h"

using namespace ogs;

Texture::Texture(std::string _name, int w, int h, int id){
    name = _name;
    width = w;
    height = h;
    textureID = id;
}
