#include <fstream>
#include <iostream>
#include "../json/json.hpp"
#include "Board.h"

using json = nlohmann::json;

Board::Board(std::string file_path) {
    std::ifstream f(file_path);
    json data = json::parse(f);

    std::cout << data << std::endl;
};