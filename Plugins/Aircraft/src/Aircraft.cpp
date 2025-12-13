#include "Aircraft.hpp"

#include "EntityRegistration.hpp"

void registerAircraft(EntityRegistry& registry) {
  registry.registerType("Aircraft", [](const JsonValue& cfg) {
    return std::make_unique<Aircraft>(cfg);
  });
}

Aircraft::Aircraft(const JsonValue& cfg) {}

void Aircraft::Update() {
  // basic placeholder logic
}
