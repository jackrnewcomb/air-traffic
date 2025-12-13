#pragma once

#include <string>

class Clock {
  void Update() { timestep_++; }

 private:
  int timestep_{0};
};
