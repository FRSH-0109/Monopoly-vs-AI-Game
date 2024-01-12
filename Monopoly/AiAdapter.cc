#include "AiAdapter.h"

AiAdapter::AiAdapter() {
	networkInputs_.assign(127, 0);
}

std::vector<double>& AiAdapter::getInputs() {
	return networkInputs_;
}

double AiAdapter::convertMoney(unsigned int money) {
	double money_normalized = (double)money / 6000.0;
	double money_clamped = std::clamp(money_normalized, 0.0, 1.0);
	return money_clamped;
}

double AiAdapter::convertMoneyValue(float value) {
	return value * 6000.0;
}

double AiAdapter::convertHouseValue(float value) {
	if (value <= 0.5) {
		return 0.0;
	} else {
		return value * 15.0;
	}
}

double AiAdapter::convertPosition(unsigned int position) {
	double position_normalized = (double)position / 39.0;
	double position_clamped = std::clamp(position_normalized, 0.0, 1.0);
	return position_clamped;
}

double AiAdapter::convertCard(unsigned int cards) {
	double cards_clamped = std::clamp((double)cards, 0.0, 1.0);
	return cards_clamped;
}

double AiAdapter::convertHouse(bool isHotel, unsigned int house_number) {
	if (isHotel) {
		return 1.0;
	} else {
		double houses_normalized = (double)house_number / 5.0;
		double houses_clamped = std::clamp(houses_normalized, 0.0, 1.0);
		return houses_clamped;
	}
}

void AiAdapter::setTurn(unsigned int index) {
	for (int i = 0; i < networkInputs_.size(); ++i) {
		if (i < 4) {
			if (i == index) {
				networkInputs_[i] = 1.0;
			} else {
				networkInputs_[i] = 0.0;
			}
		}
	}
}

void AiAdapter::setSelection(unsigned int index) {
	for (int i = SELECT - 1; i < SELECT + 29; ++i) {
		if (i == SELECT + PROPERTIES[index]) {
			networkInputs_[i] = 1.0;
		} else {
			networkInputs_[i] = 0.0;
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
		networkInputs_[i] = 0.0;
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
	float convert = (state + 1) / 4.0;

	networkInputs_[OWNER + PROPERTIES[property]] = convert;
}

void AiAdapter::setMortgage(unsigned int property, unsigned int state) {
	networkInputs_[MORT + PROPERTIES[property]] = state;
}

void AiAdapter::setHouse(bool isHotel, unsigned int houseNumber, unsigned int id) {
	networkInputs_[HOUSE + HOUSES[id]] = convertHouse(isHotel, houseNumber);
}
