#pragma once

#ifndef _USER_HPP_
#define _USER_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <string_view>
#include "unique_map.hpp"
using std::string, std::cout, std::endl, std::string_view;
struct Username {
  char str[24];
  Username(const string_view username_ = "") {
    memcpy(str, username_.data(), username_.size());
    str[username_.size()] = 0;
  }
  bool operator < (const Username &other) const {
    return std::strcmp(str, other.str) < 0;
  }
};
struct UserData {
  char password[32], name[20], mailAddr[32];
  char priviledge, logged;
  UserData(const string_view password_, const string_view name_, const string_view mailAddr_, 
        char priviledge_ = 10) : priviledge(priviledge_), logged(false) {
    memcpy(password, password_.data(), password_.size());
    password[password_.size()] = 0;
    memcpy(name, name_.data(), name_.size());
    name[name_.size()] = 0;
    memcpy(mailAddr, mailAddr_.data(), mailAddr_.size());
    mailAddr[mailAddr_.size()] = 0;
  }
};
class UserHandler {
 private:
  UniqueMap<Username, UserData> map;
 public:
  UserHandler();
  int add_user(const string_view, const string_view, const string_view, const string_view, const string_view, const string_view);
  int login(const string_view, const string_view);
  int logout(const string_view);
  string query_profile(const string_view, const string_view);
  string modify_profile(const string_view, const string_view, const string_view, const string_view, const string_view, const string_view);
};
#endif