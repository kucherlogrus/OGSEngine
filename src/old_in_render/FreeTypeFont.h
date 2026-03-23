#ifndef OGS_FONT_H
#define OGS_FONT_H

#include "SpriteBatch.h"

namespace ogs {
using namespace std;

class FreeTypeFont : public Resource {

friend class SpriteBatch;
public:
    const int mask = T_FONT;
    string fontname;
    string fontUrl; 
    Texture* font_texture;
    float advance[128];
    float w[128];
    float h[128];
    float u1[128];
    float u2[128];
    float v1[128];
    float v2[128];
    float off_x[128];
    float off_y[128];
    float color;
    float size;
    const int getMask() override { return mask; }

};

}


#endif
