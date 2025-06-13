#pragma once

#ifndef _USER_HPP_
#define _USER_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include "unique_map.hpp"
#include "utility.hpp"
#include "string_utility.hpp"
using std::string, std::cout, std::endl, std::string_view;
struct Username {
  char str[24];
  Username(const string_view username_ = "") {
    copy_string(str, username_);
  }
  bool operator < (const Username &other) const {
    return std::strcmp(str, other.str) < 0;
  }
};
struct UserData {
  char password[32], name[20], mailAddr[32];
  char priviledge, logged;
  UserData() = default;
  UserData(const string_view password_, const string_view name_, const string_view mailAddr_, 
        char priviledge_ = 10) : priviledge(priviledge_), logged(false) {
    copy_string(password, password_);
    copy_string(name, name_);
    copy_string(mailAddr, mailAddr_);
  }
};
class UserHandler {
  friend class TicketHandler;
 private:
  UniqueMap<Username, UserData> map;
 public:
  UserHandler();
  bool logged_in(const string_view&);
  int add_user(const string_view, const string_view, const string_view, const string_view, const string_view, const string_view);
  int login(const string_view, const string_view);
  int logout(const string_view);
  string query_profile(const string_view, const string_view);
  string modify_profile(const string_view, const string_view, const string_view, const string_view, const string_view, const string_view);
};
#endif