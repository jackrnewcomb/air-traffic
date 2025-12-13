#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct JsonValue;

using JsonObject = std::unordered_map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

struct JsonValue {
  using Value = std::variant<std::nullptr_t, bool, double, std::string,
                             JsonArray, JsonObject>;

  Value value;

  JsonValue() : value(nullptr) {}
  template <typename T>
  JsonValue(T v) : value(std::move(v)) {}

  bool IsObject() const { return std::holds_alternative<JsonObject>(value); }

  const JsonValue& Get(const std::string& key) const {
    const auto& obj = std::get<JsonObject>(value);
    auto it = obj.find(key);
    if (it == obj.end()) {
      throw std::runtime_error("Missing JSON key: " + key);
    }
    return it->second;
  }

  std::string AsString() const { return std::get<std::string>(value); }

  double AsNumber() const { return std::get<double>(value); }

  bool AsBool() const { return std::get<bool>(value); }

  const JsonValue& operator[](const std::string& key) const { return Get(key); }
};
