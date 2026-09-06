#include <cstddef> 
#include <vector>

#ifndef MEMPOOL_H
#define MEMPOOL_H

class MemoryPool {
    public: 
        MemoryPool(std::size_t blockSize, std::size_t blockCount, std::size_t alignment);

        ~MemoryPool();
        
        std::byte* allocate();

        void deallocate(std::byte* p);
        
        struct Node {
            Node* next;
        };

    private:
        std::size_t blockSize_;
        std::size_t blockCount_;
        std::size_t alignment_;
        std::byte* mBlock;
        Node* head_;
    //    std::vector<std::byte*> freeList_;
};

#endif