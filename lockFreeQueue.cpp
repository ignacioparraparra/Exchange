#include "memPool.h"

template <typename T>
class LockFreeQueue
{
    public:
    LockFreeQueue(std::size_t size) 
    : head_(nullptr), tail_(nullptr), nodePool_(MemoryPool(sizeof(Node), size, alignof(Node))) {
    };

    ~LockFreeQueue() {

        curr = head_;

        while (curr) {
            Node* tmp = curr->next_;
            std::destroy_at(curr);
            nodePool_->deallocate(reinterpret_cast<std::byte*>(curr));
            curr = tmp;
        }

        delete nodePool_;
    }

    struct Node
    {
        Node *next_ = nullptr;
        Node *prev_ = nullptr;
        T *data = nullptr;
    };

    Node* getNode() 
    {   
        std::byte* block = nodePool_->allocate();
        return std::construct_at(reinterpret_cast<Node*>(block));
    }

    void push_back(T* data)
    {   
        std::byte* block = nodePool_->allocate();
        Node* node = reinterpret_cast<Node*>(block);
        std::construct_at(node);
        node->data = data;

        if (!head_)
        {
            head_ = node;
            tail_ = head_;
            return;
        }

        tail_->next_ = node;
        node->prev_ = tail_;
        tail_ = node;
        tail_->next_ = nullptr;
    }

    void pop_front() {
        Node* tmp = head_;
        head_ = head->next;
        std::destroy_at(tmp);
        nodePool_->deallocate(reinterpret_cast<std::byte*>(curr));
    }

    void erase(T* data)
    {
        if (!head_)
            return;

        Node *curr = head_;

        while (curr)
        {
            if (curr->data == data)
            {
                curr->prev_->next_ = curr->next_;
                curr->next_->prev_ = curr->prev_;
                std::destroy_at(curr);
                nodePool_->deallocate(reinterpret_cast<std::byte*>(curr));
                return;
            }
            curr = curr->next_;
        }
    }

    T *back()
    {
        return tail_->data;
    }

    T *front()
    {
        return head_->data;
    }

private:
    Node *head_;
    Node *tail_;
    MemoryPool nodePool_;
};