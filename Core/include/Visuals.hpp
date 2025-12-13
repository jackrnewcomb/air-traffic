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
        "AircraftPositionResponseMessage",
        std::bind(&VisualsSystem::ProcessAircraftPositionResponseMessage, this,
                  std::placeholders::_1));
  }

  void ProcessAircraftPositionResponseMessage(const Message& msg) {
    auto response = dynamic_cast<const AircraftPositionResponseMessage*>(&msg);
    positions_[response->sender] = {static_cast<float>(response->x),
                                    static_cast<float>(response->y)};
    std::cout << "Received position update from " << response->sender
              << ": X = " << response->x << ", Y = " << response->y << "\n";
  }

  void Update() {
    AircraftPositionRequestMessage request("Sim", "All");
    messagebus_.Publish(request);

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
