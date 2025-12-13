#include <iostream>

#include "Aircraft.hpp"
#include "EntityManager.hpp"
#include "EntityRegistration.hpp"
#include "JsonParser.hpp"

void registerAircraft(EntityRegistry& registry);

int main() {
  std::string json = R"({
        "Name": "Voyager",
        "Type": "Aircraft"
    })";

  EntityRegistry registry;
  registerAircraft(registry);

  JsonParser parser(json);
  JsonValue root = parser.Parse();

  auto type = root["Type"].AsString();

  auto entity = registry.create(type, root);

  EntityManager entity_manager;
  entity_manager.Add(std::move(entity));

  int sim_duration = 100;
  for (int i = 0; i < sim_duration; i++) {
  }
}
