#pragma once

#include <string>

#include "Clock.hpp"
#include "GeometryEngine.hpp"
#include "MessageBus.hpp"
#include "Vector3.hpp"

struct Kinematics {
  Vector3 position;
  Vector3 velocity;
  Vector3 acceleration;
  double heading{0.0};
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
  GeometryEngine geometry_engine_;
};
