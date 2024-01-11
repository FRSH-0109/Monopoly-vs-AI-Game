#include "AiAdapter.h"

AiAdapter::AiAdapter() {
	networkInputs_.reserve(127);
}

std::vector<float> AiAdapter::getInputs() {
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

void AiAdapter::setTurn(unsigned int index) {
	for (int i = 0; i < 4; ++i) {
		networkInputs_[i] = 0.0f;
	}

	networkInputs_[index] = 1.0f;
}