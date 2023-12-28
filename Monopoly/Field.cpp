#include "Field.h"

// HouseException(unsigned int houses) : invalid_number_(houses){};
// HouseException(const HouseException& e) throw() : invalid_number_(e.invalid_number_){};

Field::Field(const unsigned int id,
		const FieldType type,
		const std::string name,
		const std::string graphic_path,
		const unsigned int width,
		const unsigned int height,
		const float rotation) : id_(id),
		  type_(type),
		  name_(name),
		  graphic_path_(graphic_path),
		  width_(width),
		  height_(height),
		  rotation_(rotation) {
			std::cout << "Field constructor" << std::endl;
		}

const unsigned int HouseException::getInvalidNumber() {
	return invalid_number_;
};

const unsigned int Field::getId() {
	return id_;
};

const FieldType Field::getType() {
	return type_;
};

const std::string Field::getName() {
	return name_;
};

const std::string Field::getGraphicPath() {
	return graphic_path_;
};

const unsigned int Field::getWidth() {
	return width_;
};

const unsigned int Field::getHeight() {
	return height_;
};

const float Field::getRotation() {
	return rotation_;
};

const sf::Sprite& Field::getSprite()
{
	return sprite_;
}

const sf::Texture& Field::getTexture() {
	return texture_;
}

void Field::createTexture() {
	contextWindow_ = ContextWindow::GetInstance();
	if(!texture_.loadFromFile(graphic_path_))
	{
		sprite_.setColor(sf::Color::Green);
	}
	sprite_.setTexture(texture_, true);
	const sf::Vector2f SCALE_VECT = sf::Vector2f(0.1f, 0.1f);
	sprite_.setScale(SCALE_VECT);
}

void Field::setHeight(unsigned int new_height) {
	if (contextWindow_->window_.getSize().y >= new_height && new_height != 0) {
		height_ = new_height;
	} else {
		throw(DimensionException(new_height));
	}
};

void Field::setWidth(unsigned int new_width) {
	if (contextWindow_->window_.getSize().x >= new_width && new_width != 0) {
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

const unsigned int PropertyField::getPrice() {
	return price_;
};
const std::map<PropertyTiers, unsigned int> PropertyField::getRentValues() {
	return rent_values_;
};
const std::vector<unsigned int> PropertyField::getGroupMembers() {
	return group_members_;
};
const unsigned int PropertyField::getMortage() {
	return mortage_;
};
const unsigned int PropertyField::getHouseNumber() {
	return house_number_;
};
const bool PropertyField::getIsHotel() {
	return is_hotel_;
};
const bool PropertyField::getIsMortaged() {
	return is_mortaged_;
};
const unsigned int PropertyField::getUnmortageValue() {
	return static_cast<int>(round(1.1 * mortage_));
};
Player* PropertyField::getOwner() {
	return owner_;
};

void PropertyField::setHouseNumber(unsigned int new_house_number) {
	if (new_house_number <= 4) {
		house_number_ = new_house_number;
	} else {
		throw(HouseException(new_house_number));
	}
};

void PropertyField::setIsHotel(bool new_state) {
	is_hotel_ = new_state;
};

void PropertyField::setIsMortaged(bool new_state) {
	is_mortaged_ = new_state;
};

void PropertyField::setOwner(Player* new_owner_ptr) {
	owner_ = new_owner_ptr;
};

void PropertyField::resetDefault() {
	PropertyField::setHouseNumber(0);
	PropertyField::setIsHotel(false);
	PropertyField::setIsMortaged(false);
	PropertyField::setOwner(nullptr);
};

const unsigned int StationField::getPrice() {
	return price_;
};
const std::map<StationTiers, unsigned int> StationField::getRentValues() {
	return rent_values_;
};
const std::vector<unsigned int> StationField::getGroupMembers() {
	return group_members_;
};
const unsigned int StationField::getMortage() {
	return mortage_;
};
const bool StationField::getIsMortaged() {
	return is_mortaged_;
};
const unsigned int StationField::getUnmortageValue() {
	return static_cast<int>(round(1.1 * mortage_));
};
Player* StationField::getOwner() {
	return owner_;
};

void StationField::setIsMortaged(bool new_state) {
	is_mortaged_ = new_state;
};

void StationField::setOwner(Player* new_owner_ptr) {
	owner_ = new_owner_ptr;
};

void StationField::resetDefault() {
	StationField::setIsMortaged(false);
	StationField::setOwner(nullptr);
};

const unsigned int UtilityField::getPrice() {
	return price_;
};
const std::map<UtilityTiers, unsigned int> UtilityField::getRentMultipliers() {
	return rent_multipliers_;
};
const std::vector<unsigned int> UtilityField::getGroupMembers() {
	return group_members_;
};
const unsigned int UtilityField::getMortage() {
	return mortage_;
};
const bool UtilityField::getIsMortaged() {
	return is_mortaged_;
};
const unsigned int UtilityField::getUnmortageValue() {
	return static_cast<int>(round(1.1 * mortage_));
};
Player* UtilityField::getOwner() {
	return owner_;
};

void UtilityField::setIsMortaged(bool new_state) {
	is_mortaged_ = new_state;
};

void UtilityField::setOwner(Player* new_owner_ptr) {
	owner_ = new_owner_ptr;
};

void UtilityField::resetDefault() {
	UtilityField::setIsMortaged(false);
	UtilityField::setOwner(nullptr);
}

const unsigned int TaxField::getTaxValue() {
	return tax_value_;
};
