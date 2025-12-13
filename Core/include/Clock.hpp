#pragma once

#include <string>

class Clock {
 public:
  explicit Clock(double start_time, double dt)
      : start_time_(start_time), dt_(dt) {}
  void Update() {
    timestep_++;
    current_time_ = current_time_ + dt_;
  }
  double current_time() { return current_time_; }
  double dt() { return dt_; }

 private:
  double start_time_{0.0};  // s
  int timestep_{0};
  double dt_{0.001};          // s
  double current_time_{0.0};  // s
};
