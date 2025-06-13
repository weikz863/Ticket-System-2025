#pragma once

#ifndef _TICKET_HPP_
#define _TICKET_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include "unique_map.hpp"
#include "utility.hpp"
#include "string_utility.hpp"
#include "user.hpp"
#include "train.hpp"
using std::string, std::cout, std::endl, std::string_view;
struct Order {
  int trainID, stamp, start, end, num;
  enum class Status {
    SUCCESS, PENDING, REFUNDED,
  } status;
  bool operator < (const Order& other) const {
    return trainID < other.trainID || 
      (trainID == other.trainID && stamp < other.stamp);
  }
  Order() = default;
  Order(int x) : trainID(x) {}
};
class TicketHandler {
 private:
  BlockBlockList<Order, 4096 / sizeof(Order)> order;
  BlockBlockList<trivial_pair<int, trivial_pair<int, int>>, 
      4096 / sizeof(trivial_pair<int, trivial_pair<int, int>>)> userorder;
 public:
  TicketHandler();
  string buy_ticket(const string_view, const string_view, const string_view, 
                    const string_view, const string_view, const string_view,
                    const string_view);
  string query_order(const string_view);
  int refund_ticket(const string_view, const string_view);
};
#endif