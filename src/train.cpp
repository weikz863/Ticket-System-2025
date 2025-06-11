#include "train.hpp"
#include <algorithm>

using std::stable_sort;

TrainHandler::TrainHandler() : map("train_map"), tree("train_tree") {}

int TrainHandler::add_train(const string_view trainID, const string_view stationNum,
    const string_view seatNum, const string& stations, const string& prices, 
    const string_view startTime, const string& travelTimes, 
    const string& stopoverTimes, const string& saleDate, const string_view type) {
  if (!map[trainID].empty()) return -1;
  map.insert(trainID, TrainData(trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, 
    stopoverTimes, saleDate, type));
  return 0;
}
int TrainHandler::delete_train(const string_view trainID) {
  return map.erase(trainID) ? 0 : -1;
}
int TrainHandler::release_train(const string_view trainID) {
  auto ref = map[trainID];
  if (ref.empty()) return -1;
  for (int i = 0; ref->station[i].name[0]; i++) {
    tree.insert(NameAndTrain(ref->station[i].name, ref.file_pos()));
  }
  return 0;
}
string TrainHandler::query_train(const string_view, const string_view) {
  return "-1";
}
string TrainHandler::query_ticket(const string_view from, const string_view to, 
    const string_view date, const string_view preference) {
  struct RetType {
    char trainID[24];
    Time leaving, arriving;
    int price, seat;
  };
  vector<RetType> ans;
  std::ostringstream ret;
  auto from_trains = tree.find(NameAndTrain(from, 0), NameAndTrain(from, INT_MAX));
  auto to_trains = tree.find(NameAndTrain(to, 0), NameAndTrain(to, INT_MAX));
  int dateInt = datify(date);
  if (dateInt == 99) return "0";
  for (int i = 0; i < from_trains.size(); i++) { // optimize this!
    for (int j = 0; j < to_trains.size(); j++) {
      if (from_trains[i].train == to_trains[j].train) {
        auto ref = map.make_reference(from_trains[i].train);
        ans.push_back({});
        int x = dateInt, from_station = 0, to_station = 0;
        while (from != ref->station[from_station].name) from_station++;
        x -= ref->station[from_station].leaving.date;
        while (to != ref->station[to_station].name) to_station++;
        if (from_station < to_station && x >= 0 && ref->seat[x][ref->stationNumInt - 1] == -1) {
          std::strcpy(ans.back().trainID, ref->trainID);
          ans.back().leaving = ref->station[from_station].leaving;
          ans.back().leaving.date += x;
          ans.back().arriving = ref->station[to_station].arriving;
          ans.back().arriving.date += x;
          ans.back().price = ref->station[to_station].price - ref->station[from_station].price;
          ans.back().seat = 1e8;
          for (int k = from_station; k < to_station; k++) {
            ans.back().seat = std::min(ans.back().seat, ref->seat[x][k]);
          }
        } else {
          ans.pop_back();
        }
        break;
      }
    }
  }
  ret << ans.size();
  for (int i = 0; i < ans.size(); i++) {
    ret << '\n' << ans[i].trainID << ' ' << from << ' ' << ans[i].leaving << " -> " 
        << to << ' ' << ans[i].arriving << ' ' << ans[i].price << ' ' << ans[i].seat;
  }
  return ret.str();
}
string TrainHandler::query_transfer(const string_view, const string_view, 
    const string_view, const string_view) {
  return "-1";
}