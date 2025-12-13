#pragma once

#include <string>

#include "Clock.hpp"
#include "MessageBus.hpp"

class Entity {
 public:
  void Register(std::shared_ptr<MessageBus> messagebus) {
    messagebus_ = messagebus;
  }
  virtual void Update() { clock_.Update(); }

 protected:
  std::string name_;
  std::shared_ptr<MessageBus> messagebus_;
  Clock clock_;
};
