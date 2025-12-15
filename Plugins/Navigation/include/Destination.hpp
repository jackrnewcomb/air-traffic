#pragma once

#include <map>

#include "Entity/Entity.hpp"
#include "Messaging/MessageTypes.hpp"
#include "Parsing/Json.hpp"

class Destination : public Entity {
 public:
  explicit Destination::Destination(const JsonValue& cfg, Clock& clock,
                                    MessageBus& bus);

  void Update() override;

 protected:
  void OnRegister() override;
  void ProcessNavigationRequestMessage(const Message& msg);
  void ProcessDestinationStatusRequestMessage(const Message& msg);
};
