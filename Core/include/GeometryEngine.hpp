#pragma once

#include <cmath>

#include "Vector3.hpp"

constexpr double DEG_TO_RAD = 3.14159265358979323846 / 180.0;

class GeometryEngine {
 public:
  inline double GetHeading(const Vector3& unitPos, const Vector3& targetPos) {
    double dx = targetPos.x - unitPos.x;
    double dy = targetPos.y - unitPos.y;

    double heading = std::atan2(dy, dx) / DEG_TO_RAD;

    return heading;
  }

  inline Vector3 GetVelocityFromHeading(double heading_deg, double speed) {
    double rad = heading_deg * DEG_TO_RAD;

    // TODO: hardcoded for now, come back to this
    Vector3 v;
    v.x = speed * std::cos(rad);
    v.y = speed * std::sin(rad);
    v.z = 0.0;

    return v;
  }

  inline double GetDistance(const Vector3& unitPos, const Vector3& targetPos) {
    double distance =
        sqrt((unitPos.x - targetPos.x) * (unitPos.x - targetPos.x) +
             (unitPos.y - targetPos.y) * (unitPos.y - targetPos.y));
    return distance;
  }
};
