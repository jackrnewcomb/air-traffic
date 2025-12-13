#include "ATC.hpp"

#include "EntityRegistration.hpp"

void registerATC(EntityRegistry& registry, Clock& clock, MessageBus& bus) {
  registry.registerType("ATC", [&clock, &bus](const JsonValue& cfg) {
    return std::make_unique<ATC>(cfg, clock, bus);
  });
}

ATC::ATC(const JsonValue& cfg, Clock& clock, MessageBus& bus)
    : Entity(clock, bus) {
  name_ = cfg["Name"].AsString();
  kinematics_.position.x = cfg["X_Position"].AsNumber();
  kinematics_.position.y = cfg["Y_Position"].AsNumber();
  kinematics_.position.z = cfg["Z_Position"].AsNumber();
  kinematics_.velocity.vx = cfg["X_Velocity"].AsNumber();
  kinematics_.velocity.vy = cfg["Y_Velocity"].AsNumber();
  kinematics_.velocity.vz = cfg["Z_Velocity"].AsNumber();
}

void ATC::OnRegister() {
  messagebus_.get().Subscribe(
      "AircraftStatusResponseMessage",
      std::bind(&ATC::ProcessAircraftPositionResponseMessage, this,
                std::placeholders::_1));
}

void ATC::ProcessAircraftPositionResponseMessage(const Message& msg) {
  auto response = dynamic_cast<const AircraftStatusResponseMessage*>(&msg);
  subordinate_aircraft_[response->sender] = response->kinematics;
}

void ATC::Update() {
  // Ask for a position update from all aircraft
  AircraftStatusRequestMessage request(name_, "All");
  messagebus_.get().Publish(request);

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

  // Check if anyones gonna hit anyone else
  for (auto& aircraft : subordinate_aircraft_) {
    for (auto& other_aircraft : subordinate_aircraft_) {
      if (aircraft.first == other_aircraft.first) {
        continue;
      }

      auto relative = aircraft.second.position - other_aircraft.second.position;
      float horizontalDist =
          sqrt(relative.x * relative.x + relative.y * relative.y);

      if (horizontalDist < 100) {
        IssueCourseCorrect(aircraft, other_aircraft);
      }
    }
  }
}

void ATC::IssueCourseCorrect(std::pair<std::string, Kinematics> aircraft1,
                             std::pair<std::string, Kinematics> aircraft2) {
  AircraftCourseCorrectRequestMessage request_aircraft1(name_, aircraft1.first);
  request_aircraft1.requested_kinematics = aircraft1.second;
  request_aircraft1.requested_kinematics.velocity =
      -request_aircraft1.requested_kinematics.velocity;
  messagebus_.get().Publish(request_aircraft1);

  AircraftCourseCorrectRequestMessage request_aircraft2(name_, aircraft2.first);
  request_aircraft2.requested_kinematics = aircraft2.second;
  request_aircraft2.requested_kinematics.velocity =
      -request_aircraft2.requested_kinematics.velocity;
  messagebus_.get().Publish(request_aircraft2);
}
