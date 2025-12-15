#include "Destination.hpp"

#include "Entity/EntityRegistration.hpp"

void registerDestination(EntityRegistry& registry, Clock& clock,
                         MessageBus& bus) {
  registry.registerType("Destination", [&clock, &bus](const JsonValue& cfg) {
    return std::make_unique<Destination>(cfg, clock, bus);
  });
}

Destination::Destination(const JsonValue& cfg, Clock& clock, MessageBus& bus)
    : Entity(clock, bus) {
  name_ = cfg["Name"].AsString();
  auto location = cfg["Location"];
  kinematics_.position.x = location["X"].AsNumber();
  kinematics_.position.y = location["Y"].AsNumber();
  kinematics_.position.z = location["Z"].AsNumber();
}

void Destination::OnRegister() {
  messagebus_.get().Subscribe(
      "NavigationRequestMessage",
      std::bind(&Destination::ProcessNavigationRequestMessage, this,
                std::placeholders::_1));

  messagebus_.get().Subscribe(
      "DestinationStatusRequestMessage",
      std::bind(&Destination::ProcessDestinationStatusRequestMessage, this,
                std::placeholders::_1));
}

void Destination::ProcessNavigationRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const NavigationRequestMessage*>(&msg);

  if (request->receiver == name_) {
    NavigationResponseMessage response(name_, request->sender);
    response.heading = geometry_engine_.GetHeading(request->current_position,
                                                   kinematics_.position);
    response.remaining_distance = geometry_engine_.GetDistance(
        request->current_position, kinematics_.position);
    messagebus_.get().Publish(response);
  }
}

void Destination::ProcessDestinationStatusRequestMessage(const Message& msg) {
  auto request = dynamic_cast<const DestinationStatusRequestMessage*>(&msg);

  DestinationStatusResponseMessage response(name_, request->sender);
  response.position = kinematics_.position;
  messagebus_.get().Publish(response);
}

void Destination::Update() {}
