#pragma once

#include "Entity.hpp"
#include "Message.hpp"

struct AircraftStatusRequestMessage : public Message {
  AircraftStatusRequestMessage(const std::string& sender,
                               const std::string& receiver)
      : Message(sender, receiver, "AircraftStatusRequestMessage") {}
};

struct AircraftStatusResponseMessage : public Message {
  AircraftStatusResponseMessage(const std::string& sender,
                                const std::string& receiver)
      : Message(sender, receiver, "AircraftStatusResponseMessage") {}
  Kinematics kinematics;
};

struct AircraftCourseCorrectRequestMessage : public Message {
  AircraftCourseCorrectRequestMessage(const std::string& sender,
                                      const std::string& receiver)
      : Message(sender, receiver, "AircraftCourseCorrectRequestMessage") {}
  Kinematics requested_kinematics;
};

struct AircraftCourseCorrectResponseMessage : public Message {
  AircraftCourseCorrectResponseMessage(const std::string& sender,
                                       const std::string& receiver)
      : Message(sender, receiver, "AircraftCourseCorrectResponseMessage") {}
  bool Accepted{false};
};
