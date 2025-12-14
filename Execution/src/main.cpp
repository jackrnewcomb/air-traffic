#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "ATC.hpp"
#include "Aircraft.hpp"
#include "Destination.hpp"
#include "Entity/EntityManager.hpp"
#include "Entity/EntityRegistration.hpp"
#include "Messaging/MessageTypes.hpp"
#include "Parsing/JsonParser.hpp"
#include "SFML/Graphics.hpp"
#include "Visuals/Visuals.hpp"

void registerAircraft(EntityRegistry& registry, Clock& clock, MessageBus& bus);
void registerATC(EntityRegistry& registry, Clock& clock, MessageBus& bus);
void registerDestination(EntityRegistry& registry, Clock& clock,
                         MessageBus& bus);

std::string readFile(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path);
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

int main() {
  auto json = readFile("../Tests/itinerary_test.json");

  Clock clock(0.0, 0.001);
  MessageBus message_bus;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");
  VisualsSystem visuals(window, message_bus);

  EntityRegistry registry;
  registerAircraft(registry, clock, message_bus);
  registerATC(registry, clock, message_bus);
  registerDestination(registry, clock, message_bus);

  JsonParser parser(json);
  JsonValue root = parser.Parse();

  EntityManager entity_manager(clock, message_bus);

  for (const auto& entityCfg : root.AsArray()) {
    auto type = entityCfg["Type"].AsString();
    auto entity = registry.create(type, entityCfg);
    entity_manager.Add(std::move(entity));
  }

  const float simStep = 1.0f / 60.0f;
  float accumulator = 0.0f;

  clock.Reset();

  while (window.isOpen()) {
    float frameTime = clock.RealDelta();
    accumulator += frameTime;

    while (accumulator >= simStep) {
      clock.set_dt(simStep);
      entity_manager.UpdateAll();
      accumulator -= simStep;
    }

    visuals.Render(accumulator / simStep);
  }

  return 0;
}
