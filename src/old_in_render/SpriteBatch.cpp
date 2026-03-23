#include <monitoring/Logsystem.h>
#include <glm/glm/gtc/type_ptr.hpp>
#include "SpriteBatch.h"
#include "Engine.h"

using namespace ogs;

SpriteBatch::SpriteBatch():
currentTexture(nullptr)
{
    init(1000);
}

SpriteBatch::SpriteBatch(size_t size):
currentTexture(nullptr)
{
    loginfo("TextureSpriteBatch","constructor");
    init(size);
}



SpriteBatch::~SpriteBatch(){
    loginfo("TextureSpriteBatch","destructor");
    currentTexture = nullptr;
}

void SpriteBatch::init(size_t size){
    loginfo("TextureSpriteBatch","init", size);
    vericles_data.reserve(size * 5);
    size_t len = size * 6;
    indexes_data.reserve(len);
    unsigned int j = 0;
    for( int i = 0; i < len; i += 6, j += 4){
        indexes_data[i] = j;
        indexes_data[i+1] = j + 1;
        indexes_data[i+2] = j + 2;
        indexes_data[i+3] = j;
        indexes_data[i+4] = j + 2;
        indexes_data[i+5] = j + 3;
    }

    program = RENDER->getShaderManager()->createSpriteBathShader();
    program->bind();
    program->addAttributes({
        { gl::A_POSITION, 2, 0},
        { gl::A_COLOR,    1, 2},
        { gl::A_UV,       2, 3}
     });
    program->addUniforms( {gl::U_PROJMATRIX, gl::U_TEXTURESPAMLER0} );
    arrayBuffer.setDrawType(buffer_draw_type::DYNAMIC_DRAW);
    indexBuffer.setDrawType(buffer_draw_type::DYNAMIC_DRAW);
    RENDER->checkBackendErrors();
}

void SpriteBatch::begin() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    program->bind();
    glActiveTexture(GL_TEXTURE0);
    matrixSetup();
    if( FPSCOUNTER.getTick(5)) {
        loginfo("SpriteBatch", "begin", program->getId());
    }
    isDrawing = true;

}

void SpriteBatch::end() {
    isDrawing = false;
    flush();
    program->unbind();
}

void SpriteBatch::flush() {

    if( FPSCOUNTER.getTick(5)) {
        loginfo("SpriteBatch", "flush", "vertex count:", vertex_count, "index count: ", index_count);
        loginfo("SpriteBatch", "flush", "currentTexture:", currentTexture->getTextureID());
        loginfo("SpriteBatch", "flush", "vericles_data size:", vericles_data.size());
        utils::showVector(vericles_data);
    }
    arrayBuffer.bind();
    program->applyAttribs();
    arrayBuffer.sendData(&vericles_data[0], array_float_size * vertex_count, vertex_count);
    indexBuffer.bind();
    indexBuffer.sendData(&indexes_data[0],  index_ui_size * index_count , index_count);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
    indexBuffer.unbind();
    arrayBuffer.unbind();
    vertex_count = 0;
    index_count = 0;

}


void SpriteBatch::changeShader() {

}

void SpriteBatch::matrixSetup(){
    if( FPSCOUNTER.getTick(5)) {
        utils::matrixPrint(mvp, "SpriteBatch::matrixSetup");
    }
    program->setUniform(gl::U_PROJMATRIX, mvp);


}

void SpriteBatch::checkTexture(Texture* texture) {
    if (currentTexture == nullptr){
        currentTexture = texture;
        RENDER->bindTexture(currentTexture);
        return;
    }
    if(currentTexture != texture) {
        flush();
        currentTexture = texture;
        RENDER->bindTexture(texture);
    }
}




void SpriteBatch::draw(Sprite2d* sprite) {
    if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw sprite");}
    checkTexture(sprite->getTexture());

    float x = sprite->position.x;
    float y = sprite->position.y;
    float width = sprite->width;
    float height = sprite->height;
    float angle = sprite->computeAngle();


    float centerX = x + width / 2;
    float centerY = y + height / 2;

    float x1 = x - centerX;
    float y1 = y - centerY;
    float x2 = x - centerX + width;
    float y3 = y - centerY + height;


    // scaling
    float scaleX = sprite->scale.x;
    float scaleY = sprite->scale.y;

    if(scaleX != 1 || scaleY != 1){
        x1 *= scaleX;
        x2 *= scaleX;
        y1 *= scaleY;
        y3 *= scaleY;
    }

    float y2 = y1;
    float x3 = x2;
    float x4 = x1;
    float y4 = y3;


    float xleftup;
    float yleftup;

    float xrightup;
    float yrightup;

    float xrightdown;
    float yrightdown;

    float xleftdown;
    float yleftdown;


    // rotation
    if(angle != 0) {
        if(sprite->center_rotate.x != 0){
            centerX = sprite->center_rotate.x;
            x1 = x - centerX;
            x2 = x - centerX + width;
            x3 = x2;
            x4 = x1;
        } 

        if(sprite->center_rotate.y != 0){
            centerY = sprite->center_rotate.y;
            y1 = y - centerY;
            y2 = y1;
            y3 = y - centerY + height;
            y4 = y3;
        }
        
        float cos = glm::cos(glm::radians(angle));
        float sin = glm::sin(glm::radians(angle));
        xleftup = cos * x1 - sin * y1;
        yleftup = sin * x1 + cos * y1;
        xrightup = cos * x2 - sin * y2;
        yrightup = sin * x2 + cos * y2;
        xrightdown = cos * x3 - sin * y3;
        yrightdown = sin * x3 + cos * y3;
        xleftdown = cos * x4 - sin * y4;
        yleftdown = sin * x4 + cos * y4;
    } else {
        xleftup = x1;
        yleftup = y1;
        xrightup = x2;
        yrightup = y2;
        xrightdown = x3;
        yrightdown = y3;
        xleftdown = x4;
        yleftdown = y4;
    }

    // uv coorinates

    float u1 = 0.0f;
    float v1 = 1.0f;
    float u2 = v1;
    float v2 = u1;

    Animation* animation = sprite->getCurrentAnimation();

    if(animation != nullptr){
        float t_width = (float) 1 / sprite->texture->getWidth();
        float t_height = (float) 1 / sprite->texture->getHeight();
        u1 =  t_width * animation->getPointX();
        v2 =  1 - (t_height * (animation->getPointY() + animation->getFrameHeight()));
        u2 =  t_width * (animation->getPointX() + animation->getFrameWidth());
        v1 =  1 - (t_height * animation->getPointY());
    } else {
        if (sprite->srcPos.x != 0 || sprite->srcPos.y != 0) {
            float t_width = (float) 1 / sprite->texture->getWidth();
            float t_height = (float) 1 / sprite->texture->getHeight();
            u1 =  t_width * sprite->srcPos.x;
            v2 =  1 - (t_height * (sprite->srcPos.y + sprite->srcHeight));
            u2 =  t_width * (sprite->srcPos.x + sprite->srcWidth);
            v1 =  1 - (t_height * sprite->srcPos.y);
        }
    }

    //uv flips

    if(sprite->flipX){
        float tmpX = u1;
        u1 = u2;
        u2 = tmpX;
    }

    if(sprite->flipY){
        float tmpY = v1;
        v1 = v2;
        v2 = tmpY;
    }


    float alpha = sprite->alpha;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw sprite before setting data", "vericles_data", vericles_data.size(), "verticles count:", vertex_count);}
    vericles_data[vertex_count++] = xleftup + centerX;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[0]);}
    vericles_data[vertex_count++] = yleftup + centerY;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[1]);}
    vericles_data[vertex_count++] = alpha;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[2]);}
    vericles_data[vertex_count++] = u1;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[3]);}
    vericles_data[vertex_count++] = v1;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[4]);}

    vericles_data[vertex_count++] = xrightup +centerX;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[5]);}
    vericles_data[vertex_count++] = yrightup + centerY;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[6]);}
    vericles_data[vertex_count++] = alpha;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[7]);}
    vericles_data[vertex_count++] = u2;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[8]);}
    vericles_data[vertex_count++] = v1;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[9]);}

    vericles_data[vertex_count++] = xrightdown +centerX;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[10]);}
    vericles_data[vertex_count++] = yrightdown + centerY;
   // if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[11]);}
    vericles_data[vertex_count++] = alpha;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[12]);}
    vericles_data[vertex_count++] = u2;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[13]);}
    vericles_data[vertex_count++] = v2;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[14]);}

    vericles_data[vertex_count++] = xleftdown + centerX;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[15]);}
    vericles_data[vertex_count++] = yleftdown + centerY;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[16]);}
    vericles_data[vertex_count++] = alpha;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[17]);}
    vericles_data[vertex_count++] = u1;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[18]);}
    vericles_data[vertex_count++] = v2;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw", vericles_data[19]);}

    index_count += 6;
    //if( FPSCOUNTER.getTick(5)) {loginfo("SpriteBatch", "draw sprite after setting data", "vericles_data", vericles_data.size(), "verticles count:", vertex_count);  utils::showVector(vericles_data);}
}


void SpriteBatch::draw(FreeTypeFont* bitmap, std::string text, float x, float y) {
    checkTexture(bitmap->font_texture);
    float pen_x = 0.0f;
    int msg_size = text.size();
    int c = 0;
    for(int i = 0; i < msg_size; i++) {
        c = text[i];
        float leftUpX = x + pen_x + bitmap->off_x[c];
        float leftUpY = y + bitmap->off_y[c];
        float rightUpX = x + pen_x + bitmap->off_x[c] + bitmap->w[c];
        float rightUpY = leftUpY;
        float rightDownX = rightUpX;
        float rightDownY = y + bitmap->off_y[c] + bitmap->h[c];
        float leftDownX = leftUpX;
        float leftDownY = rightDownY;
        vericles_data[vertex_count++] = leftUpX;
        vericles_data[vertex_count++] = leftUpY;
        vericles_data[vertex_count++] = 1.0f;
        vericles_data[vertex_count++] = bitmap->u1[c];
        vericles_data[vertex_count++] = bitmap->v1[c];
        vericles_data[vertex_count++] = rightUpX;
        vericles_data[vertex_count++] = rightUpY;
        vericles_data[vertex_count++] = 1.0f;
        vericles_data[vertex_count++] = bitmap->u2[c];
        vericles_data[vertex_count++] = bitmap->v1[c];
        vericles_data[vertex_count++] = rightDownX;
        vericles_data[vertex_count++] = rightDownY;
        vericles_data[vertex_count++] = 1.0f;
        vericles_data[vertex_count++] = bitmap->u2[c];
        vericles_data[vertex_count++] = bitmap->v2[c];
        vericles_data[vertex_count++] = leftDownX;
        vericles_data[vertex_count++] = leftDownY;
        vericles_data[vertex_count++] = 1.0f;
        vericles_data[vertex_count++] = bitmap->u1[c];
        vericles_data[vertex_count++] = bitmap->v2[c];
        pen_x += bitmap->advance[c];
        index_count += 6;
    }
}

void SpriteBatch::draw(Texture *texture, float x, float y, float width, float height) {
    checkTexture(texture);

    vericles_data[vertex_count++] = x;
    vericles_data[vertex_count++] = y;

    vericles_data[vertex_count++] = 1.0f;

    vericles_data[vertex_count++] = 0.0f;
    vericles_data[vertex_count++] = 1.0f;

    vericles_data[vertex_count++] = x + width;
    vericles_data[vertex_count++] = y;

    vericles_data[vertex_count++] = 1.0f;

    vericles_data[vertex_count++] = 1.0;
    vericles_data[vertex_count++] = 1.0;

    vericles_data[vertex_count++] = x + width;
    vericles_data[vertex_count++] = y + height;

    vericles_data[vertex_count++] = 1.0f;

    vericles_data[vertex_count++] = 1.0;
    vericles_data[vertex_count++] = 0.0;

    vericles_data[vertex_count++] = x;
    vericles_data[vertex_count++] = y + height;

    vericles_data[vertex_count++] = 1.0f;

    vericles_data[vertex_count++] = 0.0;
    vericles_data[vertex_count++] = 0.0;

    index_count += 6;
}

void SpriteBatch::draw(Texture *texture, float x, float y, float width, float height, float scaleX, float scaleY,
                              float srcX, float srcY, float srcWidth, float srcHeight, float angle, float alpha) {

}





