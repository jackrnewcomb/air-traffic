#pragma once

#include <string>

class Message {
 public:
  Message(std::string& sender, std::string& receiver, std::string& type)
      : sender_(sender), receiver_(receiver), type_(type) {}
  Message(std::string& type) : type_(type) {}

  std::string type_;
  std::string sender_;
  std::string receiver_;
};
