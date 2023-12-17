#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Player.h"
#include "gameEngine.h"
#include "main.h"

class Field {
	unsigned int id_;
	FieldType type_;
	std::string name_;
	std::string graphic_path_;
	unsigned int height_;
	unsigned int width_;
	float rotation_;

   public:
	Field(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int height,
		const unsigned int width,
		const float rotation)
		: id_(id), name_(name), graphic_path_(graphic_path), height_(height), width_(width), rotation_(rotation) {
		std::cout << "Field constructor" << std::endl;
	};

	const unsigned int getId() { return id_; };
	const std::string getName() { return name_; };
	const std::string getGraphicPath() { return graphic_path_; };
	const unsigned int getHeight() { return height_; };
	const unsigned int getWidth() { return width_; };
	const float getRotation() { return rotation_; };

	void setHeight(unsigned int new_height, const GameEngine& game_engine);
	void setWidth(unsigned int new_width, const GameEngine& game_engine);
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
		const unsigned int height,
		const unsigned int width,
		const float rotation,
		const unsigned int price,
		const std::map<PropertyTiers, unsigned int> rent_values,
		const std::vector<unsigned int> group_members,
		const unsigned int mortage)
		: Field(id, type, name, graphic_path, height, width, rotation),
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

	const unsigned int getPrice() { return price_; };
	const std::map<PropertyTiers, unsigned int> getRentValues() { return rent_values_; };
	const std::vector<unsigned int> getGroupMembers() { return group_members_; };
	const unsigned int getMortage() { return mortage_; };
	const unsigned int getHouseNumber() { return house_number_; };
	const bool getIsHotel() { return is_hotel_; };
	const bool getIsMortaged() { return is_mortaged_; };
	const unsigned int getUnmortageValue() { return static_cast<int>(1.1 * mortage_); };
	const Player* getOwner() { return owner_; };

	void setHouseNumber(unsigned int new_house_number);
	void setIsHotel(bool new_state);
	void setIsMortaged(bool new_state);
	void setOwner(Player* new_owner);
};

#endif