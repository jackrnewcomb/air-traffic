#pragma once

struct Vector3 {
  double x{0.0}, y{0.0}, z{0.0};

  Vector3 operator+(const Vector3& o) const {
    return {x + o.x, y + o.y, z + o.z};
  }

  Vector3 operator-(const Vector3& o) const {
    return {x - o.x, y - o.y, z - o.z};
  }

  Vector3 operator*(double s) const { return {x * s, y * s, z * s}; }

  Vector3& operator+=(const Vector3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
};

double magnitude(const Vector3& v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 normalize(const Vector3& v) {
  double mag = magnitude(v);
  if (mag == 0) return {0, 0, 0};
  return v * (1.0 / mag);
}

Vector3 clampMagnitude(const Vector3& v, double maxMag) {
  double mag = magnitude(v);
  if (mag <= maxMag) return v;
  return normalize(v) * maxMag;
}
