#include "ticket.hpp"
#include "user.hpp"
#include "train.hpp"

extern UserHandler user_handler;
extern TrainHandler train_handler;

TicketHandler::TicketHandler() : order("order_bbl"), userorder("user_order_bbl") {}
string TicketHandler::buy_ticket(const string_view username, const string_view trainID,
    const string_view date, const string_view num, const string_view from,
    const string_view to, const string_view queue) {
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
  if (from_station < to_station && x >= 0 && train_ref->seat[x][train_ref->stationNumInt - 1] > 0) {
    int min_seat = numInt;
    for (int k = from_station; k < to_station; k++) {
      min_seat = std::min(min_seat, train_ref->seat[x][k]);
    }
    if (min_seat >= numInt) {
      userorder.insert({user_ref.file_pos(), {train_ref.file_pos(), 
        static_cast<int>(order.find(Order(-1), Order(1e9)).size())}});
      for (int k = from_station; k < to_station; k++) {
        train_ref->seat[x][k] -= numInt;
      }
      return std::to_string(min_seat * 
          (train_ref->station[to_station].price - train_ref->station[from_station].price));
    } else {
      throw BuyTicketError();
      return "-1"; // TODO: queue
    }
  } else {
    return "-1";
  }
  return "0";
}
string TicketHandler::query_order(const string_view) {
  return "-1";
}
int TicketHandler::refund_ticket(const string_view, const string_view) {
  return -1;
}