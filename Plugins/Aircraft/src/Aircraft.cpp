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

  kinematics_.position = {cfg["X_Position"].AsNumber(),
                          cfg["Y_Position"].AsNumber(),
                          cfg["Z_Position"].AsNumber()};

  if (cfg["Itinerary"].Exists()) {
    for (const auto& dest : cfg["Itinerary"].AsArray()) {
      itinerary_.push_back(dest.AsString());
    }
  }
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
    if (itinerary_.empty()) {
        flight_phase_ = FlightPhase::Arrived;
        return;
    }

    // Ask navigation where to go
    NavigationRequestMessage request(name_, itinerary_.front());
    request.current_position = kinematics_.position;
    messagebus_.get().Publish(request);

    UpdateFlightPhase();
    UpdateKinematics();

    // Arrival handling
    if (flight_phase_ == FlightPhase::Arrived) {
        itinerary_.pop_front();
        if (!itinerary_.empty()) {
            flight_phase_ = FlightPhase::Enroute;
        }
    }
}

void Aircraft::UpdateFlightPhase() {
    if (distance_remaining_ >= 300) {
        flight_phase_ = FlightPhase::Enroute;
    }
    else if (distance_remaining_ >= 100) {
        flight_phase_ = FlightPhase::Approach;
    }
    else if (distance_remaining_ >= 10) {
        flight_phase_ = FlightPhase::Landing;
    }
    else {
        flight_phase_ = FlightPhase::Arrived;
    }

    flight_parameters_ = flight_phases_.at(flight_phase_);
}


void Aircraft::UpdateKinematics() {
    if (flight_phase_ == FlightPhase::Arrived) return;

    Vector3 desired_velocity =
        geometry_engine_.GetVelocityFromHeading(
            kinematics_.heading, flight_parameters_.max_speed);

    Vector3 velocity_error = desired_velocity - kinematics_.velocity;

    Vector3 accel =
        clampMagnitude(velocity_error * flight_parameters_.response_gain,
            flight_parameters_.max_accel);

    kinematics_.acceleration = accel;

    const double dt = clock_.get().dt();

    kinematics_.velocity += accel * dt;
    kinematics_.position += kinematics_.velocity * dt;
}
