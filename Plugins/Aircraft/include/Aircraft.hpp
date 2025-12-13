#pragma once

#include "Entity.hpp"
#include "Json.hpp"

class Aircraft : public Entity {
 public:
  explicit Aircraft::Aircraft(const JsonValue& cfg, Clock& clock,
                              MessageBus& bus);

  void Update() override;

 protected:
  void OnRegister() override;
  void ProcessAircraftPositionRequestMessage(const Message& msg);
};
