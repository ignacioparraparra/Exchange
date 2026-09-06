#ifndef ORDER_H
#define ORDER_H

#include <cstdint>


enum class Side {
    BID,
    ASK
};

enum class Type {
    LIMIT,
    MARKET
};

struct Order {
    Order(uint32_t price, uint32_t quantity, Side side, Type type, uint64_t id);

    uint32_t price_;
    uint32_t quantity_;
    Side side_;
    Type type_;
    uint64_t id_;
};

#endif