#include "Field.hpp"

void Field::setHeight(unsigned int new_height, const GameEngine& game_engine) {
	if (game_engine.getWindowHeight() >= new_height && new_height != 0) {
		height_ = new_height;
	} else {
		throw(DimensionException(new_height));
	}
};

void Field::setWidth(unsigned int new_width, const GameEngine& game_engine) {
	if (game_engine.getWindowWidth() >= new_width && new_width != 0) {
		width_ = new_width;
	} else {
		throw(DimensionException(new_width));
	}
};

void Field::setRotation(float new_roation) {
	if (new_roation > 0.0 && new_roation < 360.0) {
		rotation_ = new_roation;
	} else {
		throw(RotationException(new_roation));
	}
};

void PropertyField::setHouseNumber(unsigned int new_house_number) {
	if (new_house_number <= 4) {
		house_number_ = new_house_number;
	} else {
		throw(HouseException(new_house_number));
	}
};

void PropertyField::setIsHotel(bool new_state){
	is_hotel_ = new_state;
};

void PropertyField::setIsMortaged(bool new_state){
	is_mortaged_ = new_state;
};

void PropertyField::setOwner(Player* new_owner_ptr) {
	owner_ = new_owner_ptr;
};