#include "Aircraft.hpp"

#include "Entity/EntityRegistration.hpp"

void registerAircraft(EntityRegistry& registry, Clock& clock, MessageBus& bus) {
  registry.registerType("Aircraft", [&clock, &bus](const JsonValue& cfg) {
    return std::make_unique<Aircraft>(cfg, clock, bus);
  });
}

Aircraft::Aircraft(const JsonValue& cfg, Clock& clock, MessageBus& bus)
    : Entity(clock, bus) {
  name_ = cfg["Name"].AsString();
  kinematics_.position.x = cfg["X_Position"].AsNumber();
  kinematics_.position.y = cfg["Y_Position"].AsNumber();
  kinematics_.position.z = cfg["Z_Position"].AsNumber();
  kinematics_.velocity.x = cfg["X_Velocity"].AsNumber();
  kinematics_.velocity.y = cfg["Y_Velocity"].AsNumber();
  kinematics_.velocity.z = cfg["Z_Velocity"].AsNumber();

  destination_ = cfg["Destination"].AsString();
}

void Aircraft::OnRegister() {
  messagebus_.get().Subscribe(
      "AircraftStatusRequestMessage",
      std::bind(&Aircraft::ProcessAircraftPositionRequestMessage, this,
                std::placeholders::_1));

  messagebus_.get().Subscribe(
      "AircraftCourseCorrectRequestMessage",
      std::bind(&Aircraft::ProcessAircraftCourseCorrectRequestMessage, this,
                std::placeholders::_1));

  messagebus_.get().Subscribe(
      "NavigationResponseMessage",
      std::bind(&Aircraft::ProcessNavigationResponseMessage, this,
                std::placeholders::_1));
}

void Aircraft::ProcessAircraftPositionRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const AircraftStatusRequestMessage*>(&msg);
  AircraftStatusResponseMessage response(name_, request->sender);
  response.position = kinematics_.position;
  response.heading = kinematics_.heading;
  messagebus_.get().Publish(response);
}

void Aircraft::ProcessAircraftCourseCorrectRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const AircraftCourseCorrectRequestMessage*>(&msg);
  if (request->receiver == name_) {
    kinematics_.heading = request->heading;
  }
}

void Aircraft::ProcessNavigationResponseMessage(const Message& msg) {
  auto response = dynamic_cast<const NavigationResponseMessage*>(&msg);
  if (response->receiver == name_) {
    kinematics_.heading = response->heading;
    distance_remaining_ = response->remaining_distance;
  }
}

void Aircraft::Update() {
  // Ask the destination where we need to go
  if (flight_phase_ != FlightPhase::Arrived) {
    NavigationRequestMessage request(name_, destination_);
    request.current_position = kinematics_.position;
    messagebus_.get().Publish(request);
  }

  // Update flight parameters based on status
  if (distance_remaining_ >= 300) {
    flight_phase_ = FlightPhase::Enroute;
  } else if (distance_remaining_ >= 100 && distance_remaining_ < 300) {
    flight_phase_ = FlightPhase::Approach;
  } else if (distance_remaining_ >= 10 && distance_remaining_ < 100) {
    flight_phase_ = FlightPhase::Landing;
  } else if (distance_remaining_ < 10) {
    flight_phase_ = FlightPhase::Arrived;
  }
  flight_parameters_ = flight_phases_[flight_phase_];

  if (flight_phase_ != FlightPhase::Arrived) {
    // Update acceleration based on heading
    Vector3 desired_velocity = geometry_engine_.GetVelocityFromHeading(
        kinematics_.heading, flight_parameters_.max_speed);
    Vector3 velocity_error = desired_velocity - kinematics_.velocity;

    Vector3 acceleration_command =
        velocity_error * flight_parameters_.response_gain;
    acceleration_command =
        clampMagnitude(acceleration_command, flight_parameters_.max_accel);

    kinematics_.acceleration = acceleration_command;

    // Update velocity based on acceleration
    kinematics_.velocity.x =
        kinematics_.acceleration.x * clock_.get().dt() + kinematics_.velocity.x;
    kinematics_.velocity.y =
        kinematics_.acceleration.y * clock_.get().dt() + kinematics_.velocity.y;
    kinematics_.velocity.z =
        kinematics_.acceleration.z * clock_.get().dt() + kinematics_.velocity.z;

    // Update position based on velocity
    kinematics_.position.x =
        kinematics_.velocity.x * clock_.get().dt() + kinematics_.position.x;
    kinematics_.position.y =
        kinematics_.velocity.y * clock_.get().dt() + kinematics_.position.y;
    kinematics_.position.z =
        kinematics_.velocity.z * clock_.get().dt() + kinematics_.position.z;
  }
}
