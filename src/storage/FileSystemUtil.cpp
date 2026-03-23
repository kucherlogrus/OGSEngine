#include <cstring>
#include "storage/FileSystemUtil.h"
#include "render/PixelFormat.h"
#include "png.h"


using namespace ogs;


struct ImageSrc {
    char* data;
    size_t size;
    size_t offset;
};


FileData* FileSystemUtil::getDataFromFile(std::string filename) {
    // implement in platform backend (e.g. DesktopFileSystem)
    return nullptr;
}

FileData* FileSystemUtil::loadJson(std::string name) {
    return getDataFromFile(name);
}


TextureData* FileSystemUtil::loadJPEG(std::shared_ptr<FileData> data){
    return nullptr;
}



TextureData* FileSystemUtil::loadImage(std::string filename){

    std::shared_ptr<FileData> data(getDataFromFile(filename));

    TextureData* t_data = nullptr;

    if(filename.find(".png") > 0) {
        t_data = loadPNG(data);
        t_data->name = filename;
        return t_data;
    }

    else if (filename.find(".jpeg") > 0){
        return loadJPEG(data);
    }

    return nullptr;
};

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
    ImageSrc* isource = (ImageSrc*)png_get_io_ptr(png_ptr);
    if( (int)(isource->offset + length) <= isource->size ) {
        std::memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

TextureData* FileSystemUtil::loadPNG(std::shared_ptr<FileData> data) {

    png_byte header[8] = {0};

    std::memcpy(header, data->_data, 8);

    if (png_sig_cmp(header, 0, 8))
    {
        logerror("FileSystemUtil", "error is not a PNG:");
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
    {
        logerror("FileSystemUtil", "error: png_create_read_struct returned 0");
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
    {
        logerror("FileSystemUtil", "error: png_create_info_struct returned 0");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return 0;
    }

    if ( setjmp(png_jmpbuf(png_ptr)) ) {
        logerror("FileSystemUtil", "error from libpng");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    ImageSrc imagedata { data->_data, data->size, 0 };

    png_set_read_fn(png_ptr, &imagedata, pngReadCallback);

    png_read_info(png_ptr, info_ptr);

    int bit_depth, color_type;

    png_uint_32 temp_width, temp_height;

    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if(color_type==PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png_ptr);
        color_type=PNG_COLOR_TYPE_RGB_ALPHA;
    }

    if (bit_depth != 8) {
        logerror("FileSystemUtil", "Unsupported bit depth:", bit_depth);
        return 0;
    }


    PixelFormat format;

    switch(color_type) {
        case PNG_COLOR_TYPE_RGB:
            format = PixelFormat::RGB8;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            format = PixelFormat::RGBA8;
            break;
        default:
            logerror("FileSystemUtil", "Unknown libpng color type", color_type);
            return 0;
    }

    png_read_update_info(png_ptr, info_ptr);

    auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    rowbytes += 3 - ((rowbytes-1) % 4);

    png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);

    if (image_data == NULL) {
        logerror("FileSystemUtil", "could not allocate memory for PNG image data");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));

    if (row_pointers == NULL) {
        logerror("FileSystemUtil", "could not allocate memory for PNG row pointers");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        return 0;
    }

    for (unsigned int i = 0; i < temp_height; i++) {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    png_read_image(png_ptr, row_pointers);

    TextureData* textureData = new TextureData;
    textureData->width =  temp_width;
    textureData->height = temp_height;
    textureData->pixdata = image_data;
    textureData->format = format;
    return  textureData;
}

void FileSystemUtil::savePNG(const char *filename, uint8_t *pixels, int w, int h){
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        return;

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, &info);
        return;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_write_struct(&png, &info);
        return;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, w, h, 8 /* depth */, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette) {
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        return;
    }
    png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png, info);
    png_set_packing(png);

    png_bytepp rows = (png_bytepp)png_malloc(png, h * sizeof(png_bytep));
    for (int i = 0; i < h; ++i)
        rows[i] = (png_bytep)(pixels + (h - i) * w * 3);

    png_write_image(png, rows);
    png_write_end(png, info);
    png_free(png, palette);
    png_destroy_write_struct(&png, &info);

    fclose(fp);
    delete[] rows;

}




