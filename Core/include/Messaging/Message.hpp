#pragma once

#include <string>

struct Message {
  Message() = delete;
  virtual ~Message() = default;

  explicit Message(const std::string& sender, const std::string& receiver,
                   const std::string& type)
      : sender(sender), receiver(receiver), type(type) {}

  std::string type;
  std::string sender;
  std::string receiver;
};
