#include "Board.h"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "../json/json.hpp"
#include "Board.h"
#include "main.h"

using json = nlohmann::json;

using PossibleFields = std::variant<Field, PropertyField>;

Board::Board(const std::string file_path) {
	std::map<std::string, FieldType> str_to_type = {{"PROPERTY", PROPERTY}, {"STATION", STATION}, {"UTILITY", UTILITY},
		{"GO", GO}, {"CHANCE", CHANCE}, {"COMUNITY_CHEST", COMMUNITY_CHEST}, {"TAX", TAX}, {"JAIL", JAIL},
		{"FREE_PARKING", FREE_PARKING}, {"GO_TO_JAIL", GO_TO_JAIL}};

	std::ifstream f(file_path);
	json data = json::parse(f);

	for (auto& element : data) {
		unsigned int id = element["id"];
		std::string name = element["name"];
		std::string graphic_path = element["graphic_path"];
		unsigned int width = element["width"];
		unsigned int height = element["height"];
		float rotation = element["rotation"];
		std::string type_in_str = element["type"];
		FieldType type = str_to_type[type_in_str];

		switch (type) {
			case PROPERTY: {
				unsigned int price = element["price"];
				unsigned int mortage = element["mortage"];
				std::map<PropertyTiers, unsigned int> rent_values = jsonToRent(element);
				std::vector<unsigned int> group_members = element["group_members"];
				PropertyField new_field = PropertyField(
					id, type, name, graphic_path, width, height, rotation, price, rent_values, group_members, mortage);
				board_.push_back(new_field);
				break;
			}

			case STATION:

				break;

			case UTILITY:

				break;

			case GO:

				break;

			case COMMUNITY_CHEST: {
				Field new_field = Field(id, type, name, graphic_path, width, height, rotation);
				board_.push_back(new_field);
				break;
			}

			case TAX:

				break;

			case JAIL:

				break;

			case FREE_PARKING:

				break;

			case GO_TO_JAIL:

				break;
		}
	}
};

std::vector<PossibleFields> Board::getBoard() {
	return board_;
};

std::map<PropertyTiers, unsigned int> jsonToRent(const json& element) {
	std::map<PropertyTiers, unsigned int> rent_values;
	std::vector<unsigned int> list_of_rents = element["rent_values"];
	rent_values.emplace(std::make_pair(NO_HOUSES, list_of_rents[0]));
	rent_values.emplace(std::make_pair(ONE_HOUSE, list_of_rents[1]));
	rent_values.emplace(std::make_pair(TWO_HOUESES, list_of_rents[2]));
	rent_values.emplace(std::make_pair(THREE_HOUSES, list_of_rents[3]));
	rent_values.emplace(std::make_pair(FOUR_HOUSES, list_of_rents[4]));
	rent_values.emplace(std::make_pair(HOTEL, list_of_rents[5]));
	return rent_values;
}
