#pragma once

#include <string>

#include "Clock.hpp"
#include "MessageBus.hpp"

struct Position {
  double x = 0, y = 0, z = 0;

  Position operator-() const { return {-x, -y, -z}; }

  friend Position operator-(const Position& lhs, const Position& rhs) {
    Position result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
  }
};

struct Velocity {
  Velocity operator-() const { return {-vx, -vy, -vz}; }
  double vx = 0, vy = 0, vz = 0;
};

struct Acceleration {
  Acceleration operator-() const { return {-ax, -ay, -az}; }
  double ax = 0, ay = 0, az = 0;
};
struct Kinematics {
  Position position;
  Velocity velocity;
  Acceleration acceleration;

  Kinematics operator-() const { return {-position, -velocity, -acceleration}; }
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
