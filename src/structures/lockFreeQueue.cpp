#include "lockFreeQueue.h"

template <typename T>
LockFreeQueue<T>::LockFreeQueue(std::size_t size)
    : head_(nullptr),
      tail_(nullptr),
      nodePool_(sizeof(Node), size, alignof(Node))
{
}

template <typename T>
LockFreeQueue<T>::~LockFreeQueue()
{
    Node *curr = head_;

    while (curr)
    {
        Node *tmp = curr->next_;

        std::destroy_at(curr);

        nodePool_.deallocate(
            reinterpret_cast<std::byte *>(curr));

        curr = tmp;
    }
}

template <typename T>
void LockFreeQueue<T>::push_back(T *data)
{
    std::byte *block = nodePool_.allocate();

    if (!block)
        return;

    Node *node =
        reinterpret_cast<Node *>(block);

    std::construct_at(node);

    node->data = data;

    if (!head_)
    {
        head_ = node;
        tail_ = node;
        return;
    }

    tail_->next_ = node;
    node->prev_ = tail_;

    tail_ = node;
}

template <typename T>
void LockFreeQueue<T>::pop_front()
{
    if (!head_)
        return;

    Node *tmp = head_;

    head_ = head_->next_;

    if (head_)
    {
        head_->prev_ = nullptr;
    }
    else
    {
        tail_ = nullptr;
    }

    std::destroy_at(tmp);

    nodePool_.deallocate(
        reinterpret_cast<std::byte *>(tmp));
}

template <typename T>
void LockFreeQueue<T>::erase(T *data)
{
    Node *curr = head_;

    while (curr)
    {
        if (curr->data == data)
        {
            if (curr->prev_)
            {
                curr->prev_->next_ = curr->next_;
            }
            else
            {
                head_ = curr->next_;
            }

            if (curr->next_)
            {
                curr->next_->prev_ = curr->prev_;
            }
            else
            {
                tail_ = curr->prev_;
            }

            std::destroy_at(curr);

            nodePool_.deallocate(
                reinterpret_cast<std::byte *>(curr));

            return;
        }

        curr = curr->next_;
    }
}

template <typename T>
T *LockFreeQueue<T>::back()
{
    if (!tail_)
        return nullptr;

    return tail_->data;
}

template <typename T>
T *LockFreeQueue<T>::front()
{
    if (!head_)
        return nullptr;

    return head_->data;
}