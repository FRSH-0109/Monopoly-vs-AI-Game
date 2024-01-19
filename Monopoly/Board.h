/**
 * @file Board.h
 *
 * @brief Header file for monopoly game board, creation is
 * based on json file 'board.json'. Mainly handles all types of
 * fields and their usage.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef BOARD_H
#define BOARD_H

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

/**
 * @typedef PossibleFields
 * @brief A variant type representing different types of fields on the monopoly board.
 */

using PossibleFields = std::variant<Field, PropertyField, StreetField, StationField, UtilityField, TaxField>;

/**
 * @class Board
 * @brief Class representing the monopoly game board.
 */
class Board {
	unsigned int field_number_;								/** Numer of field on the board*/
	std::vector<PossibleFields> board_;						/** Vector containing all fields*/
	sf::Vector2i BOARD_POSITION_ = sf::Vector2i(700, 700);	/** Origin position for displaying board stuff*/
	float BOARD_SCALE = 1;									/** Scale for displaying board stuff*/

   public:
   	/**
	 * @brief Constructor for the Board class.
	 * @param file_path The file path to the 'board.json' file.
	 */
	Board(const std::string file_path);

	/**
	 * @brief Getter for the entire game board.
	 * @return A vector containing all types of fields on the board.
	 */
	const std::vector<PossibleFields>& getBoard();

	/**
	 * @brief Getter for the number of fields on the board.
	 * @return The number of fields.
	 */
	unsigned int getFieldNumber();

	/**
	 * @brief Getter for the position of the entire game board.
	 * @return The position as an sf::Vector2i.
	 */
	const sf::Vector2i getBoardPosition();

	/**
	 * @brief Clears the entire game board.
	 */
	void clearBoard();

	/**
	 * @brief Getter for the position of a specific field on the board.
	 * @param id The unique identifier of the field.
	 * @param prevPos The previous position of the field.
	 * @param x The horizontal position of the field.
	 * @param y The vertical position of the field.
	 * @return The position of the field as an sf::Vector2i.
	 */
	sf::Vector2i getFieldPositon(unsigned int id, sf::Vector2i prevPos, unsigned int x, unsigned int y);

	/**
	 * @brief Getter for the rotation angle of a specific field on the board.
	 * @param id The unique identifier of the field.
	 * @return The rotation angle of the field.
	 */
	float getFieldRotation(unsigned int id);

	/**
	 * @brief Getter for a specific field on the board by its unique identifier.
	 * @param wanted_id The unique identifier of the field.
	 * @return A reference to the field specified by the identifier.
	 */
	PossibleFields& getFieldById(unsigned int wanted_id);
};

/**
 * @brief Converts JSON data to a map of rent values for street properties.
 * @param element The JSON data containing rent values.
 * @return A map of rent values for street properties.
 */
std::map<StreetTiers, unsigned int> jsonToStreetRent(const json& element);

/**
 * @brief Converts JSON data to a map of rent values for station properties.
 * @param element The JSON data containing rent values.
 * @return A map of rent values for station properties.
 */
std::map<StationTiers, unsigned int> jsonToStationRent(const json& element);

/**
 * @brief Converts JSON data to a map of rent multipliers for utility properties.
 * @param element The JSON data containing rent multipliers.
 * @return A map of rent multipliers for utility properties.
 */
std::map<UtilityTiers, unsigned int> jsonToUtilityRent(const json& element);

#endif