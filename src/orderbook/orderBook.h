#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"

#include <cstdint>
#include <map>
#include <vector>

using Price = std::uint32_t;
using Orders = std::vector<Order>;
using PriceLevels = std::map<Price, Orders>;

struct Levels {
    PriceLevels prices_;
    std::uint64_t volume;
};

class OrderBook {
    void add(Order order);

    private:
        Levels bids;
        Levels asks;
        Price bestBid;
        Price bestAsk;
        std::uint64_t volume;
};

#endif