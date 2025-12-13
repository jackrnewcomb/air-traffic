#include <iostream>

#include "Aircraft.hpp"
#include "EntityManager.hpp"
#include "EntityRegistration.hpp"
#include "JsonParser.hpp"
#include "MessageTypes.hpp"

void registerAircraft(EntityRegistry& registry, Clock& clock, MessageBus& bus);

int main() {
  std::string json = R"({
        "Name": "Voyager",
        "Type": "Aircraft",
        "X_Position": 10,
        "Y_Position": 0,
        "Z_Position": 20,
        "X_Velocity": 5,
        "Y_Velocity": 5,
        "Z_Velocity": 5
    })";

  Clock clock;
  MessageBus message_bus;

  EntityRegistry registry;
  registerAircraft(registry, clock, message_bus);

  JsonParser parser(json);
  JsonValue root = parser.Parse();

  auto type = root["Type"].AsString();

  auto entity = registry.create(type, root);

  EntityManager entity_manager(clock, message_bus);
  entity_manager.Add(std::move(entity));

  std::cout << "Starting simulation!\n";
  int sim_duration = 100;
  for (int i = 0; i < sim_duration; i++) {
    clock.Update();
    entity_manager.UpdateAll();
  }

  // testing the message bus
  AircraftPositionRequestMessage request("Sim", "Voyager");
  message_bus.Publish(request);
}
