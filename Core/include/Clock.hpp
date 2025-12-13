#pragma once

#include <string>

class Clock {
public:
  void Update() { timestep_++; }

 private:
  int timestep_{0};
};
