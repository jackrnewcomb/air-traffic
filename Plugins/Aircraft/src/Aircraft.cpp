#include "Aircraft.hpp"

#include "EntityRegistration.hpp"

void registerAircraft(EntityRegistry& registry, Clock& clock, MessageBus& bus) {
  registry.registerType("Aircraft", [&clock, &bus](const JsonValue& cfg) {
    return std::make_unique<Aircraft>(cfg, clock, bus);
  });
}

Aircraft::Aircraft(const JsonValue& cfg, Clock& clock, MessageBus& bus)
    : Entity(clock, bus) {
  name_ = cfg["Name"].AsString();
  kinematics_.x = cfg["X_Position"].AsNumber();
  kinematics_.y = cfg["Y_Position"].AsNumber();
  kinematics_.z = cfg["Z_Position"].AsNumber();
  kinematics_.vx = cfg["X_Velocity"].AsNumber();
  kinematics_.vy = cfg["Y_Velocity"].AsNumber();
  kinematics_.vz = cfg["Z_Velocity"].AsNumber();
}

void Aircraft::OnRegister() {
  messagebus_.get().Subscribe(
      "AircraftPositionRequestMessage",
      std::bind(&Aircraft::ProcessAircraftPositionRequestMessage, this,
                std::placeholders::_1));
}

void Aircraft::ProcessAircraftPositionRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const AircraftPositionRequestMessage*>(&msg);
  AircraftPositionResponseMessage response(name_, request->sender);
  response.x = kinematics_.x;
  response.y = kinematics_.y;
  response.z = kinematics_.z;
  messagebus_.get().Publish(response);
}

void Aircraft::Update() {
  // Update position based on velocity
  kinematics_.x = kinematics_.vx * clock_.get().dt() + kinematics_.x;
  kinematics_.y = kinematics_.vy * clock_.get().dt() + kinematics_.y;
  kinematics_.z = kinematics_.vz * clock_.get().dt() + kinematics_.z;

  // Update velocity based on acceleration
  kinematics_.vx = kinematics_.ax * clock_.get().dt() + kinematics_.vx;
  kinematics_.vy = kinematics_.ay * clock_.get().dt() + kinematics_.vy;
  kinematics_.vz = kinematics_.az * clock_.get().dt() + kinematics_.vz;
}
