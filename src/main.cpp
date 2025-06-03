#include <sstream>
#include <iostream>
#include "blockblocklist.hpp"
#include "user.hpp"
using std::cin, std::cout, std::endl, std::getline;
UserHandler user_handler;
int main() {
  for (std::string str; std::getline(std::cin, str); ) {
    std::stringstream ss(std::move(str));
    std::getline(ss, str, ' ');
    std::cout << str << ' ';
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
    } else {
      throw sjtu::runtime_error();
    }
    std::cout << '\n';
  }
}