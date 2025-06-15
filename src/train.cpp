#include "train.hpp"
#include <algorithm>
#include <functional>

using std::sort;

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
  auto ref = map[trainID];
  if (ref->released) return -1;
  return map.erase(trainID) ? 0 : -1;
}
int TrainHandler::release_train(const string_view trainID) {
  auto ref = map[trainID];
  if (ref.empty() || ref->released == 1) return -1;
  for (int i = 0; ref->station[i].name[0]; i++) {
    tree.insert(NameAndTrain(ref->station[i].name, ref.file_pos()));
  }
  ref->released = 1;
  return 0;
}
string TrainHandler::query_train(const string_view trainID, const string_view date) {
  auto ref = map[trainID];
  int dateInt = datify(date);
  if (ref.empty() || dateInt <= 0 || ref->seat[dateInt][ref->stationNumInt - 1] <= 0) return "-1";
  std::ostringstream ans;
  ans << ref->trainID << ' ' << ref->type;
  for (int i = 0; i < ref->stationNumInt; i++) {
    auto arriving = ref->station[i].arriving, leaving = ref->station[i].leaving;
    if (arriving.date != -1) arriving.date += dateInt;
    if (leaving.date != -1) leaving.date += dateInt;
    ans << '\n' << ref->station[i].name << ' ' << arriving << " -> " << leaving << ' '
      << ref->station[i].price << ' ';
    if (i == ref->stationNumInt - 1) {
      ans << 'x';
    } else {
      ans << ref->seat[dateInt][i];
    }
  } 
  return ans.str();
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
        int x = dateInt, from_station = 0, to_station = 0;
        while (from_station < ref->stationNumInt && 
            from != ref->station[from_station].name) from_station++;
        while (to_station < ref->stationNumInt &&
            to != ref->station[to_station].name) to_station++;
        if (from_station == ref->stationNumInt || to_station == ref->stationNumInt) throw RetType();
        x -= ref->station[from_station].leaving.date;
        if (from_station < to_station && x >= 0 && ref->seat[x][ref->stationNumInt - 1] > 0) {
          ans.push_back({});
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
        }
        break;
      }
    }
  }
  std::function cost = [&] (const RetType& lhs, const RetType& rhs) {
    return lhs.price < rhs.price || 
      (lhs.price == rhs.price && std::strcmp(lhs.trainID, rhs.trainID) < 0);
  };
  std::function time = [&] (const RetType& lhs, const RetType& rhs) {
    return lhs.arriving - lhs.leaving < rhs.arriving - rhs.leaving || 
      (lhs.arriving - lhs.leaving == rhs.arriving - rhs.leaving && 
          std::strcmp(lhs.trainID, rhs.trainID) < 0);
  };
  sort(ans.begin(), ans.end(), preference[0] == 'c' ? cost: time);
  ret << ans.size();
  for (int i = 0; i < ans.size(); i++) {
    ret << '\n' << ans[i].trainID << ' ' << from << ' ' << ans[i].leaving << " -> " 
        << to << ' ' << ans[i].arriving << ' ' << ans[i].price << ' ' << ans[i].seat;
  }
  return ret.str();
}
string TrainHandler::query_transfer(const string_view from, const string_view to, 
    const string_view date, const string_view preference) {
  struct RetType {
    char trainID[2][32];
    Time leaving[2], arriving[2];
    int price[2], seat[2];
    char exchange[32];
  } ans;
  ans.price[0] = ans.price[1] = 1e8;
  ans.leaving[0] = {0, 0};
  ans.arriving[1] = {1000, 0};
  std::function cost = [&] (const RetType& lhs, const RetType& rhs) {
    return lhs.price[0] + lhs.price[1] < rhs.price[0] + rhs.price[1] ||
      (lhs.price[0] + lhs.price[1] == rhs.price[0] + rhs.price[1] && 
        lhs.arriving[1] - lhs.leaving[0] < rhs.arriving[1] - rhs.leaving[0]) ||
      (lhs.price[0] + lhs.price[1] == rhs.price[0] + rhs.price[1] && 
        lhs.arriving[1] - lhs.leaving[0] == rhs.arriving[1] - rhs.leaving[0] && 
        std::strcmp(lhs.trainID[0], rhs.trainID[0]) < 0) ||
      (lhs.price[0] + lhs.price[1] == rhs.price[0] + rhs.price[1] && 
        lhs.arriving[1] - lhs.leaving[0] == rhs.arriving[1] - rhs.leaving[0] && 
        std::strcmp(lhs.trainID[0], rhs.trainID[0]) == 0 &&
        std::strcmp(lhs.trainID[1], rhs.trainID[1]) < 0);
  };
  std::function time = [&] (const RetType& lhs, const RetType& rhs) {
    return lhs.arriving[1] - lhs.leaving[0] < rhs.arriving[1] - rhs.leaving[0] ||
      (lhs.price[0] + lhs.price[1] < rhs.price[0] + rhs.price[1] &&
        lhs.arriving[1] - lhs.leaving[0] == rhs.arriving[1] - rhs.leaving[0]) ||
      (lhs.price[0] + lhs.price[1] == rhs.price[0] + rhs.price[1] && 
        lhs.arriving[1] - lhs.leaving[0] == rhs.arriving[1] - rhs.leaving[0] && 
        std::strcmp(lhs.trainID[0], rhs.trainID[0]) < 0) ||
      (lhs.price[0] + lhs.price[1] == rhs.price[0] + rhs.price[1] && 
        lhs.arriving[1] - lhs.leaving[0] == rhs.arriving[1] - rhs.leaving[0] && 
        std::strcmp(lhs.trainID[0], rhs.trainID[0]) == 0 &&
        std::strcmp(lhs.trainID[1], rhs.trainID[1]) < 0);
  };
  std::ostringstream ret;
  auto from_trains = tree.find(NameAndTrain(from, 0), NameAndTrain(from, INT_MAX));
  int dateInt = datify(date);
  for (int i = 0; i < from_trains.size(); i++) {
    auto train1_ref = map.make_reference(from_trains[i].train);
    int x = dateInt, from_station = 0;
    while (from_station < train1_ref->stationNumInt && 
        from != train1_ref->station[from_station].name) from_station++;
    if (from_station == train1_ref->stationNumInt) throw RetType();
    x -= train1_ref->station[from_station].leaving.date;
    if (train1_ref->seat[x][train1_ref->stationNumInt - 1] <= 0) continue;
    RetType tmp;
    std::memcpy(tmp.trainID[0], train1_ref->trainID, 32);
    tmp.leaving[0] = train1_ref->station[from_station].leaving;
    tmp.leaving[0].date += x;
    tmp.price[0] = 0;
    tmp.seat[0] = INT_MAX;
    for (int exchange = from_station + 1; exchange < train1_ref->stationNumInt; exchange++) {
      tmp.price[0] += train1_ref->station[exchange].price - train1_ref->station[exchange - 1].price;
      tmp.seat[0] = std::min(tmp.seat[0], train1_ref->seat[x][exchange - 1]);
      std::strcpy(tmp.exchange, train1_ref->station[exchange].name);
      auto pre_ans = tmp;
      pre_ans.arriving[0] = train1_ref->station[exchange].arriving;
      pre_ans.arriving[0].date += x;
      auto exchange_trains = tree.find(NameAndTrain(train1_ref->station[exchange].name, 0), 
                                     NameAndTrain(train1_ref->station[exchange].name, INT_MAX));
      for (int j = 0; j < exchange_trains.size(); j++) {
        auto train2_ref = map.make_reference(exchange_trains[j].train);
        int y = pre_ans.arriving[0].date, exchange_station = 0;
        while (exchange_station < train2_ref->stationNumInt && 
            std::strcmp(train2_ref->station[exchange_station].name, pre_ans.trainID[0]))
          exchange_station++;
        y -= train2_ref->station[exchange_station].leaving.date;
        if (train2_ref->station[exchange_station].leaving.time <= pre_ans.arriving[0].time) y++;
        if (train2_ref->seat[y][train2_ref->stationNumInt - 1] <= 0) continue;
        pre_ans.seat[1] = INT_MAX;
        for (int k = exchange_station + 1; k < train2_ref->stationNumInt; k++) {
          pre_ans.seat[1] = std::min(pre_ans.seat[1], train2_ref->seat[y][k - 1]);
          if (to != train2_ref->station[k].name) continue;
          std::memcpy(pre_ans.trainID[1], train2_ref->trainID, 32);
          pre_ans.leaving[1] = train2_ref->station[exchange_station].leaving;
          pre_ans.leaving[1].date += y;
          pre_ans.arriving[1] = train2_ref->station[k].arriving;
          pre_ans.arriving[1].date += y;
          pre_ans.price[1] = train2_ref->station[k].price - train2_ref->station[exchange_station].price;
          if ((preference[0] == 'c' ? cost : time)(pre_ans, ans)) ans = pre_ans;
          break;
        }
      }
    }
  }
  if (ans.arriving[1].date == 1000) {
    ret << '0';
  } else {
    ret << ans.trainID[0] << ' ' << from << ' ' << ans.leaving[0] << " -> " 
      << ans.exchange << ' ' << ans.arriving[0] << ' ' << ans.price[0] << ' '
      << ans.seat[0] << '\n' << ans.trainID[1] << ' ' << ans.exchange << ' '
      << ans.leaving[1] << " -> " << to << ' ' << ans.arriving[1] << ' '
      << ans.price[1] << ' ' << ans.seat[1];
  }
  return ret.str();
}