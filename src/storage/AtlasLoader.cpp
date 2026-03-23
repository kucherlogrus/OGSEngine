#include <fstream>
#include "storage/AtlasLoader.h"
#include "storage/TextureAtlas.h"
#include "monitoring/Logsystem.h"

using namespace std;
using namespace ogs;

using json = nlohmann::json;

TextureAtlas* AtlasLoader::genAtlas(string name, FileData* f_data) {
     int pos = name.find(".json");
     if(pos >= 0) {
         //TODO need fix this !!!
         string str(f_data->_data);
         auto pos_last = str.rfind("}");
         string ready_str = str.substr(0, pos_last+1);
         auto js = json::parse(ready_str); //TODO chek this
         //--------------------------------------
         auto check = js["meta"]["app"];
         if(check != nullptr){
             string check_str = check;
             if( check_str.find("texturepacker") > 0 ) {
                 return genFromTexturePacker(js);
             }
         }
     }

     if(name.find(".pack") > 0) {
         return genFromHiero(name, f_data);
     
     } 
     // other formats not support
     logwarning("AtlasLoader", "genAtlas", "format of file:", name, " not suppport");
     return nullptr;
}

TextureAtlas* AtlasLoader::genFromTexturePacker(json js) {
    string image = js["meta"]["image"];
    int width = js["meta"]["size"]["w"];
    int height = js["meta"]["size"]["h"];
    auto atlas = new TextureAtlas;
    atlas->setTextureName(move(image));

    string asset;
    auto frames = js["frames"];
    AtlasSpriteFrame* activeFrame = nullptr;
    for(auto it = frames.begin(); it != frames.end(); it++) {
        auto frame = *it;
        string f_name = frame["filename"];
        if (_prepareTPframeName(&f_name, &asset)) {
            activeFrame = nullptr;
        }
        int x = frame["frame"]["x"];
        int y = frame["frame"]["y"];
        int width = frame["frame"]["w"];
        int height = frame["frame"]["h"];
        if (activeFrame == nullptr) {
            activeFrame = atlas->getFrame(asset);
            if (activeFrame == nullptr) {
                activeFrame = new AtlasSpriteFrame;
                activeFrame->frame_name = asset;
                activeFrame->frameWidth = width;
                activeFrame->frameHeight = height;
                activeFrame->frame_name = f_name;
                atlas->addFrame(asset, activeFrame);
            }
        }
        activeFrame->x_frames.push_back(x);
        activeFrame->y_frames.push_back(y);
    }

    auto map = atlas->getMap();

    return atlas;
}


bool AtlasLoader::_prepareTPframeName(string*  f_name, string* asset) {
    bool reset_frame = false;
    auto slash_pos = f_name->find("/");

    if (asset->size() == 0) {

        if (slash_pos > 0) {
            *asset = f_name->substr(0, slash_pos);
        } else {
            *asset = move(*f_name);
        }

    } else {

        string tmp_asset;

        if (slash_pos > 0) {
            tmp_asset = f_name->substr(0, slash_pos);
        } else {
            tmp_asset = *f_name;
        }

        if (tmp_asset.compare(*asset) != 0) {
            *asset = move(tmp_asset);
            reset_frame = true;
        }
    }
    return reset_frame;
}


TextureAtlas* AtlasLoader::genFromHiero(string name, FileData* f_data) {

    auto atlas = new TextureAtlas;
    string ssfile = f_data->_data;
    size_t index = 0;
    const char* N = "\n"; 
    int countN = 0;
    if(ssfile.compare(index, 1, N) == 0) {
        index++; //some .pack files begins from \n simvol
    } 
    string texturename;
    size_t tokenStart = index;
    while(1){               
        if(ssfile.compare(++index, 1, N) == 0) {
            countN++;
            if(countN == 1) {
                texturename = ssfile.substr(tokenStart, index-1);               
            } else if (countN == 4) {
                countN = 0;
                tokenStart = ++index;
                break;
            }            
        }       
    }
    atlas->setTextureName(move(texturename));

    // block
    string asset_name;
    int x;
    int y;
    int width;
    int height;
    size_t line = 0;
    while(index < ssfile.size()){ 
        line++;            
        if(ssfile.compare(++index, 1, N) == 0) {
            countN++; 
            if(countN == 1 || countN == 3 || countN == 4) { 
                index++;
                switch(countN){
                    case 1:                       
                        asset_name = ssfile.substr(tokenStart, line);
                        break;
                    case 3:                        
                        _hierroLineParse(ssfile, tokenStart, line, &x, &y);
                        break;
                    case 4:
                        _hierroLineParse(ssfile, tokenStart, line, &width, &height);
                        break;
                }           
            }            
            if( countN == 7){
               countN = 0;
               auto frame = new AtlasSpriteFrame;
               frame->frame_name = asset_name;
               frame->frameWidth = width;
               frame->frameHeight = height; 
               frame->x_frames.push_back(x);
               frame->y_frames.push_back(y);               
               atlas->addFrame(asset_name, frame);                              
            }   
            line = 0;
            tokenStart = ++index;          
        }       
    }
    return atlas;
}


void AtlasLoader::_hierroLineParse(string& ssfile, int tokenStart, int line, int* first, int* second) {
    string record = ssfile.substr(tokenStart, line);
    auto line_size = record.size();
    auto tdot = record.find(":");
    record = record.substr(tdot + 1, line_size);
    auto comma = record.find(",");
    string first_str = record.substr(0, comma);
    string second_str = record.substr(comma + 1, line_size);
    *first = stoi(first_str);
    *second = stoi(second_str);
}