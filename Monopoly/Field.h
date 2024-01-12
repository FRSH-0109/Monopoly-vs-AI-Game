#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Player.h"
#include "ContextWindow.h"
#include "main.h"

class HouseException : public std::exception {
	unsigned int invalid_number_;

   public:
	HouseException(unsigned int houses) : invalid_number_(houses){};
	HouseException(const HouseException& e) throw() : invalid_number_(e.invalid_number_){};
	unsigned int getInvalidNumber();
};

class Field {
	ContextWindow* contextWindow_;
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
	Field(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const sf::Vector2i position);

	ContextWindow* getContextWindow();
	unsigned int getId();
	FieldType getType();
	const std::string getName();
	const std::string getGraphicPath();
	unsigned int getWidth();
	unsigned int getHeight();
	float getRotation();
	const sf::Sprite& getSprite();
	const sf::Texture& getTexture();
	const sf::Vector2i& getPosition();
	const sf::Text& getNameText();

	void createSprite();

	void setHeight(unsigned int new_height);
	void setWidth(unsigned int new_width);
	void setRotation(float new_roation);
	void setPosition(sf::Vector2i pos);
};

class PropertyField : public Field {
	unsigned int price_;
	std::vector<unsigned int> group_members_;
	unsigned int mortgage_;
	bool is_mortgaged_;
	unsigned int unmortgage_value_;
	std::shared_ptr<Player> owner_;
	sf::RectangleShape owner_flag_;

   public:
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
			  //   std::cout << "PropertyField constructor" << std::endl;
		  };
	unsigned int getPrice();
	const std::vector<unsigned int> getGroupMembers();
	const unsigned int getMortgage();
	const bool getIsMortgaged();
	const unsigned int getUnmortgageValue();
	std::shared_ptr<Player> getOwner();
	sf::RectangleShape& getOwnerFlag();

	void setIsMortgaged(bool new_state);
	void setOwner(std::shared_ptr<Player> new_owner);
	void resetOwner();
	void resetDefault();

	void createFlagSprite();
};

class StreetField : public PropertyField {
	std::map<StreetTiers, unsigned int> rent_values_;
	unsigned int house_price_;
	unsigned int hotel_price_;
	unsigned int house_number_;
	bool is_hotel_;

   public:
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
			  //   std::cout << "StreetField constructor" << std::endl;
		  };

	const std::map<StreetTiers, unsigned int> getRentValues();
	unsigned int getHousePrice();
	unsigned int getHotelPrice();
	unsigned int getHouseNumber();
	bool getIsHotel();

	void setHouseNumber(unsigned int new_house_number);
	void setIsHotel(bool new_state);
	void resetDefault();

	unsigned int calculateRent();
};

class StationField : public PropertyField {
	std::map<StationTiers, unsigned int> rent_values_;

   public:
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
			  //   std::cout << "StationField constructor" << std::endl;
		  };

	const std::map<StationTiers, unsigned int> getRentValues();

	unsigned int calculateRent();
};

class UtilityField : public PropertyField {
	std::map<UtilityTiers, unsigned int> rent_multipliers_;

   public:
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

	const std::map<UtilityTiers, unsigned int> getRentMultipliers();

	unsigned int calculateRent(unsigned int dice_roll);
};

class TaxField : public Field {
	unsigned int tax_value_;

   public:
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

	unsigned int getTaxValue();
};
