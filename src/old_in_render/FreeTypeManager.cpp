#include <backends/GLConfig.h>
#include <utils/utils.h>
#include "FreeTypeManager.h"
#include "Engine.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace ogs;


FreeTypeFont* FreeTypeManager::load_font(FileData* f_data, std::string name, int point_size, int dpi, bool async) {
    loginfo("FreeTypeText", "load front", "loading TYPE MANAGER");
    FT_Library library;
    FT_Face face;
    int c;
    int i, j;


    if(FT_Init_FreeType(&library)) {
        logerror("FreeTypeText", "load_font" ,"Error loading Freetype library");
        return nullptr;
    }
    if (FT_New_Memory_Face(library, (FT_Byte*)f_data->_data, f_data->size, 0, &face)) {
        logerror("FreeTypeText", "load_font" ,"Error loading font", name);
        return nullptr;
    }
    if(FT_Set_Char_Size ( face, point_size * 64, point_size * 64, dpi, dpi)) {
        logerror("FreeTypeText", "load_font" ,"Error initializing character parameters");
        return nullptr;
    }

    int segment_size_x = 0, segment_size_y = 0;
    int num_segments_x = 16;
    int num_segments_y = 8;
    FT_GlyphSlot slot;
    FT_Bitmap bmp;
    int glyph_width, glyph_height;
    for(c = 0; c < 128; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            logerror("FreeTypeText", "load_font", "FT_Load_Char failed");

            return nullptr;
        }
        slot = face->glyph;
        bmp = slot->bitmap;
        glyph_width = bmp.width;
        glyph_height = bmp.rows;
        if (glyph_width > segment_size_x) {
            segment_size_x = glyph_width;
        }
        if (glyph_height > segment_size_y) {
            segment_size_y = glyph_height;
        }
    }
    int font_tex_width = utils::nextpow2(num_segments_x * segment_size_x);
    int font_tex_height = utils::nextpow2(num_segments_y * segment_size_y);
    int bitmap_offset_x = 0, bitmap_offset_y = 0;
    GLubyte* font_texture_data = (GLubyte*) malloc(sizeof(GLubyte) * 2 * font_tex_width * font_tex_height);
    memset((void*)font_texture_data, 0, sizeof(GLubyte) * 2 * font_tex_width * font_tex_height);

    auto font = new FreeTypeFont;

    if (!font_texture_data) {
        logerror("FreeTypeText", "load_font", "Failed to allocate memory for font texture");
        delete font;
        return nullptr;
    }
    for(c = 0; c < 128; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            logerror("FreeTypeText", "load_font", "FT_Load_Char failed\n");
            delete font;
            return nullptr;
        }
        slot = face->glyph;
        bmp = slot->bitmap;
        glyph_width = utils::nextpow2(bmp.width);
        glyph_height = utils::nextpow2(bmp.rows);
        div_t temp = div(c, num_segments_x);
        bitmap_offset_x = segment_size_x * temp.rem;
        bitmap_offset_y = segment_size_y * temp.quot;
        for (j = 0; j < glyph_height; j++) {
            for (i = 0; i < glyph_width; i++) {
                font_texture_data[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * font_tex_width) + 0] =
                font_texture_data[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * font_tex_width) + 1] =
                        (i >= bmp.width || j >= bmp.rows)? 0 : bmp.buffer[i + bmp.width * j];
            }
        }
        font->advance[c] = (float)(slot->advance.x >> 6);
        font->u1[c] = (float)bitmap_offset_x / (float) font_tex_width;
        font->u2[c] = (float)(bitmap_offset_x + bmp.width) / (float)font_tex_width;
        font->v1[c] =  (float)bitmap_offset_y / (float) font_tex_height;
        font->v2[c] = (float)(bitmap_offset_y + bmp.rows) / (float)font_tex_height;
        font->w[c] = bmp.width;
        font->h[c] = bmp.rows;
        font->off_x[c] = (float)slot->bitmap_left;
        font->off_y[c] = (float)  ( (slot->metrics.height - slot->metrics.horiBearingY-face->glyph->metrics.height)  >> 6);
    }

    auto t_data = new TextureData;
    t_data->name = name;
    t_data->width = font_tex_width;
    t_data->height = font_tex_height;
#ifdef MACOS
    t_data->format = GL_RG;
#else
    t_data->format = GL_LUMINANCE_ALPHA;
#endif
    t_data->pixdata = font_texture_data;
    t_data->pixlen = sizeof(GLubyte) * 2 * font_tex_width * font_tex_height;
    font->font_texture = ASSETS->genTexture(name, t_data, async);
    font->fontname = name;
    font->size = point_size;

    if(!async){
        int err = glGetError();
        if (err != 0) {
            logerror("FreeTypeText", "load_font", "GL Error 0x", err);
            delete font;
            return nullptr;
        }
        delete t_data;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    loginfo("FreeTypeText", "load front", "font was loaded");
    return font;
}

FreeTypeFont *FreeTypeManager::getFont(std::string name) {
    return nullptr;
}

