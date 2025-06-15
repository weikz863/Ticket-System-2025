#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <charconv>
#include "user.hpp"
using std::string, std::cout, std::endl, std::string_view;
UserHandler::UserHandler() : map("user_handler") {}
bool UserHandler::logged_in(const string_view &username) {
  auto ref = map[username];
  return !ref.empty() && ref->logged;
}
int UserHandler::add_user(const string_view cur_username, const string_view username, const string_view password, 
      const string_view name, const string_view mailAddr, const string_view priviledge) {
  if (map.size() == 0) {
    map.insert(username, {password, name, mailAddr});
    return 0;
  }
  auto curref = map[cur_username];
  char int_priv;
  std::from_chars(priviledge.begin(), priviledge.end(), int_priv);
  if (curref.empty() || !curref->logged || curref->priviledge <= int_priv || !map[username].empty()) return -1;
  map.insert(username, {password, name, mailAddr, int_priv});
  return 0;
}
int UserHandler::login(const string_view username, const string_view password) {
  auto ref = map[username];
  if (ref.empty() || ref->logged || password != string_view(ref->password)) return -1;
  ref->logged = true;
  return 0;
}
int UserHandler::logout(const string_view username) {
  auto ref = map[username];
  if (ref.empty() || !ref->logged) return -1;
  ref->logged = false;
  return 0;
}
string UserHandler::query_profile(const string_view cur_username, const string_view username) {
  auto cur_ref = map[cur_username];
  if (cur_ref.empty() || !cur_ref->logged) return "-1";
  if (username == cur_username) {
    std::ostringstream ret;
    ret << username;
    ret << ' ';
    ret << cur_ref->name;
    ret << ' ';
    ret << cur_ref->mailAddr;
    ret << ' ';
    ret << int(cur_ref->priviledge);
    return ret.str(); 
  } else {
    auto ref = map[username];
    if (ref.empty() || cur_ref->priviledge <= ref->priviledge) return "-1";
    std::ostringstream ret;
    ret << username;
    ret << ' ';
    ret << ref->name;
    ret << ' ';
    ret << ref->mailAddr;
    ret << ' ';
    ret << int(ref->priviledge);
    return ret.str(); 
  }
}
string UserHandler::modify_profile(const string_view cur_username, const string_view username, const string_view password, 
      const string_view name, const string_view mailAddr, const string_view priviledge) {
  auto cur_ref = map[cur_username];
  if (cur_ref.empty() || !cur_ref->logged) return "-1";
  char new_priv = 16;
  if (priviledge.size()) {
    std::from_chars(priviledge.begin(), priviledge.end(), new_priv);
    if (cur_ref->priviledge <= new_priv) return "-1";
  }
  if (cur_username == username) {
    if (password.size()) copy_string(cur_ref->password, password);
    if (name.size()) copy_string(cur_ref->name, name);
    if (mailAddr.size()) copy_string(cur_ref->mailAddr, mailAddr);
    if (new_priv != 16) cur_ref->priviledge = new_priv;
    std::ostringstream ret;
    ret << username;
    ret << ' ';
    ret << cur_ref->name;
    ret << ' ';
    ret << cur_ref->mailAddr;
    ret << ' ';
    ret << int(cur_ref->priviledge);
    return ret.str(); 
  } else {
    auto ref = map[username];
    if (ref.empty() || cur_ref->priviledge <= ref->priviledge) return "-1";
    if (password.size()) copy_string(ref->password, password);
    if (name.size()) copy_string(ref->name, name);
    if (mailAddr.size()) copy_string(ref->mailAddr, mailAddr);
    if (new_priv != 16) ref->priviledge = new_priv;
    std::ostringstream ret;
    ret << username;
    ret << ' ';
    ret << ref->name;
    ret << ' ';
    ret << ref->mailAddr;
    ret << ' ';
    ret << int(ref->priviledge);
    return ret.str(); 
  }
}
const char* UserHandler::exit() {
  map.for_each([] (UserData& x) { x.logged = 0; });
  return "bye";
}