/**
 * @file Field.h
 *
 * @brief Header file containing staff for single monopoly
 * board game field. Separated to many deriving classes types
 * each for specific field type.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef FIELD_H
#define FIELD_H

#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "ContextWindow.h"
#include "Player.h"
#include "main.h"

/**
 * @brief Custom exception class for handling invalid house numbers.
 */
class HouseException : public std::exception {
	unsigned int invalid_number_;

   public:
	HouseException(unsigned int houses) : invalid_number_(houses){};
	HouseException(const HouseException& e) throw() : invalid_number_(e.invalid_number_){};
	unsigned int getInvalidNumber();
};

/**
 * @brief Base class representing a generic game field.
 */
class Field {
	ContextWindow* context_window_;
	unsigned int id_;
	FieldType type_;
	std::string name_;
	std::string graphic_path_;
	unsigned int width_;
	unsigned int height_;
	float rotation_;
	sf::Texture texture_;
	sf::Sprite sprite_;
	sf::Vector2i position_;
	sf::Text nameText_;

   public:
  	/**
	 * @brief Constructor for the Field class.
	 * @param id The ID of the field.
	 * @param type The type of the field.
	 * @param name The name of the field.
	 * @param graphic_path The file path to the field's graphic.
	 * @param width The width of the field.
	 * @param height The height of the field.
	 * @param rotation The rotation angle of the field.
	 * @param position The position of the field on the board.
	 */
	Field(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position);

	/**
	 * @brief Gets the pointer to the context window.
	 * @return A pointer to the context window.
	 */
	ContextWindow* getContextWindow();

	/**
	 * @brief Gets the ID of the field.
	 * @return The ID of the field.
	 */
	unsigned int getId();

	/*
	 * @brief Gets the type of the field.
	 * @return The type of the field.
	 */
	FieldType getType();

	/**
	 * @brief Gets the name of the field.
	 * @return The name of the field.
	 */
	const std::string getName();

	/**
	 * @brief Gets the file path to the field's graphic.
	 * @return The file path to the field's graphic.
	 */
	const std::string getGraphicPath();

	/**
	 * @brief Gets the width of the field.
	 * @return The width of the field.
	 */
	unsigned int getWidth();

	/**
	 * @brief Gets the height of the field.
	 * @return The height of the field.
	 */
	unsigned int getHeight();

	/**
	 * @brief Gets the rotation angle of the field.
	 * @return The rotation angle of the field.
	 */
	float getRotation();

	/**
	 * @brief Gets the sprite representing the field.
	 * @return The sprite representing the field.
	 */
	const sf::Sprite& getSprite();

	/**
	 * @brief Gets the texture of the field.
	 * @return The texture of the field.
	 */
	const sf::Texture& getTexture();

	/**
	 * @brief Gets the position of the field on the board.
	 * @return The position of the field on the board.
	 */
	const sf::Vector2i& getPosition();

	/**
	 * @brief Gets the text representing the name of the field.
	 * @return The text representing the name of the field.
	 */
	const sf::Text& getNameText();

	/**
	 * @brief Creates the sprite for the field.
	 */
	void createSprite();

	/**
	 * @brief Sets the height of the field.
	 * @param new_height The new height of the field.
	 */
	void setHeight(unsigned int new_height);

	/**
	 * @brief Sets the width of the field.
	 * @param new_width The new width of the field.
	 */
	void setWidth(unsigned int new_width);

	/**
	 * @brief Sets the rotation angle of the field.
	 * @param new_rotation The new rotation angle of the field.
	 */
	void setRotation(float new_rotation);

	/**
	 * @brief Sets the position of the field on the board.
	 * @param pos The new position of the field.
	 */
	void setPosition(sf::Vector2i pos);
};

/**
 * @brief Derived class representing a property field on the game board.
 */
class PropertyField : public Field {
	unsigned int price_;
	std::vector<unsigned int> group_members_;
	unsigned int mortgage_;
	bool is_mortgaged_;
	unsigned int unmortgage_value_;
	std::shared_ptr<Player> owner_;
	sf::RectangleShape owner_flag_;

   public:
   	/**
	 * @brief Constructor for the PropertyField class.
	 * @param id The ID of the field.
	 * @param type The type of the field.
	 * @param name The name of the field.
	 * @param graphic_path The file path to the field's graphic.
	 * @param width The width of the field.
	 * @param height The height of the field.
	 * @param rotation The rotation angle of the field.
	 * @param position The position of the field on the board.
	 * @param price The price of the property.
	 * @param group_members The IDs of the group members (properties in the same group).
	 * @param mortgage The mortgage value of the property.
	 */
	PropertyField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position,
		const unsigned int price,
		const std::vector<unsigned int> group_members,
		const unsigned int mortgage)
		: Field(id, type, name, graphic_path, width, height, rotation, position),
		  price_(price),
		  group_members_(group_members),
		  mortgage_(mortgage),
		  is_mortgaged_(false),
		  owner_(nullptr){
		  };

	/**
	 * @brief Gets the price of the property.
	 * @return The price of the property.
	 */
	unsigned int getPrice();

	/**
	 * @brief Gets the IDs of the group members (properties in the same group).
	 * @return The IDs of the group members.
	 */
	const std::vector<unsigned int> getGroupMembers();

	/**
	 * @brief Gets the mortgage value of the property.
	 * @return The mortgage value of the property.
	 */
	const unsigned int getMortgage();

	/**
	 * @brief Checks if the property is mortgaged.
	 * @return True if the property is mortgaged, false otherwise.
	 */
	const bool getIsMortgaged();

	/**
	 * @brief Gets the value needed to unmortgage the property.
	 * @return The value needed to unmortgage the property.
	 */
	const unsigned int getUnmortgageValue();

	/**
	 * @brief Gets the owner of the property.
	 * @return A shared pointer to the owner of the property.
	 */
	std::shared_ptr<Player> getOwner();

	/**
	 * @brief Gets the owner flag sprite.
	 * @return The owner flag sprite.
	 */
	sf::RectangleShape& getOwnerFlag();

	/**
	 * @brief Sets the mortgaged state of the property.
	 * @param new_state The new mortgaged state.
	 */
	void setIsMortgaged(bool new_state);

	/**
	 * @brief Sets the owner of the property.
	 * @param new_owner A shared pointer to the new owner.
	 */
	void setOwner(std::shared_ptr<Player> new_owner);

	/**
	 * @brief Resets the owner of the property.
	 */
	void resetOwner();

	/**
	 * @brief Resets the property to its default state.
	 */
	void resetDefault();

	/**
	 * @brief Creates the owner flag sprite.
	 */
	void createFlagSprite();
};


/**
 * @brief Derived class representing a street field on the game board.
 */
class StreetField : public PropertyField {
	std::map<StreetTiers, unsigned int> rent_values_;
	unsigned int house_price_;
	unsigned int hotel_price_;
	unsigned int house_number_;
	bool is_hotel_;

   public:
    /**
	 * @brief Constructor for the StreetField class.
	 * @param id The ID of the field.
	 * @param type The type of the field.
	 * @param name The name of the field.
	 * @param graphic_path The file path to the field's graphic.
	 * @param width The width of the field.
	 * @param height The height of the field.
	 * @param rotation The rotation angle of the field.
	 * @param position The position of the field on the board.
	 * @param price The price of the property.
	 * @param house_price The price of a house.
	 * @param hotel_price The price of a hotel.
	 * @param rent_values The rent values for different tiers.
	 * @param group_members The IDs of the group members (properties in the same group).
	 * @param mortgage The mortgage value of the property.
	 */
	StreetField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position,
		const unsigned int price,
		const unsigned int house_price,
		const unsigned int hotel_price,
		const std::map<StreetTiers, unsigned int> rent_values,
		const std::vector<unsigned int> group_members,
		const unsigned int Mortgage)
		: PropertyField(id,
			  type,
			  name,
			  graphic_path,
			  width,
			  height,
			  rotation,
			  position,
			  price,
			  group_members,
			  Mortgage),
		  rent_values_(rent_values),
		  house_price_(house_price),
		  hotel_price_(hotel_price),
		  house_number_(0),
		  is_hotel_(false){
		  };

	/**
	 * @brief Gets the rent values for different tiers.
	 * @return The rent values for different tiers.
	 */
	const std::map<StreetTiers, unsigned int> getRentValues();

	/**
	 * @brief Gets the price of a house.
	 * @return The price of a house.
	 */
	unsigned int getHousePrice();

	/**
	 * @brief Gets the price of a hotel.
	 * @return The price of a hotel.
	 */
	unsigned int getHotelPrice();

	/**
	 * @brief Gets the number of houses on the property.
	 * @return The number of houses on the property.
	 */
	unsigned int getHouseNumber();

	/**
	 * @brief Checks if there is a hotel on the property.
	 * @return True if there is a hotel, false otherwise.
	 */
	bool getIsHotel();

	/**
	 * @brief Sets the number of houses on the property.
	 * @param new_house_number The new number of houses on the property.
	 */
	void setHouseNumber(unsigned int new_house_number);

	/**
	 * @brief Sets the hotel state on the property.
	 * @param new_state The new hotel state.
	 */
	void setIsHotel(bool new_state);

	/**
	 * @brief Resets the property to its default state.
	 */
	void resetDefault();

	/**
	 * @brief Calculates the rent for the property.
	 * @return The calculated rent value.
	 */
	unsigned int calculateRent();
};

/**
 * @brief Derived class representing a station field on the game board.
 */
class StationField : public PropertyField {
	std::map<StationTiers, unsigned int> rent_values_;

   public:
    /**
	 * @brief Constructor for the StationField class.
	 * @param id The ID of the field.
	 * @param type The type of the field.
	 * @param name The name of the field.
	 * @param graphic_path The file path to the field's graphic.
	 * @param width The width of the field.
	 * @param height The height of the field.
	 * @param rotation The rotation angle of the field.
	 * @param position The position of the field on the board.
	 * @param price The price of the property.
	 * @param rent_values The rent values for different tiers.
	 * @param group_members The IDs of the group members (properties in the same group).
	 * @param mortgage The mortgage value of the property.
	 */
	StationField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position,
		const unsigned int price,
		const std::map<StationTiers, unsigned int> rent_values,
		const std::vector<unsigned int> group_members,
		const unsigned int Mortgage)
		: PropertyField(id,
			  type,
			  name,
			  graphic_path,
			  width,
			  height,
			  rotation,
			  position,
			  price,
			  group_members,
			  Mortgage),
		  rent_values_(rent_values){
		  };

	/**
	 * @brief Gets the rent values for different tiers.
	 * @return The rent values for different tiers.
	 */
	const std::map<StationTiers, unsigned int> getRentValues();

	/**
	 * @brief Calculates the rent for the property.
	 * @return The calculated rent value.
	 */
	unsigned int calculateRent();
};

class UtilityField : public PropertyField {
	std::map<UtilityTiers, unsigned int> rent_multipliers_;

   public:
    /**
	 * @brief Constructor for the UtilityField class.
	 * @param id The ID of the field.
	 * @param type The type of the field.
	 * @param name The name of the field.
	 * @param graphic_path The file path to the field's graphic.
	 * @param width The width of the field.
	 * @param height The height of the field.
	 * @param rotation The rotation angle of the field.
	 * @param position The position of the field on the board.
	 * @param price The price of the property.
	 * @param rent_multipliers The rent multipliers for different tiers.
	 * @param group_members The IDs of the group members (properties in the same group).
	 * @param mortgage The mortgage value of the property.
	 */
	UtilityField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position,
		const unsigned int price,
		const std::map<UtilityTiers, unsigned int> rent_multipliers,
		const std::vector<unsigned int> group_members,
		const unsigned int Mortgage)
		: PropertyField(id,
			  type,
			  name,
			  graphic_path,
			  width,
			  height,
			  rotation,
			  position,
			  price,
			  group_members,
			  Mortgage),
		  rent_multipliers_(rent_multipliers){
			  //   std::cout << "UtilityField constructor" << std::endl;
		  };

	/**
	 * @brief Calculates the rent for the utility property.
	 * @return Map for Utility tiers mapped to mulitpliers values
	 */
	const std::map<UtilityTiers, unsigned int> getRentMultipliers();

	/**
	 * @brief Calculates the rent for the utility property.
	 * @param dice_roll The roll of the dice.
	 * @return The calculated rent value.
	 */
	unsigned int calculateRent(unsigned int dice_roll);
};

/**
 * @class TaxField
 * @brief Derived class representing a tax field on the monopoly board.
 */
class TaxField : public Field {
	unsigned int tax_value_;

   public:
   /**
	 * @brief Constructor for the TaxField class.
	 * @param id The unique identifier of the field.
	 * @param type The type of the field.
	 * @param name The name of the field.
	 * @param graphic_path The file path to the graphic representation of the field.
	 * @param width The width of the field.
	 * @param height The height of the field.
	 * @param rotation The rotation angle of the field.
	 * @param position The position of the field on the game board.
	 * @param tax_value The value of the tax.
	 */
	TaxField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position,
		const unsigned int tax_value)
		: Field(id, type, name, graphic_path, width, height, rotation, position),
		  tax_value_(tax_value){
			  //   std::cout << "TaxField constructor" << std::endl;
		  };

	/**
	 * @brief Calculates the tax for Tax property.
	 * @return The calculated tax value.
	 */
	unsigned int getTaxValue();
};

#endif