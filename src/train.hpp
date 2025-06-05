#pragma once

#ifndef _TRAIN_HPP_
#define _TRAIN_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include "unique_map.hpp"
#include "utility.hpp"
#include "string_utility.hpp"
using std::string, std::cout, std::endl, std::string_view;
struct TrainID {
  char str[24];
  TrainID() = default;
  TrainID(const string_view &ID) {
    copy_string(str, ID);
  }
};
struct TrainData {
  struct TrainStationData {
    char name[32];
    int arriving_date, arriving_time, leaving_date, leaving_time, price;
  } station[100];
  char type;
  int seat[100][100];
  TrainData() = default;
};
class TrainHandler {
 private:
  UniqueMap<TrainID, TrainData> map;
 public:
  TrainHandler();
  int add_train(const string_view, const string_view, const string_view,
                const string_view, const string_view, const string_view,
                const string_view, const string_view, const string_view,
                const string_view);
  int delete_train(const string_view);
  int release_train(const string_view);
  string query_train(const string_view, const string_view);
  string query_ticket(const string_view, const string_view, const string_view, const string_view);
  string query_transfer(const string_view, const string_view, const string_view, const string_view);
};
#endif