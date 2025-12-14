#pragma once

#include "Entity/Entity.hpp"
#include "Messaging/Message.hpp"

struct AircraftStatusRequestMessage : public Message {
  AircraftStatusRequestMessage(const std::string& sender,
                               const std::string& receiver)
      : Message(sender, receiver, "AircraftStatusRequestMessage") {}
};

struct AircraftStatusResponseMessage : public Message {
  AircraftStatusResponseMessage(const std::string& sender,
                                const std::string& receiver)
      : Message(sender, receiver, "AircraftStatusResponseMessage") {}
  Vector3 position;
  double heading{0.0};
};

struct AircraftCourseCorrectRequestMessage : public Message {
  AircraftCourseCorrectRequestMessage(const std::string& sender,
                                      const std::string& receiver)
      : Message(sender, receiver, "AircraftCourseCorrectRequestMessage") {}
  double heading{0.0};
};

struct AircraftCourseCorrectResponseMessage : public Message {
  AircraftCourseCorrectResponseMessage(const std::string& sender,
                                       const std::string& receiver)
      : Message(sender, receiver, "AircraftCourseCorrectResponseMessage") {}
  bool Accepted{false};
};

struct NavigationRequestMessage : public Message {
  NavigationRequestMessage(const std::string& sender,
                           const std::string& receiver)
      : Message(sender, receiver, "NavigationRequestMessage") {}
  Vector3 current_position;
};

struct NavigationResponseMessage : public Message {
  NavigationResponseMessage(const std::string& sender,
                            const std::string& receiver)
      : Message(sender, receiver, "NavigationResponseMessage") {}
  double heading{0.0};
  double remaining_distance{0.0};
};
