#pragma once

#include "Entity.hpp"

class EntityManager {
 public:
  void Add(std::unique_ptr<Entity> e) { entities_.push_back(std::move(e)); }

  void UpdateAll() {
    for (auto& entity : entities_) {
      entity->Update();
    }
  }

 private:
  std::vector<std::unique_ptr<Entity>> entities_;
};
