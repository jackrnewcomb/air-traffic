#pragma once

#include "Entity.hpp"
#include "Json.hpp"

class Aircraft : public Entity {
 public:
  explicit Aircraft(const JsonValue& cfg);

  void Update() override;
};
