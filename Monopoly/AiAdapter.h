/**
 * @file AiAdapter.h
 *
 * @brief TODO
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef AI_ADAPTER_H
#define AI_ADAPTER_H

#include <algorithm>

class AiAdapter {
	std::vector<double> networkInputs_;
	const unsigned int TURN = 0;
	const unsigned int POS = 4;
	const unsigned int MONEY = 8;
	const unsigned int CARD = 12;
	const unsigned int JAIL = 16;
	const unsigned int OWNER = 20;
	const unsigned int MORT = 48;
	const unsigned int HOUSE = 76;
	unsigned int SELECT = 98;
	const unsigned int SELECT_MONEY = 126;

	const std::vector<int> PROPERTIES = {-1, 0, -1, 1, -1, 2, 3, -1, 4, 5, -1, 6, 7, 8, 9, 10, 11, -1, 12, 13, -1, 14,
		-1, 15, 16, 17, 18, 19, 20, 21, -1, 22, 23, -1, 24, 25, -1, 26, -1, 27};
	const std::vector<int> HOUSES = {-1, 0, -1, 1, -1, -1, 2, -1, 3, 4, -1, 5, -1, 6, 7, -1, 8, -1, 9, 10, -1, 11, -1,
		12, 13, -1, 14, 15, -1, 16, -1, 17, 18, -1, 19, -1, -1, 20, -1, 21};

   public:
	AiAdapter();
	std::vector<double> getInputs();
	double convertMoney(unsigned int money);
	double convertMoneyValue(double value);
	double convertHouseValue(double value);
	double convertPosition(unsigned int position);
	double convertCard(unsigned int cards);
	double convertHouse(bool isHotel, unsigned int houseNumber);
	void setTurn(unsigned int index);
	void setSelection(unsigned int index);
	void setSelectionState(unsigned int index, int state);
	void setMoneyContext(int state);
	void clearSelectionState();
	void setPosition(unsigned int index, unsigned int position);
	void setMoney(unsigned int index, unsigned int money);
	void setCard(unsigned int index, unsigned int cards);
	void setJail(unsigned int index, unsigned int state);
	void setOwner(unsigned int property, unsigned int state);
	void setMortgage(unsigned int property, unsigned int state);
	void setHouse(bool isHotel, unsigned int houseNumber, unsigned int id);
};

#endif