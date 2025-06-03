#pragma once

#ifndef _USER_HPP_
#define _USER_HPP_

#include <iostream>
#include <string>
#include <string_view>
using std::string, std::cout, std::endl, std::string_view;
class UserHandler {
 public:
  int add_user(const string_view, const string_view, const string_view, const string_view, const string_view, const string_view);
  int login(const string_view, const string_view);
  int logout(const string_view);
  string query_profile(const string_view, const string_view);
  string modify_profile(const string_view, const string_view, const string_view, const string_view, const string_view, const string_view);
};
#endif