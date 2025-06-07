#include "ticket.hpp"

TicketHandler::TicketHandler() {}
string TicketHandler::buy_ticket(const string_view, const string_view, const string_view, 
                    const string_view, const string_view, const string_view,
                    const string_view) {
  return "-1";
}
string TicketHandler::query_order(const string_view) {
  return "-1";
}
int TicketHandler::refund_ticket(const string_view, const string_view) {
  return -1;
}