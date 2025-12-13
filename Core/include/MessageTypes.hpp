#pragma once

#include "Message.hpp"

struct AircraftPositionRequestMessage : public Message {
  AircraftPositionRequestMessage(const std::string& sender,
                                 const std::string& receiver)
      : Message(sender, receiver, "AircraftPositionRequestMessage") {}
};

struct AircraftPositionResponseMessage : public Message {
  AircraftPositionResponseMessage(const std::string& sender,
                                  const std::string& receiver)
      : Message(sender, receiver, "AircraftPositionResponseMessage") {}
  double x{0.0};
  double y{0.0};
  double z{0.0};
};
