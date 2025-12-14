#pragma once

#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "Messaging/Message.hpp"

class MessageBus {
 public:
  void Publish(Message& msg);
  void Subscribe(const std::string& type,
                 std::function<void(const Message& msg)> callback);

 private:
  std::unordered_map<std::string,
                     std::vector<std::function<void(const Message&)>>>
      subscriptions_;
};
