#include <iostream>

#include "Aircraft.hpp"
#include "EntityManager.hpp"
#include "EntityRegistration.hpp"
#include "JsonParser.hpp"
#include "MessageTypes.hpp"
#include "SFML/Graphics.hpp"
#include "Visuals.hpp"

void registerAircraft(EntityRegistry& registry, Clock& clock, MessageBus& bus);

int main() {
  std::string json = R"([
    {
        "Name": "Voyager",
        "Type": "Aircraft",
        "X_Position": 10,
        "Y_Position": 0,
        "Z_Position": 20,
        "X_Velocity": 5,
        "Y_Velocity": 5,
        "Z_Velocity": 5
    },
    {
        "Name": "Apollo",
        "Type": "Aircraft",
        "X_Position": 50,
        "Y_Position": 0,
        "Z_Position": 20,
        "X_Velocity": 5,
        "Y_Velocity": 5,
        "Z_Velocity": 5
    }
])";

  Clock clock(0.0, 0.001);
  MessageBus message_bus;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");
  VisualsSystem visuals(window, message_bus);

  EntityRegistry registry;
  registerAircraft(registry, clock, message_bus);

  JsonParser parser(json);
  JsonValue root = parser.Parse();

  EntityManager entity_manager(clock, message_bus);

  for (const auto& entityCfg : root.AsArray()) {
    auto type = entityCfg["Type"].AsString();
    auto entity = registry.create(type, entityCfg);
    entity_manager.Add(std::move(entity));
  }

  int sim_duration = 10000;
  for (int i = 0; i < sim_duration; i++) {
    clock.Update();
    entity_manager.UpdateAll();
    visuals.Update();
  }
}
