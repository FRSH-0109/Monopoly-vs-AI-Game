#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "../json/json.hpp"
#include "Field.h"
#include "main.h"

using json = nlohmann::json;

using PossibleFields = std::variant<Field, PropertyField, StationField, UtilityField, TaxField>;

class Board {
	unsigned int field_number_;
	std::vector<PossibleFields> board_;

   public:
	Board(const std::string file_path);
	const std::vector<PossibleFields>& getBoard();
	const unsigned int getFieldNumber();
	void clearBoard();
};

std::map<PropertyTiers, unsigned int> jsonToPropertyRent(const json& element);
std::map<StationTiers, unsigned int> jsonToStationRent(const json& element);
std::map<UtilityTiers, unsigned int> jsonToUtilityRent(const json& element);
