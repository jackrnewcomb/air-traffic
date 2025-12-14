#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

#include "MessageBus.hpp"
#include "MessageTypes.hpp"

struct Position2D {
  float x, y;
};

struct VisualPosition {
  Position2D previous;
  Position2D current;
  bool initialized = false;
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
    if (!response) return;

    auto& vp = positions_[response->sender];

    Position2D newPos{static_cast<float>(response->position.x),
                      static_cast<float>(response->position.y)};

    if (!vp.initialized) {
      vp.previous = newPos;
      vp.current = newPos;
      vp.initialized = true;
    } else {
      vp.previous = vp.current;
      vp.current = newPos;
    }
  }

  void Render(float alpha) {
    window_.clear(sf::Color::Black);

    for (const auto& [name, vp] : positions_) {
      if (!vp.initialized) continue;

      float x = vp.previous.x * (1.f - alpha) + vp.current.x * alpha;
      float y = vp.previous.y * (1.f - alpha) + vp.current.y * alpha;

      sf::CircleShape circle(5.f);
      circle.setPosition(x, y);

      name == "DTW" ? circle.setFillColor(sf::Color::Blue)
                    : circle.setFillColor(sf::Color::Green);
      window_.draw(circle);
    }

    window_.display();
  }

 private:
  sf::RenderWindow& window_;
  MessageBus& messagebus_;
  std::unordered_map<std::string, VisualPosition> positions_;
};
