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
  kinematics_.position.x = cfg["X_Position"].AsNumber();
  kinematics_.position.y = cfg["Y_Position"].AsNumber();
  kinematics_.position.z = cfg["Z_Position"].AsNumber();
  kinematics_.velocity.vx = cfg["X_Velocity"].AsNumber();
  kinematics_.velocity.vy = cfg["Y_Velocity"].AsNumber();
  kinematics_.velocity.vz = cfg["Z_Velocity"].AsNumber();
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
}

void Aircraft::ProcessAircraftPositionRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const AircraftStatusRequestMessage*>(&msg);
  AircraftStatusResponseMessage response(name_, request->sender);
  response.kinematics = kinematics_;
  messagebus_.get().Publish(response);
}

void Aircraft::ProcessAircraftCourseCorrectRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const AircraftCourseCorrectRequestMessage*>(&msg);
  if (request->receiver == name_) {
    kinematics_ = request->requested_kinematics;
  }
}

void Aircraft::Update() {
  // Update position based on velocity
  kinematics_.position.x =
      kinematics_.velocity.vx * clock_.get().dt() + kinematics_.position.x;
  kinematics_.position.y =
      kinematics_.velocity.vy * clock_.get().dt() + kinematics_.position.y;
  kinematics_.position.z =
      kinematics_.velocity.vz * clock_.get().dt() + kinematics_.position.z;

  // Update velocity based on acceleration
  kinematics_.velocity.vx =
      kinematics_.acceleration.ax * clock_.get().dt() + kinematics_.velocity.vx;
  kinematics_.velocity.vy =
      kinematics_.acceleration.ay * clock_.get().dt() + kinematics_.velocity.vy;
  kinematics_.velocity.vz =
      kinematics_.acceleration.az * clock_.get().dt() + kinematics_.velocity.vz;
}
