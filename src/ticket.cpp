#include "ticket.hpp"
#include "user.hpp"
#include "train.hpp"

extern UserHandler user_handler;
extern TrainHandler train_handler;

TicketHandler::TicketHandler() : order("order_map"), userorder("user_order_bbl"), 
    queueing("queue_bbl") {}
string TicketHandler::buy_ticket(const string_view username, const string_view trainID,
    const string_view date, const string_view num, const string_view from,
    const string_view to, const string_view queue, const string_view stamp) {
  struct BuyTicketError {};
  auto user_ref = user_handler.map[username];
  if (user_ref.empty() || !user_ref->logged) return "-1";
  auto train_ref = train_handler.map[trainID];
  if (train_ref.empty() || !train_ref->released) return "-1";
  int dateInt = datify(date), numInt;
  std::from_chars(num.begin(), num.end(), numInt);
  int x = dateInt, from_station = 0, to_station = 0;
  while (from_station < train_ref->stationNumInt && 
      from != train_ref->station[from_station].name) from_station++;
  while (to_station < train_ref->stationNumInt &&
      to != train_ref->station[to_station].name) to_station++;
  if (from_station == train_ref->stationNumInt || to_station == train_ref->stationNumInt) 
    return "-1";
  x -= train_ref->station[from_station].leaving.date;
  if (from_station < to_station && x >= 0 && x < 100 &&
      train_ref->seat[x][train_ref->stationNumInt - 1] > 0) {
    int min_seat = numInt;
    for (int k = from_station; k < to_station; k++) {
      min_seat = std::min(min_seat, train_ref->seat[x][k]);
    }
    if (min_seat >= numInt) {
      for (int k = from_station; k < to_station; k++) {
        train_ref->seat[x][k] -= numInt;
      }
      int stampInt = 0;
      std::from_chars(stamp.begin() + 1, stamp.end() - 1, stampInt);
      userorder.insert({user_ref.file_pos(), stampInt});
      order.insert(stampInt, Order(train_ref.file_pos(), from_station, to_station, numInt, x));
      return std::to_string(min_seat * 
          (train_ref->station[to_station].price - train_ref->station[from_station].price));
    } else if (queue[0] == 't') { // "true"
      int stampInt = 0;
      std::from_chars(stamp.begin() + 1, stamp.end() - 1, stampInt);
      userorder.insert({user_ref.file_pos(), stampInt});
      order.insert(stampInt, Order(train_ref.file_pos(), from_station, to_station, numInt, x,
          Order::Status::PENDING));
      queueing.insert({{train_ref.file_pos(), x}, stampInt});
      return "queue";
    } else {
      return "-1";
    }
  } else {
    return "-1";
  }
}
string TicketHandler::query_order(const string_view username) {
  auto user_ref = user_handler.map[username];
  if (user_ref.empty() || !user_ref->logged) return "-1";
  std::stringstream ret;
  auto results = userorder.find({user_ref.file_pos(), 0}, {user_ref.file_pos(), INT_MAX});
  ret << results.size();
  for (int i = static_cast<int>(results.size()) - 1; i >= 0; i--) {
    auto order_ref = order[results[i].second];
    auto train_ref = train_handler.map.make_reference(order_ref->train_pos);
    ret << '\n';
    switch (order_ref->status) {
      case Order::Status::SUCCESS: {
        ret << "[success] ";
        break;
      }
      case Order::Status::PENDING: {
        ret << "[pending] ";
        break;
      }
      case Order::Status::REFUNDED: {
        ret << "[refunded] ";
        break;
      }
    }
    auto leaving = train_ref->station[order_ref->start].leaving;
    auto arriving = train_ref->station[order_ref->end].arriving;
    leaving.date += order_ref->date;
    arriving.date += order_ref->date;
    ret << train_ref->trainID << ' ' << train_ref->station[order_ref->start].name << ' '
      << leaving << " -> " << train_ref->station[order_ref->end].name << ' ' << arriving << ' ' 
      << (train_ref->station[order_ref->end].price - train_ref->station[order_ref->start].price)
      << ' ' << order_ref->num;
  }
  return ret.str();
}
int TicketHandler::refund_ticket(const string_view username, const string_view num) {
  auto user_ref = user_handler.map[username];
  if (user_ref.empty() || !user_ref->logged) return -1;
  int numInt;
  std::from_chars(num.begin(), num.end(), numInt);
  auto results = userorder.find({user_ref.file_pos(), 0}, {user_ref.file_pos(), INT_MAX});
  if (numInt <= 0 || numInt > results.size()) return -1;
  auto refundStamp = results[results.size() - numInt].second;
  auto refund_ref = order[refundStamp];
  switch (refund_ref->status) {
    case Order::Status::PENDING : {
      refund_ref->status = Order::Status::REFUNDED;
      queueing.erase({{refund_ref->train_pos, refund_ref->date}, refundStamp});
      break;
    }
    case Order::Status::SUCCESS : {
      refund_ref->status = Order::Status::REFUNDED;
      auto train_ref = train_handler.map.make_reference(refund_ref->train_pos);
      for (int i = refund_ref->start; i < refund_ref->end; i++) {
        train_ref->seat[refund_ref->date][i] += refund_ref->num;
      }
      auto queue = queueing.find({{refund_ref->train_pos, refund_ref->date}, 0}, 
                                 {{refund_ref->train_pos, refund_ref->date}, INT_MAX});
      for (int i = 0; i < queue.size(); i++) {
        auto try_ref = order.make_reference(queue[i].second);
        bool ok = true;
        for (int j = try_ref->start; j < refund_ref->end; j++) {
          if (train_ref->seat[refund_ref->date][j] < try_ref->num) {
            ok = false;
            break;
          }
        }
        if (!ok) continue;
        try_ref->status = Order::Status::SUCCESS;
        for (int j = try_ref->start; j < refund_ref->end; j++) {
          train_ref->seat[refund_ref->date][j] -= try_ref->num;
        }
        queueing.erase(queue[i]);
      }
      break;
    }
    case Order::Status::REFUNDED : {
      return -1;
      break;
    }
  }
  return 0;
}