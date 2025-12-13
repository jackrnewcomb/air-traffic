#include <iostream>

#include "JsonParser.hpp"

int main() {
  std::string json = R"({
        "name": "Voyager",
        "active": true,
        "distance": 22700000000,
        "instruments": ["camera", "plasma sensor"]
    })";

  JsonParser parser(json);
  JsonValue root = parser.Parse();

  std::cout << "Parsed successfully!\n";
}
