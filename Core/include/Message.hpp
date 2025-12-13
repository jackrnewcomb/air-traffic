#pragma once

#include <string>

struct Message {
  Message() = delete;
  explicit Message(const std::string& sender, const std::string& receiver,
                   const std::string& type)
      : sender_(sender), receiver_(receiver), type_(type) {}

  std::string type_;
  std::string sender_;
  std::string receiver_;
};
