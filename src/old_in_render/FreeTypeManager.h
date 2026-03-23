#ifndef OGS_FREE_TYPE_TEXT_H
#define OGS_FREE_TYPE_TEXT_H


#include "SpriteBatch.h"
#include "FreeTypeFont.h"
#include <unordered_map>
#include <storage/FileSystemUtil.h>

namespace ogs {

class FreeTypeFont;
class FileData;

class FreeTypeManager {

public:
    FreeTypeFont* getFont(std::string name);
    FreeTypeFont* load_font(FileData* f_data, std::string name, int point_size, int dpi, bool async);
        
};


}




#endif
