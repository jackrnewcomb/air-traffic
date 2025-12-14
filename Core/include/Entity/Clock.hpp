#pragma once
#include <chrono>
#include <string>

class Clock {
 public:
  explicit Clock(double start_time, double dt)
      : start_time_(start_time), dt_(dt) {}

  void Reset() { last_ = std::chrono::steady_clock::now(); }

  float RealDelta() {
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - last_).count();
    last_ = now;
    return std::min(dt, 0.1f);  // clamp
  }

  void Update() {
    timestep_++;
    current_time_ = current_time_ + dt_;
  }

  double current_time() { return current_time_; }
  void set_dt(double dt) { dt_ = dt; }
  double dt() { return dt_; }

 private:
  std::chrono::steady_clock::time_point last_;
  double start_time_{0.0};  // s
  int timestep_{0};
  double dt_{0.001};          // s
  double current_time_{0.0};  // s
};
