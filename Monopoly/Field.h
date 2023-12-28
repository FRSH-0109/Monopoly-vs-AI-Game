#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Player.h"
#include "contextWindow.h"
#include "main.h"

class HouseException : public std::exception {
	unsigned int invalid_number_;

   public:
	HouseException(unsigned int houses) : invalid_number_(houses){};
	HouseException(const HouseException& e) throw() : invalid_number_(e.invalid_number_){};
	const unsigned int getInvalidNumber();
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

   public:
	Field(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation);

	const unsigned int getId();
	const FieldType getType();
	const std::string getName();
	const std::string getGraphicPath();
	const unsigned int getWidth();
	const unsigned int getHeight();
	const float getRotation();
	const sf::Sprite& getSprite();

	// void setHeight(unsigned int new_height, const GameEngine& game_engine);
	// void setWidth(unsigned int new_width, const GameEngine& game_engine);

	void setHeight(unsigned int new_height);
	void setWidth(unsigned int new_width);
	void setRotation(float new_roation);
};

class PropertyField : public Field {
	unsigned int price_;
	std::map<PropertyTiers, unsigned int> rent_values_;
	std::vector<unsigned int> group_members_;
	unsigned int mortage_;
	unsigned int house_number_;
	bool is_hotel_;
	bool is_mortaged_;
	unsigned int unmortage_value_;
	Player* owner_;

   public:
	PropertyField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const unsigned int price,
		const std::map<PropertyTiers, unsigned int> rent_values,
		const std::vector<unsigned int> group_members,
		const unsigned int mortage)
		: Field(id, type, name, graphic_path, width, height, rotation),
		  price_(price),
		  rent_values_(rent_values),
		  group_members_(group_members),
		  mortage_(mortage),
		  house_number_(0),
		  is_hotel_(false),
		  is_mortaged_(false),
		  owner_(nullptr) {
		std::cout << "PropertyField constructor" << std::endl;
	};

	const unsigned int getPrice();
	const std::map<PropertyTiers, unsigned int> getRentValues();
	const std::vector<unsigned int> getGroupMembers();
	const unsigned int getMortage();
	const unsigned int getHouseNumber();
	const bool getIsHotel();
	const bool getIsMortaged();
	const unsigned int getUnmortageValue();
	Player* getOwner();

	void setHouseNumber(unsigned int new_house_number);
	void setIsHotel(bool new_state);
	void setIsMortaged(bool new_state);
	void setOwner(Player* new_owner);
	void resetDefault();
};

class StationField : public Field {
	unsigned int price_;
	std::map<StationTiers, unsigned int> rent_values_;
	std::vector<unsigned int> group_members_;
	unsigned int mortage_;
	bool is_mortaged_;
	unsigned int unmortage_value_;
	Player* owner_;

   public:
	StationField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const unsigned int price,
		const std::map<StationTiers, unsigned int> rent_values,
		const std::vector<unsigned int> group_members,
		const unsigned int mortage)
		: Field(id, type, name, graphic_path, width, height, rotation),
		  price_(price),
		  rent_values_(rent_values),
		  group_members_(group_members),
		  mortage_(mortage),
		  is_mortaged_(false),
		  owner_(nullptr) {
		std::cout << "StationField constructor" << std::endl;
	};

	const unsigned int getPrice();
	const std::map<StationTiers, unsigned int> getRentValues();
	const std::vector<unsigned int> getGroupMembers();
	const unsigned int getMortage();
	const bool getIsMortaged();
	const unsigned int getUnmortageValue();
	Player* getOwner();

	void setIsMortaged(bool new_state);
	void setOwner(Player* new_owner);
	void resetDefault();
};

class UtilityField : public Field {
	unsigned int price_;
	std::map<UtilityTiers, unsigned int> rent_multipliers_;
	std::vector<unsigned int> group_members_;
	unsigned int mortage_;
	bool is_mortaged_;
	unsigned int unmortage_value_;
	Player* owner_;

   public:
	UtilityField(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation,
		const unsigned int price,
		const std::map<UtilityTiers, unsigned int> rent_multipliers,
		const std::vector<unsigned int> group_members,
		const unsigned int mortage)
		: Field(id, type, name, graphic_path, width, height, rotation),
		  price_(price),
		  rent_multipliers_(rent_multipliers),
		  group_members_(group_members),
		  mortage_(mortage),
		  is_mortaged_(false),
		  owner_(nullptr) {
		std::cout << "UtilityField constructor" << std::endl;
	};

	const unsigned int getPrice();
	const std::map<UtilityTiers, unsigned int> getRentMultipliers();
	const std::vector<unsigned int> getGroupMembers();
	const unsigned int getMortage();
	const bool getIsMortaged();
	const unsigned int getUnmortageValue();
	Player* getOwner();

	void setIsMortaged(bool new_state);
	void setOwner(Player* new_owner);
	void resetDefault();
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
		const unsigned int tax_value)
		: Field(id, type, name, graphic_path, width, height, rotation), tax_value_(tax_value) {
		std::cout << "TaxField constructor" << std::endl;
	};

	const unsigned int getTaxValue();
};
