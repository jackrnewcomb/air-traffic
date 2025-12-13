#pragma once

#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "Message.hpp"

class MessageBus {
 public:
  void publish(Message& msg);
  void subscribe(const std::string& type,
                 std::function<void(const Message& msg)> callback);

 private:
  std::unordered_map<std::string,
                     std::vector<std::function<void(const Message&)>>>
      subscriptions;
};
