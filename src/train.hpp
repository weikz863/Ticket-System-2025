#pragma once

#ifndef _TRAIN_HPP_
#define _TRAIN_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include <charconv>
#include <sstream>
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
  bool operator < (const TrainID &x) const {
    return std::strcmp(str, x.str) < 0;
  }
};
struct TrainData {
  struct TrainStationData {
    char name[32];
    Time arriving, leaving;
    int price;
  } station[25];
  char type, stationNumInt, trainID[32], released;
  int seat[100][25];
  TrainData() = default;
  TrainData(const string_view& trainID_, const string_view& stationNum, const string_view& seatNum, 
      const string& stations, const string& prices, const string_view& startTime, 
      const string& travelTimes, const string& stopoverTimes, 
      const string& saleDate, const string_view& type_) : type{type_[0]}, seat{}, released{0} {
    copy_string(trainID, trainID_);
    int seatNumInt;
    std::from_chars(stationNum.begin(), stationNum.end(), stationNumInt);
    std::from_chars(seatNum.begin(), seatNum.end(), seatNumInt);
    Time current(startTime);
    std::istringstream stationStream(stations), priceStream(prices), 
      travelTimeStream(travelTimes), stopoverTimeStream(stopoverTimes), 
      saleDateStream(saleDate);
    string tmpstr;
    int tmpint;
    for (int i = 0; i < stationNumInt; i++) {
      std::getline(stationStream, tmpstr, '|');
      std::strcpy(station[i].name, tmpstr.c_str());
      if (i == 0) {
        station[i].arriving = Time(nullptr);
        station[i].price = 0;
      } else {
        station[i].arriving = current;
        std::getline(stopoverTimeStream, tmpstr, '|');
        std::from_chars(tmpstr.c_str(), tmpstr.c_str() + tmpstr.size(), tmpint);
        current += tmpint;
        std::getline(priceStream, tmpstr, '|');
        std::from_chars(tmpstr.c_str(), tmpstr.c_str() + tmpstr.size(), tmpint);
        station[i].price = station[i - 1].price + tmpint;
      }
      if (i == stationNumInt - 1) {
        station[i].leaving = Time(nullptr);
      } else {
        station[i].leaving = current;
        std::getline(travelTimeStream, tmpstr, '|');
        std::from_chars(tmpstr.c_str(), tmpstr.c_str() + tmpstr.size(), tmpint);
        current += tmpint;
      }
    }
    station[stationNumInt].name[0] = 0;
    int first_date, last_date;
    std::getline(saleDateStream, tmpstr, '|');
    first_date = datify(tmpstr);
    std::getline(saleDateStream, tmpstr, '|');
    last_date = datify(tmpstr);
    for (int date = first_date; date <= last_date; date++) {
      for (int j = 0; j < stationNumInt; j++) {
        seat[date][j] = seatNumInt;
      }
    }
  }
};
struct NameAndTrain {
  char name[32];
  int train;
  NameAndTrain() = default;
  NameAndTrain(const char *name_, int train_) : train(train_) {
    std::memcpy(name, name_, 32);
  }
  NameAndTrain(const string_view& name_, int train_) : train(train_) {
    copy_string(name, name_);
  }
  bool operator < (const NameAndTrain &x) const {
    int t = std::strcmp(name, x.name);
    return t < 0 || (t == 0 && train < x.train);
  }
};
class TrainHandler {
  friend class TicketHandler;
 private:
  UniqueMap<TrainID, TrainData> map;
  BlockBlockList<NameAndTrain, 4096 / sizeof(sjtu::pair<NameAndTrain, int>)> tree;
 public:
  TrainHandler();
  int add_train(const string_view, const string_view, const string_view,
                const string&, const string&, const string_view,
                const string&, const string&, const string&,
                const string_view);
  int delete_train(const string_view);
  int release_train(const string_view);
  string query_train(const string_view, const string_view);
  string query_ticket(const string_view, const string_view, const string_view, const string_view);
  string query_transfer(const string_view, const string_view, const string_view, const string_view);
};
#endif