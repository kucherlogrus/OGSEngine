#include "Allocator.h"
#include <cstring>
#include <monitoring/Logsystem.h>

using namespace ogs;
using namespace std;


Allocator* Allocator::instance = nullptr;

Allocator* Allocator::getInstance() {
    if (!instance){
        instance = new Allocator;
        return instance;
    }
    return instance;
}


void* Allocator::alloc(size_t size){
    auto page = pages.find(size);
    if (page != pages.end()) {
        return allocFromPage(page->second);
    } else {
        Page* newpage = new Page(size);
        pages[size] = newpage;
        return allocFromPage(newpage);
    }
};


void Allocator::dealloc(void* t, size_t size) {
    auto page = pages.find(size);
    if (page != pages.end()) {
        deallocFromPage(page->second, t);
    }
};

void Allocator::freeBySize(size_t size) {
    auto pages_pair = pages.find(size);
    if (pages_pair != pages.end()) {
        Page* page = pages_pair->second;
        do {
            pages.erase(size);
            Page* tmp = page;
            page = page->next;
            delete tmp;
        } while(page != nullptr);
    }
}

void Allocator::debug() {
    for( auto pages_pair : pages ) {
        cout << "\nPages: " << pages_pair.first << endl;
        Page* page = pages_pair.second;
        int pn = 0;
        do {
            cout << "page : " << ++pn << endl;
            int ind = 0;
            for(auto chunk : page->chunks){
                cout << "chunk: " << ++ind << " ";
                chunkDebug(chunk);

            }
            page = page->next;
        } while(page != nullptr);
    }
};

void Allocator::createCustomPage(size_t size, int chunks_count) {
     auto page = pages.find(size);
     if (page != pages.end()){
         logerror("Allocator", "createCustomPage","page with size:", size, "already exist");
         return;
     }
     Page* newpage = new Page(size, chunks_count);
     pages[size] = newpage;
}


void Allocator::chunkDebug(Chunk& chunk){
    unsigned int mask = chunk.mask;
    int index = 0;
    cout << "Chunk:" << mask <<" [";
    while (index < CHUNK_WIDTH) {
        bool state =  mask & (1 << index);
        if(state) {
            cout << "+";
        } else {
            cout << "0";
        }
        index++;
    }
    cout << "]";
    if(chunk.clear) {
        cout << " clean" << endl;
    } else {
        cout << " dirty" << endl;
    }
};


void* Allocator::allocFromPage(Page* page) {

    if(page->full) {
        return allocFromPage(page->next);
    }
    if( page->hasCleanChunk() ) {
        for(Chunk& chunk : page->chunks) {
            if( chunk.mask < CHUNK_MAX && chunk.clear) {
                int index = chunk.index;
                chunk.index++;
                chunk.mask |= 1 << index;
                return chunk.pointer + page->memory_size * index;
            }
        }
    } else {
        for(Chunk& chunk : page->chunks) {
            if( chunk.mask < CHUNK_MAX) {
                int index = high_bit_standart(chunk.mask);
                chunk.mask |= 1 << index;
                return chunk.pointer + page->memory_size * index;
            }
        }

    }
    page->full = true;
    if(page->next == nullptr) {
        auto chunk_count = page->chunksInPage;
        if(!page->custom){
            if(chunk_count < 32) {
                chunk_count *= 2;
            }
        } 
        Page* newpage = new Page(page->memory_size, chunk_count);
        page->next = newpage;
    }
    return allocFromPage(page->next);
}


void Allocator::deallocFromPage(Page* page, void* ptr) {
    char* c_ptr = (char*) ptr;
    auto left = page->pointer;
    auto right = left + page->page_size;
    bool inPage = left <= ptr && ptr < right;
    if(!inPage) {
        if(page->next == nullptr){
            logwarning("Allocator", "deallocFromPage","can't be in normal work !!");
            return;
        }
        return deallocFromPage(page->next, ptr);
    }
    int ptr_sh = c_ptr -  page->pointer;
    int chunk_index = ptr_sh / (page->memory_size * CHUNK_WIDTH);
    Chunk& chunk = page->chunks[chunk_index];
    int index = ptr_sh / page->memory_size;
    chunk.mask ^= 1 << index;
    std::memset(ptr, 0, page->memory_size);
    if (page->full) {
        page->full = false;
    }

    if(chunk.mask == 0) {
        if(chunk.clear){
            chunk.clear = false; //TODO - check it maby chunk.clear must be true ?
            page->cleanChunks++;
        }
        chunk.index = 0;
        return;
    }

    if(chunk.index == index + 1) {
        if(chunk.clear) {
            chunk.index--;
        }
    } else {
        if(chunk.clear){
            chunk.clear = false;
            page->cleanChunks--;
        }
    }
};
