#pragma once

#include "Entity.hpp"

class EntityManager {
 public:
  EntityManager(Clock& clock, MessageBus& bus) : clock_(clock), bus_(bus) {}

  void Add(std::unique_ptr<Entity> e) {
    e->Register(clock_, bus_);
    entities_.push_back(std::move(e));
  }

  void UpdateAll() {
    for (auto& entity : entities_) {
      entity->Update();
    }
  }

 private:
  std::vector<std::unique_ptr<Entity>> entities_;
  Clock& clock_;
  MessageBus& bus_;
};
