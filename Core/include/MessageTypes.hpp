#pragma once

#include "Message.hpp"

struct AircraftPositionRequestMessage : public Message {
  AircraftPositionRequestMessage(const std::string& sender,
                                 const std::string& receiver)
      : Message(sender, receiver, "AircraftPositionRequestMessage") {}
};
