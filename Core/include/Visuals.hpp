#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

#include "MessageBus.hpp"
#include "MessageTypes.hpp"

struct Position2D {
  float x, y;
};

class VisualsSystem {
 public:
  VisualsSystem(sf::RenderWindow& window, MessageBus& bus)
      : window_(window), messagebus_(bus) {
    // Subscribe to AircraftState messages
    messagebus_.Subscribe(
        "AircraftStatusResponseMessage",
        std::bind(&VisualsSystem::ProcessAircraftPositionResponseMessage, this,
                  std::placeholders::_1));
  }

  void ProcessAircraftPositionResponseMessage(const Message& msg) {
    auto response = dynamic_cast<const AircraftStatusResponseMessage*>(&msg);
    positions_[response->sender] = {
        static_cast<float>(response->kinematics.position.x),
        static_cast<float>(response->kinematics.position.y)};
  }

  void Update() {
    window_.clear(sf::Color::Black);

    for (const auto& [name, pos] : positions_) {
      sf::CircleShape circle(5.f);
      circle.setPosition(pos.x, pos.y);
      circle.setFillColor(sf::Color::Green);
      window_.draw(circle);
    }

    window_.display();
  }

 private:
  sf::RenderWindow& window_;
  MessageBus& messagebus_;
  std::unordered_map<std::string, Position2D> positions_;
};
