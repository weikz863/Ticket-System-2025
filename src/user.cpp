#include <iostream>
#include <string>
#include <string_view>
#include "user.hpp"
using std::string, std::cout, std::endl, std::string_view;
UserHandler::UserHandler() : map("user_handler") {}
int UserHandler::add_user(const string_view cur_username, const string_view username, const string_view password, 
      const string_view name, const string_view mailAddr, const string_view priviledge) {
  if ()
  return 0;
}
int UserHandler::login(const string_view username, const string_view password) {  
  return 0;
}
int UserHandler::logout(const string_view username) {  
  return 0;
}
string UserHandler::query_profile(const string_view cur_username, const string_view username) {  
  return "-1";
}
string UserHandler::modify_profile(const string_view cur_username, const string_view username, const string_view password, 
      const string_view name, const string_view mailAddr, const string_view priviledge) {
  return "-1";
}