#include "order.h"

Order::Order(uint32_t price, uint32_t quantity, Side side, Type type, uint64_t id)
        : price_(price), quantity_(quantity), type_(type), id_(id) {};