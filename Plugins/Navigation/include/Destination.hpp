#pragma once

#include <map>

#include "Entity.hpp"
#include "Json.hpp"
#include "MessageTypes.hpp"

class Destination : public Entity {
 public:
  explicit Destination::Destination(const JsonValue& cfg, Clock& clock,
                                    MessageBus& bus);

  void Update() override;

 protected:
  void OnRegister() override;
  void ProcessNavigationRequestMessage(const Message& msg);
};
