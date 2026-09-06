#include "memPool.h"

#include <iostream>
#include <format>

MemoryPool::MemoryPool(std::size_t blockSize, std::size_t blockCount, std::size_t alignment)
                         : blockSize_(blockSize), blockCount_(blockCount), alignment_(alignment) {

    std::cout<<"Allocating blocks of sz " << blockSize_ << " and count " << blockCount_ << " with alignment of " << alignment_ << "\n";
    
    // treat memory as btyes
    mBlock = static_cast<std::byte*>(::operator new(blockSize_ * blockCount_));                        

    // freeList_.reserve(blockCount_);
    
    // for (std::size_t i = 0; i < blockCount; ++i) {
    //     auto offset = i * blockSize_;

    //     freeList_.push_back(mBlock + offset);
    // }

    for (std::size_t i = 0; i < blockCount_; ++i) {
        auto offset = mBlock + i * blockSize_;

        Node* node = reinterpret_cast<Node*>(offset);

        node->next = head_;
        head_ = node;
    }
}

MemoryPool::~MemoryPool() {
    std::cout<<"Deallocating block\n";

    ::operator delete(mBlock);
}

std::byte* MemoryPool::allocate() {
    // if (freeList_.empty()) {
    //     std::cout<<"freeList is out of memory\n";
    //     return nullptr;
    // }

    // std::byte* blk = freeList_.back();
    // freeList_.pop_back();
    // return blk;
    if (!head_) return nullptr;

    Node* curr = head_;
    head_ = head_->next;
    return reinterpret_cast<std::byte*>(curr);

}

void MemoryPool::deallocate(std::byte* p) {
    // freeList_.push_back(p);
    Node* node = reinterpret_cast<Node*>(p);
    node->next = head_;
    head_ = node;
}

