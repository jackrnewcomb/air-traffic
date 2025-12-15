#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

#include "Messaging/MessageBus.hpp"
#include "Messaging/MessageTypes.hpp"

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
        std::bind(&VisualsSystem::ProcessAircraftStatusResponseMessage, this,
                  std::placeholders::_1));

    messagebus_.Subscribe(
        "DestinationStatusResponseMessage",
        std::bind(&VisualsSystem::ProcessDestinationStatusResponseMessage, this,
                  std::placeholders::_1));
  }

  void ProcessAircraftStatusResponseMessage(const Message& msg) {
    auto response = dynamic_cast<const AircraftStatusResponseMessage*>(&msg);
    if (!response) return;

    auto& vp = aircraft_positions_[response->sender];

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

  void ProcessDestinationStatusResponseMessage(const Message& msg) {
    auto response = dynamic_cast<const DestinationStatusResponseMessage*>(&msg);
    if (!response) return;

    auto& vp = destination_positions_[response->sender];

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
    DestinationStatusRequestMessage request("visuals", "all");
    messagebus_.Publish(request);

    window_.clear(sf::Color::Black);

    for (const auto& [name, vp] : aircraft_positions_) {
      if (!vp.initialized) continue;

      float x = vp.previous.x * (1.f - alpha) + vp.current.x * alpha;
      float y = vp.previous.y * (1.f - alpha) + vp.current.y * alpha;

      sf::CircleShape circle(5.f);
      circle.setPosition(x, y);

      circle.setFillColor(sf::Color::Green);
      window_.draw(circle);
    }

    for (const auto& [name, vp] : destination_positions_) {
      if (!vp.initialized) continue;

      float x = vp.previous.x * (1.f - alpha) + vp.current.x * alpha;
      float y = vp.previous.y * (1.f - alpha) + vp.current.y * alpha;

      sf::CircleShape circle(5.f);
      circle.setPosition(x, y);

      circle.setFillColor(sf::Color::Blue);
      window_.draw(circle);
    }

    window_.display();
  }

 private:
  sf::RenderWindow& window_;
  MessageBus& messagebus_;
  std::unordered_map<std::string, VisualPosition> aircraft_positions_;
  std::unordered_map<std::string, VisualPosition> destination_positions_;
};
