#ifndef OGS_FILESYSTEMUTILS_H
#define OGS_FILESYSTEMUTILS_H

#include <iostream>
#include <memory>
#include "storage/Texture.h"

namespace ogs {

    struct FileData {
        char* _data;
        size_t size;
        FileData(char* a, size_t b): _data(a), size(b) {};
        ~FileData(){
            free(_data);
        } 
    };


    class FileSystemUtil {

    public:
        FileSystemUtil() = default;
        virtual ~FileSystemUtil() = default;

        TextureData* loadImage(std::string filename);
        void savePNG(const char *filename, uint8_t *pixels, int w, int h);
        FileData* loadJson(std::string name);
        virtual FileData* getDataFromFile(std::string filename);

        virtual std::string& getAppPath() { return app_path; }
        void setAppPath(std::string&& a)  { app_path = a; }

    protected:
        std::string app_path;
        TextureData* loadPNG(std::shared_ptr<FileData> data);
        TextureData* loadJPEG(std::shared_ptr<FileData> data);




        


    };

}

#endif
