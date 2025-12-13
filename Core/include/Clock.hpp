#pragma once

#include <string>

class Clock {
 public:
  void Update() {
    timestep_ += dt;
    current_time_ = current_time_ + timestep_;
  }
  double current_time() { return current_time_; }
  int timestep() { return timestep_; }

 private:
  double start_time_{0.0};  // s
  int timestep_{0};
  int dt{1};
  double current_time_{0.0};  // s
};
