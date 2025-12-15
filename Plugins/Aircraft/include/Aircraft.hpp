#pragma once

#include <deque>

#include "Entity/Entity.hpp"
#include "Messaging/MessageTypes.hpp"
#include "Parsing/Json.hpp"

enum class FlightPhase { Enroute, Approach, Landing, Arrived };

struct FlightParameters {
  double max_speed;
  double max_accel;
  double response_gain;
};

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
  void UpdateFlightPhase();
  void UpdateKinematics();

  std::unordered_map<FlightPhase, FlightParameters> flight_phases_{
      {FlightPhase::Enroute, {30.0, 10.0, 2}},
      {FlightPhase::Approach, {20.0, 5.0, 2}},
      {FlightPhase::Landing, {5.0, 2.0, 2}},
      {FlightPhase::Arrived, {0.0, 0.0, 2}}};

  FlightPhase flight_phase_;
  FlightParameters flight_parameters_;
  double distance_remaining_{0.0};
  std::deque<std::string> itinerary_;
};
