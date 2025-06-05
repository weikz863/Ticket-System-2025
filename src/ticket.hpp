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
class TicketHandler {
 private:
 public:
  TicketHandler();
  string buy_ticket(const string_view, const string_view, const string_view, 
                    const string_view, const string_view, const string_view,
                    const string_view);
  string query_order(const string_view);
  int refund_ticket(const string_view, const string_view);
};
#endif