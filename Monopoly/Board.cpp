#include "Board.h"

using json = nlohmann::json;

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
				std::map<PropertyTiers, unsigned int> rent_values = jsonToPropertyRent(element);
				std::vector<unsigned int> group_members = element["group_members"];
				PropertyField new_field = PropertyField(
					id, type, name, graphic_path, width, height, rotation, price, rent_values, group_members, mortage);
				board_.push_back(new_field);
				break;
			}

			case STATION: {
				unsigned int price = element["price"];
				unsigned int mortage = element["mortage"];
				std::map<StationTiers, unsigned int> rent_values = jsonToStationRent(element);
				std::vector<unsigned int> group_members = element["group_members"];
				StationField new_field = StationField(
					id, type, name, graphic_path, width, height, rotation, price, rent_values, group_members, mortage);
				board_.push_back(new_field);
				break;
			}

			case UTILITY: {
				unsigned int price = element["price"];
				unsigned int mortage = element["mortage"];
				std::map<UtilityTiers, unsigned int> rent_multipliers = jsonToUtilityRent(element);
				std::vector<unsigned int> group_members = element["group_members"];
				UtilityField new_field = UtilityField(id, type, name, graphic_path, width, height, rotation, price,
					rent_multipliers, group_members, mortage);
				board_.push_back(new_field);
				break;
			}

			case TAX: {
				unsigned int tax_value = element["tax_value"];
				TaxField new_field = TaxField(id, type, name, graphic_path, width, height, rotation, tax_value);
				board_.push_back(new_field);
				break;
			}

			case GO:
			case CHANCE:
			case COMMUNITY_CHEST:
			case JAIL:
			case FREE_PARKING:
			case GO_TO_JAIL: {
				Field new_field = Field(id, type, name, graphic_path, width, height, rotation);
				board_.push_back(new_field);
				break;
			}
		}
	}
	field_number_ = board_.size();
	for (auto& field : board_) {
		std::visit([](Field& visited_field) { visited_field.createTexture(); }, field);
	}
};

const std::vector<PossibleFields>& Board::getBoard() {
	return board_;
};

const unsigned int Board::getFieldNumber() {
	return field_number_;
};

void Board::clearBoard() {
	board_.clear();
	field_number_ = board_.size();
};

std::map<PropertyTiers, unsigned int> jsonToPropertyRent(const json& element) {
	std::map<PropertyTiers, unsigned int> rent_values;
	std::vector<unsigned int> list_of_rents = element["rent_values"];
	rent_values.emplace(std::make_pair(NO_HOUSES, list_of_rents[0]));
	rent_values.emplace(std::make_pair(ONE_HOUSE, list_of_rents[1]));
	rent_values.emplace(std::make_pair(TWO_HOUESES, list_of_rents[2]));
	rent_values.emplace(std::make_pair(THREE_HOUSES, list_of_rents[3]));
	rent_values.emplace(std::make_pair(FOUR_HOUSES, list_of_rents[4]));
	rent_values.emplace(std::make_pair(HOTEL, list_of_rents[5]));
	return rent_values;
};

std::map<StationTiers, unsigned int> jsonToStationRent(const json& element) {
	std::map<StationTiers, unsigned int> rent_values;
	std::vector<unsigned int> list_of_rents = element["rent_values"];
	rent_values.emplace(std::make_pair(ONE_STATION, list_of_rents[0]));
	rent_values.emplace(std::make_pair(TWO_STATIONS, list_of_rents[1]));
	rent_values.emplace(std::make_pair(THREE_STATIONS, list_of_rents[2]));
	rent_values.emplace(std::make_pair(FOUR_STATIONS, list_of_rents[3]));
	return rent_values;
};

std::map<UtilityTiers, unsigned int> jsonToUtilityRent(const json& element) {
	std::map<UtilityTiers, unsigned int> rent_multipliers;
	std::vector<unsigned int> list_of_rents = element["rent_multipliers"];
	rent_multipliers.emplace(std::make_pair(ONE_UTILITY, list_of_rents[0]));
	rent_multipliers.emplace(std::make_pair(TWO_UTILITIES, list_of_rents[1]));
	return rent_multipliers;
}
