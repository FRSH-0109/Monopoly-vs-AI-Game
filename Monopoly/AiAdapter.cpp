#include "AiAdapter.h"

AiAdapter::AiAdapter() {
	networkInputs_.assign(127, 0);
}

std::vector<float>& AiAdapter::getInputs() {
	return networkInputs_;
}

float AiAdapter::convertMoney(unsigned int money) {
	float money_normalized = (float)money / 6000.0f;
	float money_clamped = std::clamp(money_normalized, 0.0f, 1.0f);
	return money_clamped;
}

float AiAdapter::convertMoneyValue(float value) {
	return value * 6000.0f;
}

float AiAdapter::convertHouseValue(float value) {
	if (value <= 0.5f) {
		return 0.0f;
	} else {
		return value * 15.0f;
	}
}

float AiAdapter::convertPosition(unsigned int position) {
	float position_normalized = (float)position / 39.0f;
	float position_clamped = std::clamp(position_normalized, 0.0f, 1.0f);
	return position_clamped;
}

float AiAdapter::convertCard(unsigned int cards) {
	float cards_clamped = std::clamp((float)cards, 0.0f, 1.0f);
	return cards_clamped;
}

float AiAdapter::convertHouse(StreetField& field) {
	if (field.getIsHotel()) {
		return 1.0f;
	} else {
		unsigned int houses = field.getHouseNumber();
		float houses_normalized = (float)houses / 5.0f;
		float houses_clamped = std::clamp(houses_normalized, 0.0f, 1.0f);
		return houses_clamped;
	}
}

void AiAdapter::setTurn(unsigned int index) {
	for (int i = 0; i < networkInputs_.size(); ++i) {
		if (i < 4) {
			if (i == index) {
				networkInputs_[i] = 1.0f;
			} else {
				networkInputs_[i] = 0.0f;
			}
		}
	}
}

void AiAdapter::setSelection(unsigned int index) {
	for (int i = SELECT - 1; i < SELECT + 29; ++i) {
		if (i == SELECT + PROPERTIES[index]) {
			networkInputs_[i] = 1.0f;
		} else {
			networkInputs_[i] = 0.0f;
		}
	}
}

void AiAdapter::setSelectionState(unsigned int index, int state) {
	networkInputs_[SELECT + PROPERTIES[index]] = state;
}

void AiAdapter::setMoneyContext(unsigned int state) {
	networkInputs_[SELECT_MONEY] = state;
}

void AiAdapter::clearSelectionState() {
	for (int i = SELECT; i < SELECT + 29; ++i) {
		networkInputs_[i] = 0.0f;
	}
}

void AiAdapter::setPosition(unsigned int index, unsigned int position) {
	networkInputs_[POS + index] = convertPosition(position);
}

void AiAdapter::setMoney(unsigned int index, unsigned int money) {
	networkInputs_[MONEY + index] = convertMoney(money);
}

void AiAdapter::setCard(unsigned int index, unsigned int cards) {
	networkInputs_[CARD + index] = convertCard(cards);
}

void AiAdapter::setJail(unsigned int index, unsigned int state) {
	networkInputs_[JAIL + index] = state;
}

void AiAdapter::setOwner(unsigned int property, unsigned int state) {
	float convert = (state + 1) / 4.0f;

	networkInputs_[OWNER + PROPERTIES[property]] = convert;
}

void AiAdapter::setMortgage(unsigned int property, unsigned int state) {
	networkInputs_[MORT + PROPERTIES[property]] = state;
}

void AiAdapter::setHouse(StreetField& field) {
	networkInputs_[HOUSE + HOUSES[field.getId()]] = convertHouse(field);
}
