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
  int train_pos, start, end, num, date;
  enum class Status {
    SUCCESS, PENDING, REFUNDED,
  } status;
  Order() = default;
  Order(int train_pos_, int start_, int end_, int num_, int date_, 
    Status status_ = Status::SUCCESS) : 
      train_pos(train_pos_), start(start_), end(end_), num(num_), date(date_), 
      status(status_) {}
};
class TicketHandler {
 private:
  UniqueMap<int, Order> order; // timestamp to order info
  BlockBlockList<trivial_pair<int, int>, 
      4096 / sizeof(trivial_pair<int, int>)> userorder; // userID to order timestamp
  using TupleIntIntInt = trivial_pair<trivial_pair<int, int>, int>;
  BlockBlockList<TupleIntIntInt, 4096 / sizeof(TupleIntIntInt)> queueing;
  // trainID and date to order timestamp
 public:
  TicketHandler();
  string buy_ticket(const string_view, const string_view, const string_view, 
                    const string_view, const string_view, const string_view,
                    const string_view, const string_view);
  string query_order(const string_view);
  int refund_ticket(const string_view, const string_view);
};
#endif