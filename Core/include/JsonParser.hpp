#pragma once
#include <string>

#include "json.hpp"

class JsonParser {
 public:
  explicit JsonParser(const std::string& input);
  JsonValue Parse();

 private:
  const std::string& src_;
  size_t pos_ = 0;

  void SkipWhitespace();
  char Peek() const;
  char Get();
  bool Match(char c);

  JsonValue ParseValue();
  JsonValue ParseNull();
  JsonValue ParseBool();
  JsonValue ParseNumber();
  JsonValue ParseString();
  JsonValue ParseArray();
  JsonValue ParseObject();
};
