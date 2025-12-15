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

  static const JsonValue& Null() {
    static JsonValue null_value;
    return null_value;
  }

  const JsonValue& Get(const std::string& key) const {
    if (!std::holds_alternative<JsonObject>(value)) {
      return Null();
    }

    const auto& obj = std::get<JsonObject>(value);
    auto it = obj.find(key);
    return (it != obj.end()) ? it->second : Null();
  }

  const JsonValue& operator[](const std::string& key) const { return Get(key); }

  std::string AsString() const {
    if (std::holds_alternative<std::string>(value)) {
      return std::get<std::string>(value);
    }
    return "";
  }

  double AsNumber() const {
    if (std::holds_alternative<double>(value)) {
      return std::get<double>(value);
    }
    return 0.0;
  }

  bool AsBool() const {
    if (std::holds_alternative<bool>(value)) {
      return std::get<bool>(value);
    }
    return false;
  }

  const JsonArray& AsArray() const {
    if (!std::holds_alternative<JsonArray>(value)) {
      throw std::runtime_error("JsonValue is not an array");
    }
    return std::get<JsonArray>(value);
  }
};
