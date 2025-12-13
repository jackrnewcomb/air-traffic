#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Entity.hpp"
#include "Json.hpp"

class EntityRegistry {
 public:
  using FactoryFn = std::function<std::unique_ptr<Entity>(const JsonValue&)>;

  void registerType(const std::string& type, FactoryFn fn) {
    factories[type] = std::move(fn);
  }

  std::unique_ptr<Entity> create(const std::string& type,
                                 const JsonValue& config) const {
    auto it = factories.find(type);
    if (it == factories.end()) {
      throw std::runtime_error("Unknown entity type: " + type);
    }
    return it->second(config);
  }

 private:
  std::unordered_map<std::string, FactoryFn> factories;
};
