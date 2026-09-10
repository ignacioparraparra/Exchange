#ifndef LOCKFREEQUEUE_H
#define LOCKFREEQUEUE_H

#include <cstddef>
#include <memory>

#include "memPool.h"

/*
    Queue is implemented as a doubly linked list. It requests memory from its memory pool
    to remove allocation & deallocation overhead. This will maintain the sequential order
    of incoming orders. Useful for O(1) deletion, O(1) insertion to head and tail

    Inorder to find element to delete in O(1) we need to maintain a reference to the orders
    address to perform erase. <order_id : order_addr> -> unlink prev and next -> deallocate addr

    <priceLevel : OrdersQueue> add, match
    <order_id : order_addr> cancel
*/

template <typename T>
class LockFreeQueue
{
public:
    struct Node
    {
        Node *next_ = nullptr;
        Node *prev_ = nullptr;
        T *data = nullptr;
    };

    LockFreeQueue(std::size_t size);
    ~LockFreeQueue();

    void push_back(T *data);
    void pop_front();
    void erase(T *data);

    T *back();
    T *front();

private:
    Node *head_;
    Node *tail_;
    MemoryPool nodePool_;
};

#endif