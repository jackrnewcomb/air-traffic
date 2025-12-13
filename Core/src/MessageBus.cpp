#include "MessageBus.hpp"

void MessageBus::Publish(Message& msg) {
  if (subscriptions_.find(msg.type) != subscriptions_.end()) {
    for (auto& subscriber : subscriptions_[msg.type]) {
      subscriber(msg);
    }
  }
}
void MessageBus::Subscribe(const std::string& type,
                           std::function<void(const Message& msg)> callback) {
  subscriptions_[type].push_back(callback);
}
