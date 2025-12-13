#pragma once

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
};
