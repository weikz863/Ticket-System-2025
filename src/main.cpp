#include <sstream>
#include <iostream>
#include "blockblocklist.hpp"
#include "user.hpp"
#include "train.hpp"
#include "ticket.hpp"
using std::cin, std::cout, std::endl, std::getline;
UserHandler user_handler;
TrainHandler train_handler;
TicketHandler ticket_handler;
int main() {
  std::string stamp;
  for (std::string str; std::getline(std::cin, str); ) {
    std::stringstream ss(std::move(str));
    std::getline(ss, stamp, ' ');
    std::cout << stamp << ' ';
    std::getline(ss, str, ' ');
    if (str == "exit") {
      std::cout << "bye\n";
      break;
    } else if (str == "add_user") {
      std::string cur_username, username, password, name, mailAddr, priviledge;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'c' : {
            std::getline(ss, cur_username, ' ');
            break;
          }
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          case 'p' : {
            std::getline(ss, password, ' ');
            break;
          }
          case 'n' : {
            std::getline(ss, name, ' ');
            break;
          }
          case 'm' : {
            std::getline(ss, mailAddr, ' ');
            break;
          }
          case 'g' : {
            std::getline(ss, priviledge, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
          }
        }
      }
      cout << user_handler.add_user(cur_username, username, password, name, mailAddr, priviledge);
    } else if (str == "login") {
      std::string username, password;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          case 'p' : {
            std::getline(ss, password, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
          }
        }
      }
      cout << user_handler.login(username, password);
    } else if (str == "logout") {
      std::string username;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
          }
        }
      }
      cout << user_handler.logout(username);
    } else if (str == "query_profile") {
      std::string cur_username, username;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'c' : {
            std::getline(ss, cur_username, ' ');
            break;
          }
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
          }
        }
      }
      cout << user_handler.query_profile(cur_username, username);
    } else if (str == "modify_profile") {
      std::string cur_username, username, password, name, mailAddr, priviledge;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'c' : {
            std::getline(ss, cur_username, ' ');
            break;
          }
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          case 'p' : {
            std::getline(ss, password, ' ');
            break;
          }
          case 'n' : {
            std::getline(ss, name, ' ');
            break;
          }
          case 'm' : {
            std::getline(ss, mailAddr, ' ');
            break;
          }
          case 'g' : {
            std::getline(ss, priviledge, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
          }
        }
      }
      cout << user_handler.modify_profile(cur_username, username, password, name, mailAddr, priviledge);
    } else if (str == "add_train") {
      string trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, 
             stopoverTimes, saleDate, type;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'i' : {
            std::getline(ss, trainID, ' ');
            break;
          }
          case 'n' : {
            std::getline(ss, stationNum, ' ');
            break;
          }
          case 'm' : {
            std::getline(ss, seatNum, ' ');
            break;
          }
          case 's' : {
            std::getline(ss, stations, ' ');
            break;
          }
          case 'p' : {
            std::getline(ss, prices, ' ');
            break;
          }
          case 'x' : {
            std::getline(ss, startTime, ' ');
            break;
          }
          case 't' : {
            std::getline(ss, travelTimes, ' ');
            break;
          }
          case 'o' : {
            std::getline(ss, stopoverTimes, ' ');
            break;
          }
          case 'd' : {
            std::getline(ss, saleDate, ' ');
            break;
          }
          case 'y' : {
            std::getline(ss, type, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << train_handler.add_train(trainID, stationNum, seatNum, stations, prices,
          startTime, travelTimes, stopoverTimes, saleDate, type);
    } else if (str == "delete_train") {
      string trainID;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'i' : {
            std::getline(ss, trainID, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << train_handler.delete_train(trainID);
    } else if (str == "release_train") {
      string trainID;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'i' : {
            std::getline(ss, trainID, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << train_handler.release_train(trainID);
    } else if (str == "query_train") {
      string trainID, date;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'i' : {
            std::getline(ss, trainID, ' ');
            break;
          }
          case 'd' : {
            std::getline(ss, date, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << train_handler.query_train(trainID, date);
    } else if (str == "query_ticket") {
      string from, to, date, preference = "cost";
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 's' : {
            std::getline(ss, from, ' ');
            break;
          }
          case 't' : {
            std::getline(ss, to, ' ');
            break;
          }
          case 'd' : {
            std::getline(ss, date, ' ');
            break;
          }
          case 'p' : {
            std::getline(ss, preference, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << train_handler.query_ticket(from, to, date, preference);
    } else if (str == "query_transfer") {
      string from, to, date, preference = "cost";
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 's' : {
            std::getline(ss, from, ' ');
            break;
          }
          case 't' : {
            std::getline(ss, to, ' ');
            break;
          }
          case 'd' : {
            std::getline(ss, date, ' ');
            break;
          }
          case 'p' : {
            std::getline(ss, preference, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << train_handler.query_transfer(from, to, date, preference);
    } else if (str == "buy_ticket") {
      string username, trainID, date, number, from, to, queue = "false";
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          case 'i' : {
            std::getline(ss, trainID, ' ');
            break;
          }
          case 'f' : {
            std::getline(ss, from, ' ');
            break;
          }
          case 't' : {
            std::getline(ss, to, ' ');
            break;
          }
          case 'd' : {
            std::getline(ss, date, ' ');
            break;
          }
          case 'n' : {
            std::getline(ss, number, ' ');
            break;
          }
          case 'q' : {
            std::getline(ss, queue, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << ticket_handler.buy_ticket(username, trainID, date, number, from, to, queue, stamp);
    } else if (str == "refund_ticket") {
      string username, number = "1";
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          case 'n' : {
            std::getline(ss, number, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << ticket_handler.refund_ticket(username, number);
    } else if (str == "query_order") {
      string username;
      while (std::getline(ss, str, ' ')) {
        switch (str[1]) {
          case 'u' : {
            std::getline(ss, username, ' ');
            break;
          }
          default: {
            throw sjtu::runtime_error();
            break;
          }
        }
      }
      cout << ticket_handler.query_order(username);
    } else
      throw sjtu::runtime_error();
    std::cout << endl;
  }
}