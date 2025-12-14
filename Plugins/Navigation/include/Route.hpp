#pragma once

#include <map>

#include "Entity/Entity.hpp"
#include "Messaging/MessageTypes.hpp"
#include "Parsing/Json.hpp"

class ATC : public Entity {
 public:
  explicit ATC::ATC(const JsonValue& cfg, Clock& clock, MessageBus& bus);

  void Update() override;

 protected:
  void OnRegister() override;
  void ProcessAircraftPositionResponseMessage(const Message& msg);
  std::unordered_map<std::string, Kinematics> subordinate_aircraft_;
  void IssueCourseCorrect(std::pair<std::string, Kinematics> aircraft1,
                          std::pair<std::string, Kinematics> aircraft2);
};
