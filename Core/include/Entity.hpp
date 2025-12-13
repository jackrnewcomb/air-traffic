#pragma once

#include <string>

#include "Clock.hpp"
#include "MessageBus.hpp"

struct Kinematics {
  double x = 0, y = 0, z = 0;
  double vx = 0, vy = 0, vz = 0;
  double ax = 0, ay = 0, az = 0;
  double heading = 0, pitch = 0, roll = 0;
};

class Entity {
 public:
  Entity(Clock& c, MessageBus& b) : clock_(c), messagebus_(b) {}
  virtual ~Entity() = default;

  void Register(Clock& clock, MessageBus& bus) {
    clock_ = clock;
    messagebus_ = bus;
    OnRegister();
  }

  virtual void Update() = 0;

  std::string& name() { return name_; }

 protected:
  virtual void OnRegister() {}
  std::string name_;
  Kinematics kinematics_;
  std::reference_wrapper<Clock> clock_;
  std::reference_wrapper<MessageBus> messagebus_;
};
