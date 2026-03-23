#ifndef ALLOCATOR_H
#define ALLOCATOR_H


#include <iostream>
#include <vector>
#include <unordered_map>

namespace ogs {

using namespace std;


#define CHUNK 1
#define CHUNK_TYPE uint32_t

constexpr CHUNK_TYPE bitsToOne(CHUNK_TYPE type) noexcept {
    return (type == 0 ? type - 1 : 0);
}

constexpr CHUNK_TYPE CHUNK_MAX = bitsToOne(0);

constexpr int CHUNK_WIDTH = sizeof(CHUNK_TYPE) * 8;


inline int high_bit_standart(CHUNK_TYPE num){
        int index = 0;

#if defined(__GNUC__)
        if(num != 0){
            CHUNK_TYPE n = num ^ CHUNK_MAX;
            index = __builtin_ctz(n);
        }
#else
        while (index <= CHUNK_WIDTH) {
        if(!(num & (1 << index))) {
            break;
        }
        index++;
    }
    //cout << std::bitset<32>(num)<< endl;
    //cout << std::bitset<32>(num ^ CHUNK_MAX)<< endl;
    //cout << "index: " << index << " " << index2 << endl;
#endif
        return index;
    }



class Allocator {
public:
    void* alloc(size_t size);

    void dealloc(void* t, size_t size);

    void freeBySize(size_t size);

    void debug();

    static Allocator *getInstance();

    void createCustomPage(size_t size, int chunk_count); 


private:

    static Allocator *instance;

    struct Chunk {
        CHUNK_TYPE mask = 0;
        bool clear;
        unsigned short free;
        char* pointer;
        int index;
        explicit Chunk(char* ptr): mask(0), free(CHUNK_WIDTH), clear(true), index(0)  { pointer = ptr; };
    };


    struct Page {
        // change or not monitor to full/size memory etc
        Page* next;
        int memory_size = 0;
        size_t page_size = 0;
        int cleanChunks;
        vector<Chunk> chunks;
        bool full;
        bool custom = false;
        char* pointer;
        int chunksInPage = 0;
        explicit Page(int size, int chunks_count = CHUNK):
          next(nullptr),
          memory_size(size),
          full(false),
          pointer(nullptr),  
          cleanChunks(chunks_count),
          chunksInPage(chunks_count),
          custom(false)
        {
            if(chunksInPage != CHUNK) {
               custom = true; 
            } 
            page_size = sizeof(char) * (CHUNK_WIDTH * memory_size) * chunksInPage ;
            pointer = (char*) malloc(page_size);
            chunks.reserve(chunksInPage);
            for(int i = 0; i < chunksInPage; i++){
                int align = i * memory_size * CHUNK_WIDTH;
                char* ptr = pointer + align;
                Chunk chunk(ptr);
                chunks.push_back(chunk);
            }
        };

        ~Page() {
            free(pointer);
        };

        bool hasCleanChunk() { return  cleanChunks > 0; };
    };


    void* allocFromPage(Page* page);

    void deallocFromPage(Page* page, void* ptr);

    void chunkDebug(Chunk& chunk);

    int getChunkFromAdress(Page* page, void* ptr) { return 42; };

    unordered_map<int, Page*> pages;



};

    
};

#endif //ALLOCATOR_H
