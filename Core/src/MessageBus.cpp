#include "MessageBus.hpp"

void MessageBus::publish(Message& msg) {
  if (subscriptions.find(msg.type_) != subscriptions.end()) {
    for (auto& subscriber : subscriptions[msg.type_]) {
      subscriber(msg);
    }
  }
}
void MessageBus::subscribe(const std::string& type,
                           std::function<void(const Message& msg)> callback) {
  subscriptions[type].push_back(callback);
}
