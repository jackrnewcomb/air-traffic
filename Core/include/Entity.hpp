#pragma once

#include <string>

#include "Clock.hpp"
#include "MessageBus.hpp"

class Entity {
 protected:
  std::string name_;
  std::shared_ptr<MessageBus> messagebus_;
  std::shared_ptr<Clock> clock_;
};
