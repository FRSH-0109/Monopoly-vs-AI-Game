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

using PossibleFields = std::variant<Field, PropertyField, StreetField, StationField, UtilityField, TaxField>;

class Board {
	unsigned int field_number_;
	std::vector<PossibleFields> board_;
	sf::Vector2i BOARD_POSITION_ = sf::Vector2i(700, 700);
	float BOARD_SCALE = 1;

   public:
	Board(const std::string file_path);
	const std::vector<PossibleFields>& getBoard();
	const unsigned int getFieldNumber();
	const sf::Vector2i getBoardPosition();
	void clearBoard();
	sf::Vector2i getFieldPositon(unsigned int id, sf::Vector2i prevPos, unsigned int x, unsigned int y);
	float getFieldRotation(unsigned int id);
	PossibleFields& getFieldById(unsigned int wanted_id);
};

std::map<StreetTiers, unsigned int> jsonToStreetRent(const json& element);
std::map<StationTiers, unsigned int> jsonToStationRent(const json& element);
std::map<UtilityTiers, unsigned int> jsonToUtilityRent(const json& element);
