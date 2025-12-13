#include "JsonParser.hpp"

#include <cctype>
#include <stdexcept>

JsonParser::JsonParser(const std::string& input) : src_(input) {}

JsonValue JsonParser::Parse() {
  SkipWhitespace();
  JsonValue val = ParseValue();
  SkipWhitespace();
  if (pos_ != src_.size())
    throw std::runtime_error("Unexpected trailing characters");
  return val;
}

void JsonParser::SkipWhitespace() {
  while (pos_ < src_.size() && std::isspace(src_[pos_])) pos_++;
}

char JsonParser::Peek() const {
  if (pos_ >= src_.size()) throw std::runtime_error("Unexpected end of input");
  return src_[pos_];
}

char JsonParser::Get() { return src_[pos_++]; }

bool JsonParser::Match(char c) {
  if (Peek() == c) {
    pos_++;
    return true;
  }
  return false;
}

JsonValue JsonParser::ParseValue() {
  char c = Peek();
  if (c == 'n') return ParseNull();
  if (c == 't' || c == 'f') return ParseBool();
  if (c == '"') return ParseString();
  if (c == '[') return ParseArray();
  if (c == '{') return ParseObject();
  if (c == '-' || std::isdigit(c)) return ParseNumber();

  throw std::runtime_error("Invalid JSON value");
}

JsonValue JsonParser::ParseNull() {
  if (src_.substr(pos_, 4) != "null") throw std::runtime_error("Invalid null");
  pos_ += 4;
  return nullptr;
}

JsonValue JsonParser::ParseBool() {
  if (src_.substr(pos_, 4) == "true") {
    pos_ += 4;
    return true;
  }
  if (src_.substr(pos_, 5) == "false") {
    pos_ += 5;
    return false;
  }
  throw std::runtime_error("Invalid boolean");
}

JsonValue JsonParser::ParseNumber() {
  size_t start = pos_;
  if (Peek() == '-') pos_++;
  while (pos_ < src_.size() && std::isdigit(src_[pos_])) pos_++;
  if (pos_ < src_.size() && src_[pos_] == '.') {
    pos_++;
    while (pos_ < src_.size() && std::isdigit(src_[pos_])) pos_++;
  }
  double value = std::stod(src_.substr(start, pos_ - start));
  return value;
}

JsonValue JsonParser::ParseString() {
  Get();  // opening quote
  std::string result;

  while (true) {
    char c = Get();
    if (c == '"') break;
    if (c == '\\') {
      char esc = Get();
      if (esc == '"' || esc == '\\' || esc == '/')
        result += esc;
      else if (esc == 'n')
        result += '\n';
      else if (esc == 't')
        result += '\t';
      else
        throw std::runtime_error("Invalid escape");
    } else {
      result += c;
    }
  }
  return result;
}

JsonValue JsonParser::ParseArray() {
  Get();  // [
  JsonArray arr;
  SkipWhitespace();

  if (Match(']')) return arr;

  while (true) {
    SkipWhitespace();
    arr.push_back(ParseValue());
    SkipWhitespace();

    if (Match(']')) break;
    if (!Match(',')) throw std::runtime_error("Expected ',' in array");
  }
  return arr;
}

JsonValue JsonParser::ParseObject() {
  Get();  // {
  JsonObject obj;
  SkipWhitespace();

  if (Match('}')) return obj;

  while (true) {
    SkipWhitespace();
    if (Peek() != '"') throw std::runtime_error("Expected string key");
    std::string key = std::get<std::string>(ParseString().value);
    SkipWhitespace();

    if (!Match(':')) throw std::runtime_error("Expected ':'");
    SkipWhitespace();

    obj[key] = ParseValue();
    SkipWhitespace();

    if (Match('}')) break;
    if (!Match(',')) throw std::runtime_error("Expected ',' in object");
  }
  return obj;
}
