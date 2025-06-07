#include "train.hpp"

TrainHandler::TrainHandler() : map("train_map"), tree("train_tree") {}

int TrainHandler::add_train(const string_view trainID, const string_view stationNum,
    const string_view seatNum, const string& stations, const string& prices, 
    const string_view startTime, const string& travelTimes, 
    const string& stopoverTimes, const string& saleDate, const string_view type) {
  if (!map[trainID].empty()) return -1;
  map.insert(trainID, TrainData(stationNum, seatNum, stations, prices, startTime, travelTimes, 
    stopoverTimes, saleDate, type));
  return 0;
}
int TrainHandler::delete_train(const string_view) {
  return -1;
}
int TrainHandler::release_train(const string_view) {
  return -1;
}
string TrainHandler::query_train(const string_view, const string_view) {
  return "-1";
}
string TrainHandler::query_ticket(const string_view, const string_view, 
    const string_view, const string_view) {
  return "-1";
}
string TrainHandler::query_transfer(const string_view, const string_view, 
    const string_view, const string_view) {
  return "-1";
}