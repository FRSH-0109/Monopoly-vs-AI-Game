#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <variant>
#include <vector>
#include "../json/json.hpp"
#include "Field.hpp"

using json = nlohmann::json;

using PossibleFields = std::variant<Field, PropertyField>;

class Board {
	unsigned int field_number_;
	std::vector<PossibleFields> board_;

   public:
	Board(const std::string file_path);
	std::vector<PossibleFields> getBoard();
};

std::map<PropertyTiers, unsigned int> jsonToRent(const json& element);

#endif