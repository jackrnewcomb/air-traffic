#pragma once

#include "Entity.hpp"
#include "Json.hpp"
#include "MessageTypes.hpp"

class Aircraft : public Entity {
 public:
  explicit Aircraft::Aircraft(const JsonValue& cfg, Clock& clock,
                              MessageBus& bus);

  void Update() override;

 protected:
  void OnRegister() override;
  void ProcessAircraftPositionRequestMessage(const Message& msg);
  void ProcessAircraftCourseCorrectRequestMessage(const Message& msg);
  void ProcessNavigationResponseMessage(const Message& msg);

  std::string destination_;
};
