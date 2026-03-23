#ifndef OGS_ATLAS_LOADER_H
#define OGS_ATLAS_LOADER_H

#include "storage/FileSystemUtil.h"
#include "storage/TextureAtlas.h"
#include "json.hpp"


namespace ogs {

struct FileData;
class TextureAtlas;

using namespace std;

using json = nlohmann::json;


class AtlasLoader {
//support formats: TexturePacker, Hierro
public:
    TextureAtlas* genAtlas(string name, FileData* f_data);

private:
    TextureAtlas* genFromTexturePacker(json);
    bool _prepareTPframeName(string*, string*);

    TextureAtlas* genFromHiero(string name, FileData* f_data);
    void _hierroLineParse(string& ssfile, int tokenStart, int line, int* first, int* second);
        
};

}




#endif
