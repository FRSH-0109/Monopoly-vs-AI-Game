#include "monopolyGameEngine.h"

monopolyGameEngine::monopolyGameEngine() {
	turnState_ = RollDice;
	if (!houseTexture_.loadFromFile("textures_and_fonts/textures/house.png")) {
		// TODO: exception
	}
	if (!hotelTexture_.loadFromFile("textures_and_fonts/textures/hotel.png")) {
		// TODO: exception
	}
}

void monopolyGameEngine::setScreenType(GameScreenType newScreenType) {
	screenType_ = newScreenType;
}
const GameScreenType monopolyGameEngine::getScreenType() const {
	return screenType_;
}

void monopolyGameEngine::createBoard() {
	gameboard_ = std::make_shared<Board>(GAMEBOARD_FILE_PATH_);
}

void monopolyGameEngine::createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list) {
	int i = 0;
	// TODO Parsować po wektorze playerSettings i generować na jego podstawie przeshufflowaną listę graczy)
	int playerId = 0;
	for (auto it : player_settings_list) {
		if (!(it->isNone)) {
			Player new_player = Player(PLAYER_MONEY_DEFAULT_);
			new_player.setIsAi(!(it->isHuman));
			new_player.setAiLevel(it->level);
			new_player.setId(playerId);
			players_.push_back(std::make_shared<Player>(new_player));
			// if (new_player.getId() == 1) {
			// 	players_[1]->addFieldOwnedId(16);
			// 	players_[1]->addFieldOwnedId(18);
			// 	players_[1]->addFieldOwnedId(19);
			// 	std::get<StreetField>(gameboard_->getFieldById(16)).setOwner(players_[1]);
			// 	std::get<StreetField>(gameboard_->getFieldById(18)).setOwner(players_[1]);
			// 	std::get<StreetField>(gameboard_->getFieldById(19)).setOwner(players_[1]);
			// }
			// if (new_player.getId() == 2) {
			// 	players_[2]->addFieldOwnedId(1);
			// 	players_[2]->addFieldOwnedId(3);
			// 	std::get<StreetField>(gameboard_->getFieldById(1)).setOwner(players_[2]);
			// 	std::get<StreetField>(gameboard_->getFieldById(3)).setOwner(players_[2]);
			// }
		}
		++playerId;
	};
	playersStartingAmount_ = i;

	i = 0;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(players_), std::end(players_), g);

	for (std::shared_ptr<Player> player : players_) {
		player->createSprite();
		if (i % 2 == 0) {
			player->setSpriteOffsetX(0.33f);
		} else {
			player->setSpriteOffsetX(0.66f);
		}
		if (i >= 0 && i < 2) {
			player->setSpriteOffsetY(0.33f);
		} else if (i >= 2 && i < 4) {
			player->setSpriteOffsetY(0.66f);
		}
		const sf::Vector2i BOARD_POSITION = gameboard_->getBoardPosition();
		const float HEIGHT_OFFSET = 20.0;
		const unsigned int FIELD_WIDTH =
			std::visit([](Field& field) { return field.getWidth(); }, gameboard_->getFieldById(0));
		const unsigned int FIELD_HEIGHT =
			std::visit([](Field& field) { return field.getHeight(); }, gameboard_->getFieldById(0)) - HEIGHT_OFFSET;
		const float SPRITE_POSITION_X = (float)BOARD_POSITION.x + (float)FIELD_WIDTH * player->getSpriteOffsetX();
		const float SPRITE_POSITION_Y =
			(float)BOARD_POSITION.y + (float)FIELD_HEIGHT * player->getSpriteOffsetY() + (float)HEIGHT_OFFSET;
		player->setSpritePosition(sf::Vector2f(SPRITE_POSITION_X, SPRITE_POSITION_Y));
		++i;
	}
}

void monopolyGameEngine::clearPlayers() {
	players_.clear();
}

void monopolyGameEngine::clearBoard() {
	gameboard_->clearBoard();
}

std::shared_ptr<Board> monopolyGameEngine::getBoard() {
	return gameboard_;
}

std::vector<std::shared_ptr<Player>>& monopolyGameEngine::getPlayers() {
	return players_;
}

void monopolyGameEngine::setPlayerIndexTurn(unsigned int indx) {
	if (indx < PLAYERS_MAX_) {
		playerIndexturn_ = indx;
	}
}

unsigned int monopolyGameEngine::getPlayerIndexTurn() const {
	return playerIndexturn_;
}

void monopolyGameEngine::incPlayerIndexTurn() {
	++playerIndexturn_;
	if (playerIndexturn_ >= players_.size()) {
		playerIndexturn_ = 0;
	}
}

bool monopolyGameEngine::isButtonClicked(std::shared_ptr<Button> button_ptr) {
	if (button_ptr->getIsActive() && button_ptr->getWasReleased()) {
		button_ptr->setWasReleased(false);
		button_ptr->setIsActive(false);
		return true;
	}
	return false;
}

unsigned int monopolyGameEngine::rollDice() const {	 // dices roll for 1-12 move
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 6);
	return dist(gen);
}
void monopolyGameEngine::setTurnState(TurnState newState) {
	turnState_ = newState;
}

TurnState monopolyGameEngine::getTurnState() const {
	return turnState_;
}

unsigned int monopolyGameEngine::getFontSize() const {
	return FONT_SIZE;
}

void monopolyGameEngine::turnInfoTextWorker() {
	turnInfoText_->setString("Turn: Player " + std::to_string(players_[getPlayerIndexTurn()]->getId() + 1));
}

unsigned int monopolyGameEngine::getHouseCount() {
	return house_count_;
}

unsigned int monopolyGameEngine::getHotelCount() {
	return hotel_count_;
}

void monopolyGameEngine::setHouseCount(unsigned int new_count) {
	house_count_ = new_count;
}

void monopolyGameEngine::setHotelCount(unsigned int new_count) {
	hotel_count_ = new_count;
}

void monopolyGameEngine::addHouses(unsigned int added_amount) {
	setHouseCount(house_count_ + added_amount);
}

void monopolyGameEngine::substractHouses(unsigned int substracted_amount) {
	setHouseCount(house_count_ - substracted_amount);
}

void monopolyGameEngine::addHotels(unsigned int added_amount) {
	setHotelCount(hotel_count_ + added_amount);
}

void monopolyGameEngine::substractHotels(unsigned int substracted_amount) {
	setHotelCount(hotel_count_ - substracted_amount);
}

unsigned int monopolyGameEngine::performAuction(unsigned int starting_player, unsigned int auctioned_field_id) {
	unsigned int current_bid = 10;
	std::vector<std::shared_ptr<Player>> players_bidding = players_;
	FieldType field_type = std::visit([] (Field& field) { return field.getType(); }, getBoard()->getFieldById(auctioned_field_id));
	if (field_type == STREET) {
		// Tutaj trzeba wygenerować ekran aukcji
		StreetField auctioned_field = std::get<StreetField>(getBoard()->getFieldById(auctioned_field_id));
	} else if (field_type == STATION) {
		// Tutaj trzeba wygenerować ekran aukcji
		StationField auctioned_field = std::get<StationField>(getBoard()->getFieldById(auctioned_field_id));
	} else if (field_type == UTILITY) {
		// Tutaj trzeba wygenerować ekran aukcji
		UtilityField auctioned_field = std::get<UtilityField>(getBoard()->getFieldById(auctioned_field_id));
	}
	while (players_bidding.size() > 1) {
		// Głowna logika aukcji
	}
	std::shared_ptr<Player> winner = players_bidding[0];
}

bool monopolyGameEngine::groupCompleted(std::vector<unsigned int> player_fields, PropertyField& field) const {
	std::vector<unsigned int> field_group_members = field.getGroupMembers();
	for (auto group_member : field_group_members) {
		if (std::find(player_fields.cbegin(), player_fields.cend(), group_member) == player_fields.cend()) {
			return false;
		}
	}
	return true;
}

unsigned int monopolyGameEngine::calculateGroupFieldsOwned(std::vector<unsigned int> player_fields,
	PropertyField& field) const {
	unsigned int fields_owned = 1;
	std::vector<unsigned int> field_group_members = field.getGroupMembers();
	for (auto group_member : field_group_members) {
		if (std::find(player_fields.cbegin(), player_fields.cend(), group_member) != player_fields.cend()) {
			++fields_owned;
		}
	}
	return fields_owned;
}

bool monopolyGameEngine::isBuildingLegal(std::shared_ptr<Player> builder, StreetField& field) {
	std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
	unsigned int field_houses = field.getHouseNumber();
	if (!field.getIsMortaged() && groupCompleted(builder_ownes, field) && builder->getMoney() > field.getHousePrice() &&
		field_houses < 4 && getHouseCount() > 0) {	// W tym if trzeba będzie dodać kontrolę budynków w puli
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if (field_houses > group_member.getHouseNumber() || group_member.getIsMortaged()) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

bool monopolyGameEngine::isDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field) {
	std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
	unsigned int field_houses = field.getHouseNumber();
	if (!field.getIsMortaged() && groupCompleted(builder_ownes, field) && field_houses > 0) {
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if (field_houses < group_member.getHouseNumber() || group_member.getIsMortaged()) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

bool monopolyGameEngine::isHotelBuildingLegal(std::shared_ptr<Player> builder, StreetField& field) {
	std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
	unsigned int field_houses = field.getHouseNumber();

	if (!field.getIsMortaged() && groupCompleted(builder_ownes, field) &&
		builder->getMoney() >= field.getHotelPrice() && field_houses == 4 && getHotelCount() > 0 &&
		!field.getIsHotel()) {
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if ((group_member.getHouseNumber() < 4 && !group_member.getIsHotel()) || group_member.getIsMortaged()) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

bool monopolyGameEngine::isHotelDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field) {
	std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
	unsigned int field_houses = field.getHouseNumber();
	if (!field.getIsMortaged() && groupCompleted(builder_ownes, field) && field.getIsHotel() && getHouseCount() >= 4) {
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if (group_member.getIsMortaged()) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

unsigned int monopolyGameEngine::calculateRent(unsigned int rolled_val, int pos) {
	unsigned int rent_to_pay;
	FieldType field_type = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	if (field_type == STREET) {
		StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
		unsigned int house_number = field.getHouseNumber();
		std::map<StreetTiers, unsigned int> rent_values = field.getRentValues();
		if (field.getIsMortaged()) {
			rent_to_pay = 0;
		} else if (field.getIsHotel()) {
			rent_to_pay = rent_values[HOTEL];
		} else if (house_number != 0) {
			std::map<unsigned int, StreetTiers> house_number_map = {
				{1, ONE_HOUSE}, {2, TWO_HOUESES}, {3, THREE_HOUSES}, {4, FOUR_HOUSES}};
			StreetTiers rent_tier = house_number_map[house_number];
			rent_to_pay = rent_values[rent_tier];
		} else if (groupCompleted(field.getOwner()->getFiledOwnedId(), field)) {
			rent_to_pay = 2 * rent_values[NO_HOUSES];
		} else {
			rent_to_pay = rent_values[NO_HOUSES];
		}
	} else if (field_type == STATION) {
		StationField field = std::get<StationField>(getBoard()->getFieldById(pos));
		if (field.getIsMortaged()) {
			rent_to_pay = 0;
		} else {
			std::map<StationTiers, unsigned int> rent_values = field.getRentValues();
			std::vector<unsigned int> player_owns = field.getOwner()->getFiledOwnedId();
			const unsigned int stations_owned = calculateGroupFieldsOwned(player_owns, field);
			std::map<unsigned int, StationTiers> stations_number_map = {
				{1, ONE_STATION}, {2, TWO_STATIONS}, {3, THREE_STATIONS}, {4, FOUR_STATIONS}};
			StationTiers rent_tier = stations_number_map[stations_owned];
			rent_to_pay = rent_values[rent_tier];
		}
	} else if (field_type == UTILITY) {
		UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
		if (field.getIsMortaged()) {
			rent_to_pay = 0;
		} else {
			std::map<UtilityTiers, unsigned int> rent_multipliers = field.getRentMultipliers();
			std::vector<unsigned int> player_owns = field.getOwner()->getFiledOwnedId();
			const unsigned int utility_owned = calculateGroupFieldsOwned(player_owns, field);
			std::map<unsigned int, UtilityTiers> utility_number_map = {{1, ONE_UTILITY}, {2, TWO_UTILITIES}};
			UtilityTiers utility_tier = utility_number_map[utility_owned];
			rent_to_pay = rolled_val * rent_multipliers[utility_tier];
		}
	}
	return rent_to_pay;
}

void monopolyGameEngine::movePlayer(unsigned int turnIndex, unsigned int positionIncrement) {
	int oldPos = players_[turnIndex]->getPositon();
	int newPos = (oldPos + positionIncrement) % 40;
	players_[turnIndex]->setPositon(newPos);
	sf::Vector2f newPlayerSpritePos = getUpdatePlayerSpritePosition();
	players_[turnIndex]->setSpritePosition(newPlayerSpritePos);
}

void monopolyGameEngine::sendToJail(unsigned int turnIndex) {
	const unsigned int JAIL_ID = 10;
	players_[turnIndex]->setPositon(JAIL_ID);
	sf::Vector2f newPlayerSpritePos = getUpdatePlayerSpritePosition();
	players_[turnIndex]->setSpritePosition(newPlayerSpritePos);
}

void monopolyGameEngine::handlePassingStart(unsigned int oldPos, unsigned int newPos) {
	if (newPos < oldPos) {	// start passed
		players_[playerIndexturn_]->addMoney(START_PASSING_MONEY_);
	}
}

void monopolyGameEngine::showAllPropertiesWorker() {
	if (isButtonClicked(nextPropertyButton_)) {
		++currentPropertyShowed_;
		if (currentPropertyShowed_ == 40) {
			currentPropertyShowed_ = 1;
		}
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		while (!(field_type == STREET || field_type == STATION || field_type == UTILITY)) {
			++currentPropertyShowed_;
			if (currentPropertyShowed_ == 40) {
				currentPropertyShowed_ = 1;
			}
			field_type = std::visit(
				[](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		}
		clearPropertyData(false);
		showPropertyData(currentPropertyShowed_, false);
	}

	if (isButtonClicked(previousPropertyButton_)) {
		if (currentPropertyShowed_ == 0) {
			currentPropertyShowed_ = 39;
		} else {
			--currentPropertyShowed_;
		}
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		while (!(field_type == STREET || field_type == STATION || field_type == UTILITY)) {
			if (currentPropertyShowed_ == 0) {
				currentPropertyShowed_ = 39;
			} else {
				--currentPropertyShowed_;
			}
			field_type = std::visit(
				[](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		}
		clearPropertyData(false);
		showPropertyData(currentPropertyShowed_, false);
	}
}

void monopolyGameEngine::buildingsManagingWorker() {
	if (isButtonClicked(buyHouseButton_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		if (field_type == STREET) {
			// Ostatnia poprawka - dodałem referencje - sprawdzić czy poprawa
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (isBuildingLegal(players_[playerIndexturn_], field)) {
				players_[playerIndexturn_]->substractMoney(field.getHousePrice());
				field.setHouseNumber(field.getHouseNumber() + 1);
				substractHouses(1);
				std::cout << "House builded!" << std::endl;
			} else {
				notificationAdd(playerIndexturn_, "Unable to buy house");
			}
		} else {
			std::cout << "Can't buy house on a non street field" << std::endl;
		}
	}
	if (isButtonClicked(sellHouseButton_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (isDestroyingLegal(players_[playerIndexturn_], field)) {
				players_[playerIndexturn_]->addMoney(field.getHousePrice() / 2);
				field.setHouseNumber(field.getHouseNumber() - 1);
				addHouses(1);
				std::cout << "House destroyed!" << std::endl;
			} else {
				notificationAdd(playerIndexturn_, "Unable to sell house");
			}
		} else {
			std::cout << "Can't sell house on a non street field" << std::endl;
		}
	}
	if (isButtonClicked(buyHotelButton_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (isHotelBuildingLegal(players_[playerIndexturn_], field)) {
				players_[playerIndexturn_]->substractMoney(field.getHotelPrice());
				field.setIsHotel(true);
				substractHotels(1);
				addHouses(4);
				std::cout << "Hotel builded!" << std::endl;
			} else {
				notificationAdd(playerIndexturn_, "Unable to buy hotel");
			}
		} else {
			std::cout << "Can't buy hotel on a non street field" << std::endl;
		}
	}
	if (isButtonClicked(sellHotelButton_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (isHotelDestroyingLegal(players_[playerIndexturn_], field)) {
				players_[playerIndexturn_]->addMoney(field.getHotelPrice() / 2);
				field.setIsHotel(false);
				addHotels(1);
				substractHouses(4);
				std::cout << "Hotel destroyed!" << std::endl;
			} else {
				notificationAdd(playerIndexturn_, "Unable to sell hotel");
			}
		} else {
			std::cout << "Can't sell hotel on a non street field" << std::endl;
		}
	}
}

void monopolyGameEngine::withdrawWorker() {
	if (isButtonClicked(withdrawButton_)) {	 // player decied to go bankrupt
		notificationAdd(playerIndexturn_, " is busy, can not withdraw right now");
		if (getTurnState() == RollDice || getTurnState() == TurnEnd) {
			setScreenType(WithdrawChoosePlayer);
		}
	}
}

void monopolyGameEngine::monopolyGameWorker() {
	turnInfoTextWorker();
	updateTextPlayersInfo();
	updateAvailableHousesHotelText();
	showAllPropertiesWorker();
	withdrawWorker();
	unsigned int JAIL_BAILOUT = 50;
	static int rolled_val;
	static unsigned int money_to_find;
	static unsigned int double_turns = 0;
	static bool isDouble;

	if (isButtonClicked(bankruptButton_)) {	 // player decied to go bankrupt
		if (makePlayerBankrupt(playerIndexturn_)) {
			notificationAdd(playerIndexturn_, "decided to go bankrupt!");
		}
	}

	switch (getTurnState()) {
		case RollDice: {
			unsigned int player_jail_status = players_[playerIndexturn_]->getJailStatus();
			buildingsManagingWorker();
			if (player_jail_status != 0) {
				jailPayButton_->setIsVisible(true);
			} else {
				jailPayButton_->setIsVisible(false);
			}

			if (player_jail_status != 0 && isButtonClicked(jailPayButton_)) {
				if (players_[playerIndexturn_]->getMoney() < JAIL_PAY_MONEY) {
					std::string notification_msg = "Not enough money for bail out";
					notificationAdd(playerIndexturn_, notification_msg);
				} else {
					players_[playerIndexturn_]->setJailStatus(0);
					players_[playerIndexturn_]->substractMoney(JAIL_BAILOUT);
					std::string notification_msg = "Leaves jail on bailout.";
					notificationAdd(playerIndexturn_, notification_msg);
					jailPayButton_->setIsVisible(false);
				}
			}

			if (isButtonClicked(rollDiceButton_)) {
				unsigned int roll1 = rollDice();
				unsigned int roll2 = rollDice();
				std::string val1 = std::to_string(roll1);
				std::string val2 = std::to_string(roll2);
				rolled_val = roll1 + roll2;
				std::string rol = "Rolled value: ";
				std::string val = std::to_string(rolled_val);
				rolledValueText_->setString(rol + val);

				notificationAdd(playerIndexturn_, rol + val + " -> (" + val1 + ", " + val2 + ")");

				if (player_jail_status == 0) {
					if (roll1 == roll2) {
						isDouble = true;
						double_turns += 1;
					} else {
						isDouble = false;
					}

					if (double_turns == 3) {
						std::string notification_msg = "Went to Jail on doubles";
						sendToJail(playerIndexturn_);
						players_[playerIndexturn_]->setJailStatus(3);
						notificationAdd(playerIndexturn_, notification_msg);

						rollDiceButton_->setIsVisible(false);
						setTurnState(TurnEnd);
					} else {
						int oldPos = players_[playerIndexturn_]->getPositon();
						movePlayer(playerIndexturn_, rolled_val);
						int newPos = players_[playerIndexturn_]->getPositon();
						handlePassingStart(oldPos, newPos);

						rollDiceButton_->setIsVisible(false);
						setTurnState(FieldAction);
					}
				} else {
					if (roll1 == roll2) {
						players_[playerIndexturn_]->setJailStatus(0);

						int oldPos = players_[playerIndexturn_]->getPositon();
						movePlayer(playerIndexturn_, rolled_val);
						int newPos = players_[playerIndexturn_]->getPositon();
						handlePassingStart(oldPos, newPos);

						std::string notification_msg = "Leaving jail on doubles";
						notificationAdd(playerIndexturn_, notification_msg);

						rollDiceButton_->setIsVisible(false);
						jailPayButton_->setIsVisible(false);
						setTurnState(FieldAction);
					} else if (player_jail_status == 1) {
						if (players_[playerIndexturn_]->getMoney() < JAIL_PAY_MONEY) {
							money_to_find = JAIL_PAY_MONEY;
							std::cout << "Gracz ma problemy finansowe " << money_to_find << " do zapłacenia"
									  << std::endl;
							while (players_[playerIndexturn_]->getMoney() < JAIL_PAY_MONEY) {
								// Tutaj ideowo gracz ma być zmuszony do zrobienia wymiany, sprzedania domków/hoteli
								// i/lub zastawienia nieruchomości
								buildingsManagingWorker();
							}
						}

						players_[playerIndexturn_]->setJailStatus(0);
						players_[playerIndexturn_]->substractMoney(JAIL_BAILOUT);

						int oldPos = players_[playerIndexturn_]->getPositon();
						movePlayer(playerIndexturn_, rolled_val);
						int newPos = players_[playerIndexturn_]->getPositon();
						handlePassingStart(oldPos, newPos);

						rollDiceButton_->setIsVisible(false);
						jailPayButton_->setIsVisible(false);
						setTurnState(FieldAction);

						std::string notification_msg = "Player left jail on forced bailout";
						notificationAdd(playerIndexturn_, notification_msg);

					} else {
						players_[playerIndexturn_]->reduceJailStatus();

						rollDiceButton_->setIsVisible(false);
						jailPayButton_->setIsVisible(false);

						setTurnState(TurnEnd);
					}
				}
			}
		} break;
		case FieldAction: {
			int pos = players_[playerIndexturn_]->getPositon();
			FieldType field_type =
				std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));

			std::string textWhereIsPlayer("is on field " + std::visit([](Field& field) { return field.getName(); },
															   getBoard()->getFieldById(pos)));
			notificationAdd(playerIndexturn_, textWhereIsPlayer);

			if (field_type == STREET || field_type == STATION || field_type == UTILITY) {
				std::shared_ptr<Player> owner = nullptr;
				if (field_type == STREET) {
					StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
					owner = field.getOwner();
				} else if (field_type == STATION) {
					StationField field = std::get<StationField>(getBoard()->getFieldById(pos));
					owner = field.getOwner();
				} else {
					UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
					owner = field.getOwner();
				}

				if (owner == nullptr) {
					setTurnState(BuyAction);
					clearPropertyData(true);
					showPropertyData(pos, true);
				} else if (owner->getId() != players_[playerIndexturn_]->getId()) {
					unsigned int rent_to_pay = calculateRent(rolled_val, pos);
					std::string notification("Rent to pay: " + std::to_string(rent_to_pay));
					notificationAdd(playerIndexturn_, notification);
					if (players_[playerIndexturn_]->getMoney() >= rent_to_pay) {
						players_[playerIndexturn_]->substractMoney(rent_to_pay);
						owner->addMoney(rent_to_pay);
						setTurnState(TurnEnd);
					} else {
						money_to_find = rent_to_pay;
						setTurnState(PayRent);
					}
				} else {
					std::cout << "No action - player owns this field" << field_type << std::endl;
					setTurnState(TurnEnd);
				}
			} else if (field_type == TAX) {
				TaxField field = std::get<TaxField>(getBoard()->getFieldById(pos));
				unsigned int tax_to_pay = field.getTaxValue();
				if (players_[playerIndexturn_]->getMoney() >= tax_to_pay) {
					players_[playerIndexturn_]->substractMoney(tax_to_pay);
					setTurnState(TurnEnd);
				} else {
					money_to_find = tax_to_pay;
					setTurnState(PayRent);
				}
			} else if (field_type == GO_TO_JAIL) {
				std::string notification_msg = "Goes to jail via GO TO JAIL";
				notificationAdd(playerIndexturn_, notification_msg);
				sendToJail(playerIndexturn_);
				players_[playerIndexturn_]->setJailStatus(3);
				setTurnState(TurnEnd);
			} else {
				std::cout << "No action" << field_type << std::endl;
				setTurnState(TurnEnd);
			}
		} break;

		case BuyAction: {
			int pos = players_[playerIndexturn_]->getPositon();
			unsigned int price = getFieldPriceByPosition(pos);
			FieldType fieldType =
				std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
			resignBuyFieldButton_->setIsVisible(true);
			buyFieldButton_->setIsVisible(true);
			if (buyFieldButton_->getIsActive()) {
				if (players_[playerIndexturn_]->getMoney() >= price) {	// possible to buy property

					std::string textPlayerBoughtProperty(
						"bought field " +
						std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
					notificationAdd(playerIndexturn_, textPlayerBoughtProperty);

					players_[playerIndexturn_]->substractMoney(price);
					players_[playerIndexturn_]->addFieldOwnedId(pos);
					std::shared_ptr<Player> player_ptr = players_[playerIndexturn_];
					addOwnerToPropertyField(player_ptr, pos);

					buyFieldButton_->setIsActive(false);
					resignBuyFieldButton_->setIsVisible(false);
					buyFieldButton_->setIsVisible(false);
					setTurnState(TurnEnd);
				} else	// NOT possible to buy property
				{
					resignBuyFieldButton_->setIsVisible(false);
					buyFieldButton_->setIsVisible(false);
					setTurnState(TurnEnd);
				}
				buyFieldButton_->setIsActive(false);
			}

			if (resignBuyFieldButton_->getIsActive()) {
				std::string textPlayerResginedProperty(
					"resigned to buy field " +
					std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
				notificationAdd(playerIndexturn_, textPlayerResginedProperty);
				resignBuyFieldButton_->setIsActive(false);
				resignBuyFieldButton_->setIsVisible(false);
				buyFieldButton_->setIsVisible(false);
				setTurnState(TurnEnd);
			}
		} break;

		case PayRent: {
			// Tutaj ideowo gracz ma być zmuszony do zrobienia wymiany, sprzedania domków/hoteli i/lub zastawienia
			// nieruchomości
			std::cout << "Gracz ma problemy finansowe " << money_to_find << " do zapłacenia" << std::endl;
			setTurnState(TurnEnd);
		} break;

		case TurnEnd:
			buildingsManagingWorker();
			if (isButtonClicked(nextTurnButton_)) {
				rollDiceButton_->setIsVisible(true);
				rolledValueText_->setString("");
				resignBuyFieldButton_->setIsVisible(false);
				buyFieldButton_->setIsVisible(false);
				if (!isDouble || players_[playerIndexturn_]->getJailStatus() != 0) {
					double_turns = 0;
					incPlayerIndexTurn();
				}
				setTurnState(RollDice);
			}
			break;
		default:
			break;
	}
}

void monopolyGameEngine::updateAvailableHousesHotelText() {
	availableHousesText_->setString("Houses bank: " + std::to_string(getHouseCount()));
	availableHotelsText_->setString("Hotels bank: " + std::to_string(getHotelCount()));
}

void monopolyGameEngine::createAvailableHousesHotelText() {
	std::shared_ptr<sf::Text> avaHousesText(
		new sf::Text("Houses bank: " + std::to_string(getHouseCount()), getFont(), getFontSize() - 2));
	avaHousesText->setPosition(AVAILABLE_HOUSE_TEXT_POSITION);
	avaHousesText->setColor(sf::Color::Black);
	availableHousesText_ = avaHousesText;
	addText(avaHousesText);

	std::shared_ptr<sf::Text> avaHotelsText(
		new sf::Text("Hotels bank: " + std::to_string(getHotelCount()), getFont(), getFontSize() - 2));
	avaHotelsText->setPosition(sf::Vector2f(AVAILABLE_HOUSE_TEXT_POSITION.x, AVAILABLE_HOUSE_TEXT_POSITION.y + 30));
	avaHotelsText->setColor(sf::Color::Black);
	availableHotelsText_ = avaHotelsText;
	addText(avaHotelsText);
}

sf::Vector2f monopolyGameEngine::getUpdatePlayerSpritePosition() {
	float x_offset;
	float y_offset;
	const float HEIGHT_OFFSET = 20.0;  // If we want piece to go lower we increase this value.
	unsigned int player_position = players_[playerIndexturn_]->getPositon();
	PossibleFields& curr_field = getBoard()->getFieldById(player_position);
	unsigned int curr_field_width = std::visit([](Field& field) { return field.getWidth(); }, curr_field);
	unsigned int curr_field_height = std::visit([](Field& field) { return field.getHeight(); }, curr_field);
	if (player_position <= 10) {
		x_offset = (float)curr_field_width * players_[playerIndexturn_]->getSpriteOffsetX();
		y_offset = (float)curr_field_height * players_[playerIndexturn_]->getSpriteOffsetY() + HEIGHT_OFFSET;
	} else if (player_position > 10 && player_position < 20) {
		x_offset = -(float)curr_field_height * players_[playerIndexturn_]->getSpriteOffsetX() - HEIGHT_OFFSET;
		y_offset = (float)curr_field_width * players_[playerIndexturn_]->getSpriteOffsetY();
	} else if (player_position >= 20 && player_position <= 30) {
		x_offset = -(float)curr_field_width * players_[playerIndexturn_]->getSpriteOffsetX();
		y_offset = -(float)curr_field_height * players_[playerIndexturn_]->getSpriteOffsetY() - HEIGHT_OFFSET;
	} else if (player_position > 30 && player_position <= 40) {
		x_offset = (float)curr_field_height * players_[playerIndexturn_]->getSpriteOffsetX() + HEIGHT_OFFSET;
		y_offset = -(float)curr_field_width * players_[playerIndexturn_]->getSpriteOffsetY();
	}
	float pos_x = (float)std::visit([](Field& field) { return field.getPosition().x; }, curr_field) + x_offset;
	float pos_y = (float)std::visit([](Field& field) { return field.getPosition().y; }, curr_field) + y_offset;
	return sf::Vector2f(pos_x, pos_y);
}

void monopolyGameEngine::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& monopolyGameEngine::getFont() {
	return font_;
}

void monopolyGameEngine::addButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	buttons_.push_back(button);
}

void monopolyGameEngine::addText(std::shared_ptr<sf::Text> textTmp) {
	std::shared_ptr<sf::Text> text = textTmp;
	texts_.push_back(text);
}

std::vector<std::shared_ptr<Button>>& monopolyGameEngine::getButtons() {
	return buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& monopolyGameEngine::getTexts() {
	return texts_;
}

sf::Sprite& monopolyGameEngine::getPropertyDataSprite() {
	return propertyDataSprite_;
}

std::vector<std::shared_ptr<sf::Text>>& monopolyGameEngine::getPropertyDataTexts() {
	return propertyDataTexts_;
}

sf::Sprite& monopolyGameEngine::getAllPropertyDataSprite() {
	return allPropertyDataSprite_;
}

std::vector<std::shared_ptr<sf::Text>>& monopolyGameEngine::getAllPropertyDataTexts() {
	return allPropertyDataTexts_;
}

void monopolyGameEngine::createButtonRollDice() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonRollDice(new Button(Idle, "Roll Dice", buttonSize, getFontSize()));
	buttonRollDice->setFont(getFont());
	buttonRollDice->setPosition(ROLL_DICE_BUTTON_POSITION);
	buttonRollDice->setActiveBackColor(activeButtonBackColor);
	buttonRollDice->setActiveTextColor(activeButtonTextColor);
	buttonRollDice->setInactiveBackColor(inActiveButtonBackColor);
	buttonRollDice->setInactiveTextColor(inActiveButtonTextColor);
	buttonRollDice->setFocusBackColor(FocusButtonBackColor);
	buttonRollDice->setFocusTextColor(FocusButtonTextColor);
	buttonRollDice->setIsClicked(false);
	buttonRollDice->setIsVisible(true);
	buttonRollDice->setIsActive(false);
	buttonRollDice->setIsFocus(false);
	rollDiceButton_ = buttonRollDice;
	addButton(buttonRollDice);
}

void monopolyGameEngine::createTextTurnInfo() {
	std::shared_ptr<sf::Text> turnInfoText(new sf::Text("Turn: ", getFont(), getFontSize()));
	turnInfoText->setPosition(TURN_INFO_TEXT_POSITION);
	turnInfoText->setColor(sf::Color::Black);
	turnInfoText_ = turnInfoText;
	addText(turnInfoText);
}

void monopolyGameEngine::createTextRolledValue() {
	std::shared_ptr<sf::Text> rolledValueText(new sf::Text("", getFont(), getFontSize()));
	rolledValueText->setPosition(ROLLED_VALUE_TEXT_POSITION);
	rolledValueText->setColor(sf::Color::Black);
	rolledValueText_ = rolledValueText;
	addText(rolledValueText);
}

void monopolyGameEngine::createTextPlayersInfo() {
	sf::Vector2f defPos = PLAYERS_INFO_TEXT_POSITION;
	int i = 0;
	for (auto player : players_) {
		int id = player->getId();
		if (i > 0) {
			defPos.x += 180;
		}
		std::shared_ptr<sf::Text> playerText(
			new sf::Text("Player " + std::to_string(id + 1), getFont(), getFontSize()));
		playerText->setPosition(defPos);
		playerText->setColor(player->getColor());
		playerText->setOutlineColor(sf::Color::Black);
		playerText->setOutlineThickness(2);
		addText(playerText);

		std::shared_ptr<sf::Text> playerMoneyText(
			new sf::Text("Money: " + std::to_string(player->getMoney()), getFont(), getFontSize() - 7));
		playerMoneyText->setPosition(sf::Vector2f(defPos.x, defPos.y + 50));
		playerMoneyText->setColor(sf::Color::Black);
		addText(playerMoneyText);

		std::shared_ptr<sf::Text> playerPositionText(
			new sf::Text("Position: " + std::to_string(player->getPositon() + 1), getFont(), getFontSize() - 7));
		playerPositionText->setPosition(sf::Vector2f(defPos.x, defPos.y + 80));
		playerPositionText->setColor(sf::Color::Black);
		addText(playerPositionText);

		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(player->getPositon()));
		std::shared_ptr<sf::Text> playerPositionNameText(new sf::Text(streetName, getFont(), getFontSize() - 7));
		playerPositionNameText->setPosition(sf::Vector2f(defPos.x, defPos.y + 110));
		playerPositionNameText->setColor(sf::Color::Black);
		addText(playerPositionNameText);

		std::vector<std::shared_ptr<sf::Text>> playerInfoTextTmp;
		playerInfoTextTmp.push_back(playerText);
		playerInfoTextTmp.push_back(playerMoneyText);
		playerInfoTextTmp.push_back(playerPositionText);
		playerInfoTextTmp.push_back(playerPositionNameText);
		playerInfoText_[id].push_back(playerText);
		playerInfoText_[id].push_back(playerMoneyText);
		playerInfoText_[id].push_back(playerPositionText);
		playerInfoText_[id].push_back(playerPositionNameText);

		++i;
	}
}

void monopolyGameEngine::updateTextPlayersInfo() {
	bool isPlayerinGame[4] = {false, false, false, false};
	for (auto player : players_) {
		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(player->getPositon()));
		int id = player->getId();
		playerInfoText_[id][1]->setString("Money: " + std::to_string(player->getMoney()));
		playerInfoText_[id][2]->setString("Position: " + std::to_string(player->getPositon() + 1));
		playerInfoText_[id][3]->setString(streetName);
		isPlayerinGame[id] = true;
	}

	for (int i = 0; i < playersStartingAmount_; ++i) {
		if (!isPlayerinGame[i]) {
			int id = players_[i]->getId();
			playerInfoText_[id][1]->setString("Bankrupt");
			playerInfoText_[id][2]->setString("");
			playerInfoText_[id][3]->setString("");
		}
	}
}

void monopolyGameEngine::createTextBidderInfo() {
	std::shared_ptr<sf::Text> bidderInfoText(new sf::Text("Current bidder: ", getFont(), getFontSize()));
	bidderInfoText->setPosition(BIDDER_INFO_TEXT_POSITION);
	bidderInfoText->setColor(sf::Color::Black);
	bidderInfoText_ = bidderInfoText;
	addText(bidderInfoText);
}

void monopolyGameEngine::createTextHighestBidInfo() {
	std::shared_ptr<sf::Text> highestBidText(new sf::Text("Highest bid: ", getFont(), getFontSize()));
	highestBidText->setPosition(HIGHEST_BID_TEXT_POSITION);
	highestBidText->setColor(sf::Color::Black);
	highestBidInfoText_ = highestBidText;
	addText(highestBidText);
}

void monopolyGameEngine::createTextLeadingBidderInfo() {
	std::shared_ptr<sf::Text> leadingBidderText(new sf::Text("Leading bidder: ", getFont(), getFontSize()));
	leadingBidderText->setPosition(LEADING_BIDDER_TEXT_POSITION);
	leadingBidderText->setColor(sf::Color::Black);
	leadingBidderInfoText_ = leadingBidderText;
	addText(leadingBidderText);
}

void monopolyGameEngine::createCurrentOfferBidderInfo() {
	std::shared_ptr<sf::Text> currentOfferText(new sf::Text("Current offer: ", getFont(), getFontSize()));
	currentOfferText->setPosition(CURRENT_OFFER_TEXT_POSITION);
	currentOfferText->setColor(sf::Color::Black);
	currentOfferInfoText_ = currentOfferText;
	addText(currentOfferText);
}

void monopolyGameEngine::createButtonBuyResign() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBuy(new Button(Idle, "Buy", buttonSize, getFontSize()));
	buttonBuy->setFont(getFont());
	buttonBuy->setPosition(BUY_BUTTON_POSITION);
	buttonBuy->setActiveBackColor(activeButtonBackColor);
	buttonBuy->setActiveTextColor(activeButtonTextColor);
	buttonBuy->setInactiveBackColor(inActiveButtonBackColor);
	buttonBuy->setInactiveTextColor(inActiveButtonTextColor);
	buttonBuy->setFocusBackColor(FocusButtonBackColor);
	buttonBuy->setFocusTextColor(FocusButtonTextColor);
	buttonBuy->setIsClicked(false);
	buttonBuy->setIsVisible(false);
	buttonBuy->setIsActive(false);
	buttonBuy->setIsFocus(false);
	buyFieldButton_ = buttonBuy;
	addButton(buttonBuy);

	std::shared_ptr<Button> buttonResign(new Button(Idle, "Resign", buttonSize, getFontSize()));
	buttonResign->setFont(getFont());
	buttonResign->setPosition(sf::Vector2f(BUY_BUTTON_POSITION.x + 140, BUY_BUTTON_POSITION.y));
	buttonResign->setActiveBackColor(activeButtonBackColor);
	buttonResign->setActiveTextColor(activeButtonTextColor);
	buttonResign->setInactiveBackColor(inActiveButtonBackColor);
	buttonResign->setInactiveTextColor(inActiveButtonTextColor);
	buttonResign->setFocusBackColor(FocusButtonBackColor);
	buttonResign->setFocusTextColor(FocusButtonTextColor);
	buttonResign->setIsClicked(false);
	buttonResign->setIsVisible(false);
	buttonResign->setIsActive(false);
	buttonResign->setIsFocus(false);
	resignBuyFieldButton_ = buttonResign;
	addButton(buttonResign);
}

void monopolyGameEngine::createButtonNextProperty() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonNext(new Button(Idle, "Next", buttonSize, getFontSize()));
	buttonNext->setFont(getFont());
	buttonNext->setPosition(NEXT_PROPERTY_BUTTON_POSITION);
	buttonNext->setActiveBackColor(activeButtonBackColor);
	buttonNext->setActiveTextColor(activeButtonTextColor);
	buttonNext->setInactiveBackColor(inActiveButtonBackColor);
	buttonNext->setInactiveTextColor(inActiveButtonTextColor);
	buttonNext->setFocusBackColor(FocusButtonBackColor);
	buttonNext->setFocusTextColor(FocusButtonTextColor);
	buttonNext->setIsClicked(false);
	buttonNext->setIsVisible(true);
	buttonNext->setIsActive(false);
	buttonNext->setIsFocus(false);
	nextPropertyButton_ = buttonNext;
	addButton(buttonNext);
}

void monopolyGameEngine::createButtonPerviousProperty() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonPrev(new Button(Idle, "Previous", buttonSize, getFontSize()));
	buttonPrev->setFont(getFont());
	buttonPrev->setPosition(PREVIOUS_PROPERTY_BUTTON_POSITION);
	buttonPrev->setActiveBackColor(activeButtonBackColor);
	buttonPrev->setActiveTextColor(activeButtonTextColor);
	buttonPrev->setInactiveBackColor(inActiveButtonBackColor);
	buttonPrev->setInactiveTextColor(inActiveButtonTextColor);
	buttonPrev->setFocusBackColor(FocusButtonBackColor);
	buttonPrev->setFocusTextColor(FocusButtonTextColor);
	buttonPrev->setIsClicked(false);
	buttonPrev->setIsVisible(true);
	buttonPrev->setIsActive(false);
	buttonPrev->setIsFocus(false);
	previousPropertyButton_ = buttonPrev;
	addButton(buttonPrev);
}

void monopolyGameEngine::createButtonsBuySellHouseHotel() {
	houseText_ = std::make_shared<sf::Text>("House", getFont(), getFontSize() - 2);
	houseText_->setPosition(HOUSE_TEXT_POSITION);
	houseText_->setColor(sf::Color::Black);
	houseText_->setOrigin(houseText_->getGlobalBounds().getSize() / 2.f + houseText_->getLocalBounds().getPosition());
	hotelText_ = std::make_shared<sf::Text>("Hotel", getFont(), getFontSize() - 2);
	hotelText_->setPosition(HOTEL_TEXT_POSITION);
	hotelText_->setColor(sf::Color::Black);
	hotelText_->setOrigin(hotelText_->getGlobalBounds().getSize() / 2.f + hotelText_->getLocalBounds().getPosition());
	addText(houseText_);
	addText(hotelText_);
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBuyHouse(new Button(Idle, "Buy", buttonSize, getFontSize()));
	buttonBuyHouse->setFont(getFont());
	buttonBuyHouse->setPosition(BUY_HOUSE_BUTTON_POSITION);
	buttonBuyHouse->setActiveBackColor(activeButtonBackColor);
	buttonBuyHouse->setActiveTextColor(activeButtonTextColor);
	buttonBuyHouse->setInactiveBackColor(inActiveButtonBackColor);
	buttonBuyHouse->setInactiveTextColor(inActiveButtonTextColor);
	buttonBuyHouse->setFocusBackColor(FocusButtonBackColor);
	buttonBuyHouse->setFocusTextColor(FocusButtonTextColor);
	buttonBuyHouse->setIsClicked(false);
	buttonBuyHouse->setIsVisible(true);
	buttonBuyHouse->setIsActive(false);
	buttonBuyHouse->setIsFocus(false);
	buyHouseButton_ = buttonBuyHouse;
	addButton(buttonBuyHouse);

	std::shared_ptr<Button> buttonSellHouse(new Button(Idle, "Sell", buttonSize, getFontSize()));
	buttonSellHouse->setFont(getFont());
	buttonSellHouse->setPosition(SELL_HOUSE_BUTTON_POSITION);
	buttonSellHouse->setActiveBackColor(activeButtonBackColor);
	buttonSellHouse->setActiveTextColor(activeButtonTextColor);
	buttonSellHouse->setInactiveBackColor(inActiveButtonBackColor);
	buttonSellHouse->setInactiveTextColor(inActiveButtonTextColor);
	buttonSellHouse->setFocusBackColor(FocusButtonBackColor);
	buttonSellHouse->setFocusTextColor(FocusButtonTextColor);
	buttonSellHouse->setIsClicked(false);
	buttonSellHouse->setIsVisible(true);
	buttonSellHouse->setIsActive(false);
	buttonSellHouse->setIsFocus(false);
	sellHouseButton_ = buttonSellHouse;
	addButton(buttonSellHouse);

	std::shared_ptr<Button> buttonBuyHotel(new Button(Idle, "Buy", buttonSize, getFontSize()));
	buttonBuyHotel->setFont(getFont());
	buttonBuyHotel->setPosition(BUY_HOTEL_BUTTON_POSITION);
	buttonBuyHotel->setActiveBackColor(activeButtonBackColor);
	buttonBuyHotel->setActiveTextColor(activeButtonTextColor);
	buttonBuyHotel->setInactiveBackColor(inActiveButtonBackColor);
	buttonBuyHotel->setInactiveTextColor(inActiveButtonTextColor);
	buttonBuyHotel->setFocusBackColor(FocusButtonBackColor);
	buttonBuyHotel->setFocusTextColor(FocusButtonTextColor);
	buttonBuyHotel->setIsClicked(false);
	buttonBuyHotel->setIsVisible(true);
	buttonBuyHotel->setIsActive(false);
	buttonBuyHotel->setIsFocus(false);
	buyHotelButton_ = buttonBuyHotel;
	addButton(buttonBuyHotel);

	std::shared_ptr<Button> buttonSellHotel(new Button(Idle, "Sell", buttonSize, getFontSize()));
	buttonSellHotel->setFont(getFont());
	buttonSellHotel->setPosition(SELL_HOTEL_BUTTON_POSITION);
	buttonSellHotel->setActiveBackColor(activeButtonBackColor);
	buttonSellHotel->setActiveTextColor(activeButtonTextColor);
	buttonSellHotel->setInactiveBackColor(inActiveButtonBackColor);
	buttonSellHotel->setInactiveTextColor(inActiveButtonTextColor);
	buttonSellHotel->setFocusBackColor(FocusButtonBackColor);
	buttonSellHotel->setFocusTextColor(FocusButtonTextColor);
	buttonSellHotel->setIsClicked(false);
	buttonSellHotel->setIsVisible(true);
	buttonSellHotel->setIsActive(false);
	buttonSellHotel->setIsFocus(false);
	sellHotelButton_ = buttonSellHotel;
	addButton(buttonSellHotel);
}

void monopolyGameEngine::createButtonsBankrupt() {
	sf::Vector2f buttonSize = sf::Vector2f(140, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBankrupt(new Button(Idle, "Go Bankrupt", buttonSize, getFontSize()));
	buttonBankrupt->setFont(getFont());
	buttonBankrupt->setPosition(BANKRUPT_BUTTON_POSITION);
	buttonBankrupt->setActiveBackColor(activeButtonBackColor);
	buttonBankrupt->setActiveTextColor(activeButtonTextColor);
	buttonBankrupt->setInactiveBackColor(inActiveButtonBackColor);
	buttonBankrupt->setInactiveTextColor(inActiveButtonTextColor);
	buttonBankrupt->setFocusBackColor(FocusButtonBackColor);
	buttonBankrupt->setFocusTextColor(FocusButtonTextColor);
	buttonBankrupt->setIsClicked(false);
	buttonBankrupt->setIsVisible(true);
	buttonBankrupt->setIsActive(false);
	buttonBankrupt->setIsFocus(false);
	bankruptButton_ = buttonBankrupt;
	addButton(buttonBankrupt);
}

void monopolyGameEngine::createButtonsNextTurn() {
	sf::Vector2f buttonSize = sf::Vector2f(140, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonNextTurn(new Button(Idle, "Next turn", buttonSize, getFontSize()));
	buttonNextTurn->setFont(getFont());
	buttonNextTurn->setPosition(NEXT_TURN_BUTTON_POSITION);
	buttonNextTurn->setActiveBackColor(activeButtonBackColor);
	buttonNextTurn->setActiveTextColor(activeButtonTextColor);
	buttonNextTurn->setInactiveBackColor(inActiveButtonBackColor);
	buttonNextTurn->setInactiveTextColor(inActiveButtonTextColor);
	buttonNextTurn->setFocusBackColor(FocusButtonBackColor);
	buttonNextTurn->setFocusTextColor(FocusButtonTextColor);
	buttonNextTurn->setIsClicked(false);
	buttonNextTurn->setIsVisible(true);
	buttonNextTurn->setIsActive(false);
	buttonNextTurn->setIsFocus(false);
	nextTurnButton_ = buttonNextTurn;
	addButton(buttonNextTurn);
}

void monopolyGameEngine::createButtonsJailPay() {
	sf::Vector2f buttonSize = sf::Vector2f(210, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonJailPay(
		new Button(Idle, "Pay " + std::to_string(JAIL_PAY_MONEY) + " to leave jail", buttonSize, getFontSize()));
	buttonJailPay->setFont(getFont());
	buttonJailPay->setPosition(JAIL_PAY_BUTTON_POSITION);
	buttonJailPay->setActiveBackColor(activeButtonBackColor);
	buttonJailPay->setActiveTextColor(activeButtonTextColor);
	buttonJailPay->setInactiveBackColor(inActiveButtonBackColor);
	buttonJailPay->setInactiveTextColor(inActiveButtonTextColor);
	buttonJailPay->setFocusBackColor(FocusButtonBackColor);
	buttonJailPay->setFocusTextColor(FocusButtonTextColor);
	buttonJailPay->setIsClicked(false);
	buttonJailPay->setIsVisible(false);
	buttonJailPay->setIsActive(false);
	buttonJailPay->setIsFocus(false);
	jailPayButton_ = buttonJailPay;
	addButton(buttonJailPay);
}

void monopolyGameEngine::createAuctionOfferButtons() {
	Text100_ = std::make_shared<sf::Text>("100", getFont(), getFontSize());
	Text100_->setPosition(TEXT_100_POSITION);
	Text100_->setColor(sf::Color::Black);
	Text100_->setOrigin(Text100_->getGlobalBounds().getSize() / 2.f + Text100_->getLocalBounds().getPosition());
	addText(Text100_);

	Text10_ = std::make_shared<sf::Text>("10", getFont(), getFontSize());
	Text10_->setPosition(TEXT_10_POSITION);
	Text10_->setColor(sf::Color::Black);
	Text10_->setOrigin(Text10_->getGlobalBounds().getSize() / 2.f + Text10_->getLocalBounds().getPosition());
	addText(Text10_);

	Text1_ = std::make_shared<sf::Text>("1", getFont(), getFontSize());
	Text1_->setPosition(TEXT_1_POSITION);
	Text1_->setColor(sf::Color::Black);
	Text1_->setOrigin(Text1_->getGlobalBounds().getSize() / 2.f + Text1_->getLocalBounds().getPosition());
	addText(Text1_);

	sf::Vector2f buttonSize = sf::Vector2f(50, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonAdd100(new Button(Idle, "+", buttonSize, getFontSize() + 10));
	buttonAdd100->setFont(getFont());
	buttonAdd100->setPosition(ADD_100_BUTTON_POSITION);
	buttonAdd100->getText().setPosition(ADD_100_BUTTON_POSITION.x - 12, ADD_100_BUTTON_POSITION.y - 28);
	buttonAdd100->setActiveBackColor(activeButtonBackColor);
	buttonAdd100->setActiveTextColor(activeButtonTextColor);
	buttonAdd100->setInactiveBackColor(inActiveButtonBackColor);
	buttonAdd100->setInactiveTextColor(inActiveButtonTextColor);
	buttonAdd100->setFocusBackColor(FocusButtonBackColor);
	buttonAdd100->setFocusTextColor(FocusButtonTextColor);
	buttonAdd100->setIsClicked(false);
	buttonAdd100->setIsVisible(true);
	buttonAdd100->setIsActive(false);
	buttonAdd100->setIsFocus(false);
	add100ToOfferButton_ = buttonAdd100;
	addButton(buttonAdd100);

	std::shared_ptr<Button> buttonAdd10(new Button(Idle, "+", buttonSize, getFontSize() + 10));
	buttonAdd10->setFont(getFont());
	buttonAdd10->setPosition(ADD_10_BUTTON_POSITION);
	buttonAdd10->getText().setPosition(ADD_10_BUTTON_POSITION.x - 12, ADD_10_BUTTON_POSITION.y - 28);
	buttonAdd10->setActiveBackColor(activeButtonBackColor);
	buttonAdd10->setActiveTextColor(activeButtonTextColor);
	buttonAdd10->setInactiveBackColor(inActiveButtonBackColor);
	buttonAdd10->setInactiveTextColor(inActiveButtonTextColor);
	buttonAdd10->setFocusBackColor(FocusButtonBackColor);
	buttonAdd10->setFocusTextColor(FocusButtonTextColor);
	buttonAdd10->setIsClicked(false);
	buttonAdd10->setIsVisible(true);
	buttonAdd10->setIsActive(false);
	buttonAdd10->setIsFocus(false);
	add10ToOfferButton_ = buttonAdd10;
	addButton(buttonAdd10);

	std::shared_ptr<Button> buttonAdd1(new Button(Idle, "+", buttonSize, getFontSize() + 10));
	buttonAdd1->setFont(getFont());
	buttonAdd1->setPosition(ADD_1_BUTTON_POSITION);
	buttonAdd1->getText().setPosition(ADD_1_BUTTON_POSITION.x - 12, ADD_1_BUTTON_POSITION.y - 28);
	buttonAdd1->setActiveBackColor(activeButtonBackColor);
	buttonAdd1->setActiveTextColor(activeButtonTextColor);
	buttonAdd1->setInactiveBackColor(inActiveButtonBackColor);
	buttonAdd1->setInactiveTextColor(inActiveButtonTextColor);
	buttonAdd1->setFocusBackColor(FocusButtonBackColor);
	buttonAdd1->setFocusTextColor(FocusButtonTextColor);
	buttonAdd1->setIsClicked(false);
	buttonAdd1->setIsVisible(true);
	buttonAdd1->setIsActive(false);
	buttonAdd1->setIsFocus(false);
	add1ToOfferButton_ = buttonAdd1;
	addButton(buttonAdd1);

	std::shared_ptr<Button> buttonSubstract100(new Button(Idle, "-", buttonSize, getFontSize() + 10));
	buttonSubstract100->setFont(getFont());
	buttonSubstract100->setPosition(SUBSTRACT_100_BUTTON_POSITION);
	buttonSubstract100->getText().setPosition(SUBSTRACT_100_BUTTON_POSITION.x - 6, SUBSTRACT_100_BUTTON_POSITION.y - 28);
	buttonSubstract100->setActiveBackColor(activeButtonBackColor);
	buttonSubstract100->setActiveTextColor(activeButtonTextColor);
	buttonSubstract100->setInactiveBackColor(inActiveButtonBackColor);
	buttonSubstract100->setInactiveTextColor(inActiveButtonTextColor);
	buttonSubstract100->setFocusBackColor(FocusButtonBackColor);
	buttonSubstract100->setFocusTextColor(FocusButtonTextColor);
	buttonSubstract100->setIsClicked(false);
	buttonSubstract100->setIsVisible(true);
	buttonSubstract100->setIsActive(false);
	buttonSubstract100->setIsFocus(false);
	substract100FromOfferButton_ = buttonSubstract100;
	addButton(buttonSubstract100);

	std::shared_ptr<Button> buttonSubstract10(new Button(Idle, "-", buttonSize, getFontSize() + 10));
	buttonSubstract10->setFont(getFont());
	buttonSubstract10->setPosition(SUBSTRACT_10_BUTTON_POSITION);
	buttonSubstract10->getText().setPosition(SUBSTRACT_10_BUTTON_POSITION.x - 6, SUBSTRACT_10_BUTTON_POSITION.y - 28);
	buttonSubstract10->setActiveBackColor(activeButtonBackColor);
	buttonSubstract10->setActiveTextColor(activeButtonTextColor);
	buttonSubstract10->setInactiveBackColor(inActiveButtonBackColor);
	buttonSubstract10->setInactiveTextColor(inActiveButtonTextColor);
	buttonSubstract10->setFocusBackColor(FocusButtonBackColor);
	buttonSubstract10->setFocusTextColor(FocusButtonTextColor);
	buttonSubstract10->setIsClicked(false);
	buttonSubstract10->setIsVisible(true);
	buttonSubstract10->setIsActive(false);
	buttonSubstract10->setIsFocus(false);
	substract10FromOfferButton_ = buttonSubstract10;
	addButton(buttonSubstract10);

	std::shared_ptr<Button> buttonSubstract1(new Button(Idle, "-", buttonSize, getFontSize() + 10));
	buttonSubstract1->setFont(getFont());
	buttonSubstract1->setPosition(SUBSTRACT_1_BUTTON_POSITION);
	buttonSubstract1->getText().setPosition(SUBSTRACT_1_BUTTON_POSITION.x - 6, SUBSTRACT_1_BUTTON_POSITION.y - 28);
	buttonSubstract1->setActiveBackColor(activeButtonBackColor);
	buttonSubstract1->setActiveTextColor(activeButtonTextColor);
	buttonSubstract1->setInactiveBackColor(inActiveButtonBackColor);
	buttonSubstract1->setInactiveTextColor(inActiveButtonTextColor);
	buttonSubstract1->setFocusBackColor(FocusButtonBackColor);
	buttonSubstract1->setFocusTextColor(FocusButtonTextColor);
	buttonSubstract1->setIsClicked(false);
	buttonSubstract1->setIsVisible(true);
	buttonSubstract1->setIsActive(false);
	buttonSubstract1->setIsFocus(false);
	substract1FromOfferButton_ = buttonSubstract1;
	addButton(buttonSubstract1);
}

void monopolyGameEngine::createAuctionResignButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonResignAuction(new Button(Idle, "Resign", buttonSize, getFontSize()));
	buttonResignAuction->setFont(getFont());
	buttonResignAuction->setPosition(AUCTION_RESIGN_BUTTON_POSITION);
	buttonResignAuction->setActiveBackColor(activeButtonBackColor);
	buttonResignAuction->setActiveTextColor(activeButtonTextColor);
	buttonResignAuction->setInactiveBackColor(inActiveButtonBackColor);
	buttonResignAuction->setInactiveTextColor(inActiveButtonTextColor);
	buttonResignAuction->setFocusBackColor(FocusButtonBackColor);
	buttonResignAuction->setFocusTextColor(FocusButtonTextColor);
	buttonResignAuction->setIsClicked(false);
	buttonResignAuction->setIsVisible(true);
	buttonResignAuction->setIsActive(false);
	buttonResignAuction->setIsFocus(false);
	auctionResignButton_ = buttonResignAuction;
	addButton(buttonResignAuction);
}

void monopolyGameEngine::createButtonWithdraw() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonWithdraw(new Button(Idle, "Withdraw", buttonSize, getFontSize()));
	buttonWithdraw->setFont(getFont());
	buttonWithdraw->setPosition(WITHDRAW_BUTTON_POSITION);
	buttonWithdraw->setActiveBackColor(activeButtonBackColor);
	buttonWithdraw->setActiveTextColor(activeButtonTextColor);
	buttonWithdraw->setInactiveBackColor(inActiveButtonBackColor);
	buttonWithdraw->setInactiveTextColor(inActiveButtonTextColor);
	buttonWithdraw->setFocusBackColor(FocusButtonBackColor);
	buttonWithdraw->setFocusTextColor(FocusButtonTextColor);
	buttonWithdraw->setIsClicked(false);
	buttonWithdraw->setIsVisible(true);
	buttonWithdraw->setIsActive(false);
	buttonWithdraw->setIsFocus(false);
	withdrawButton_ = buttonWithdraw;
	addButton(buttonWithdraw);
}

void monopolyGameEngine::clearPropertyData(bool isPropertyShownToBuy) {
	if (isPropertyShownToBuy) {
		propertyDataTexts_.clear();
	} else {
		allPropertyDataTexts_.clear();
	}
}

void monopolyGameEngine::showPropertyData(unsigned int pos, bool isPropertyShownToBuy) {
	FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	unsigned int price;
	unsigned int mortage;
	unsigned int housePrice;
	unsigned int hotelPrice;
	unsigned int rents[7];
	if (fieldType == STREET) {
		StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
		std::map<StreetTiers, unsigned int> rentsMap = field.getRentValues();
		rents[0] = rentsMap[NO_HOUSES];
		rents[1] = rentsMap[NO_HOUSES] * 2;
		rents[2] = rentsMap[ONE_HOUSE];
		rents[3] = rentsMap[TWO_HOUESES];
		rents[4] = rentsMap[THREE_HOUSES];
		rents[5] = rentsMap[FOUR_HOUSES];
		rents[6] = rentsMap[HOTEL];
		mortage = field.getMortage();
		housePrice = field.getHousePrice();
		hotelPrice = field.getHotelPrice();
	} else if (fieldType == STATION) {
		StationField field = std::get<StationField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
		std::map<StationTiers, unsigned int> rentsMap = field.getRentValues();
		rents[0] = rentsMap[ONE_STATION];
		rents[1] = rentsMap[TWO_STATIONS];
		rents[2] = rentsMap[THREE_STATIONS];
		rents[3] = rentsMap[FOUR_STATIONS];
		mortage = field.getMortage();
	} else	// fieldType == UTILITY
	{
		UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
		std::map<UtilityTiers, unsigned int> rentsMap = field.getRentMultipliers();
		rents[0] = rentsMap[ONE_UTILITY];
		rents[1] = rentsMap[TWO_UTILITIES];
		mortage = field.getMortage();
	}

	unsigned int width = std::visit([](Field& field) { return field.getWidth(); }, getBoard()->getFieldById(pos));
	unsigned int height = std::visit([](Field& field) { return field.getHeight(); }, getBoard()->getFieldById(pos));
	std::string graphic_path =
		std::visit([](Field& field) { return field.getGraphicPath(); }, getBoard()->getFieldById(pos));

	sf::Vector2f dataPos;
	if (isPropertyShownToBuy) {
		dataPos = PROPERTY_DATA_POSITION;
		if (!propertyDataTexture_.loadFromFile(graphic_path)) {
			propertyDataSprite_.setColor(sf::Color::Green);
		}
		propertyDataSprite_.setTexture(propertyDataTexture_, true);
		sf::Vector2u texture_dim = propertyDataTexture_.getSize();
		float scale_x = (float)width / (float)texture_dim.x;
		float scale_y = (float)height / (float)texture_dim.y;
		const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
		propertyDataSprite_.setScale(SCALE_VECT * PROPERTY_DATA_SCALE);
		propertyDataSprite_.setPosition(dataPos.x, dataPos.y);
		propertyDataSprite_.setRotation(0);

	} else {
		dataPos = ALL_PROPERTY_DATA_POSITION;
		if (!allPropertyDataTexture_.loadFromFile(graphic_path)) {
			allPropertyDataSprite_.setColor(sf::Color::Green);
		}
		allPropertyDataSprite_.setTexture(allPropertyDataTexture_, true);
		sf::Vector2u texture_dim = allPropertyDataTexture_.getSize();
		float scale_x = (float)width / (float)texture_dim.x;
		float scale_y = (float)height / (float)texture_dim.y;
		const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
		allPropertyDataSprite_.setScale(SCALE_VECT * PROPERTY_DATA_SCALE);
		allPropertyDataSprite_.setPosition(dataPos.x, dataPos.y);
		allPropertyDataSprite_.setRotation(0);
	}

	const std::string streetName =
		std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos));
	std::shared_ptr<sf::Text> propertyName(new sf::Text(streetName, getFont(), getFontSize() - 2));
	propertyName->setOrigin(
		propertyName->getGlobalBounds().getSize() / 2.f + propertyName->getLocalBounds().getPosition());
	sf::Sprite dataSprite;
	if (isPropertyShownToBuy) {
		dataSprite = propertyDataSprite_;
	} else {
		dataSprite = allPropertyDataSprite_;
	}
	propertyName->setPosition(
		sf::Vector2f(dataPos.x + (dataSprite.getGlobalBounds().getSize().x / 2.f), dataPos.y + 80));
	propertyName->setColor(sf::Color::Black);

	const float yOffset = 90;
	const float yOffset_step = 25;
	const float rentPricesOffsetX = 180;

	std::shared_ptr<sf::Text> propertyPrice(
		new sf::Text("Price: " + std::to_string(price), getFont(), getFontSize() - 2));
	propertyPrice->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset));
	propertyPrice->setColor(sf::Color::Black);

	std::shared_ptr<sf::Text> propertyMortage(new sf::Text("Mortage:", getFont(), getFontSize() - 2));
	propertyMortage->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 8));
	propertyMortage->setColor(sf::Color::Black);

	std::shared_ptr<sf::Text> propertyMortagePrice(new sf::Text(std::to_string(mortage), getFont(), getFontSize() - 2));
	propertyMortagePrice->setPosition(
		sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 8));
	propertyMortagePrice->setColor(sf::Color::Black);

	if (isPropertyShownToBuy) {
		propertyDataTexts_.push_back(propertyName);
		propertyDataTexts_.push_back(propertyMortagePrice);
		propertyDataTexts_.push_back(propertyPrice);
		propertyDataTexts_.push_back(propertyMortage);
	} else {
		allPropertyDataTexts_.push_back(propertyName);
		allPropertyDataTexts_.push_back(propertyMortagePrice);
		allPropertyDataTexts_.push_back(propertyPrice);
		allPropertyDataTexts_.push_back(propertyMortage);
	}

	if (fieldType == STREET) {
		std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Rent: ", getFont(), getFontSize() - 2));
		propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
		propertyRent1->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent2(new sf::Text("  with color set:", getFont(), getFontSize() - 2));
		propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
		propertyRent2->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent3(new sf::Text("  with 1 house:", getFont(), getFontSize() - 2));
		propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
		propertyRent3->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent4(new sf::Text("  with 2 houses:", getFont(), getFontSize() - 2));
		propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
		propertyRent4->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent5(new sf::Text("  with 3 houses:", getFont(), getFontSize() - 2));
		propertyRent5->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 5));
		propertyRent5->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent6(new sf::Text("  with 4 houses:", getFont(), getFontSize() - 2));
		propertyRent6->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
		propertyRent6->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent7(new sf::Text("  with hotel:", getFont(), getFontSize() - 2));
		propertyRent7->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 7));
		propertyRent7->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHouseCost(new sf::Text("Houses cost:", getFont(), getFontSize() - 2));
		propertyHouseCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 9));
		propertyHouseCost->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHotelCost(new sf::Text("Hotel cost:", getFont(), getFontSize() - 2));
		propertyHotelCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 10));
		propertyHotelCost->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost1(
			new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
		propertyRentCost1->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 1));
		propertyRentCost1->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost2(
			new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
		propertyRentCost2->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 2));
		propertyRentCost2->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost3(
			new sf::Text(std::to_string(rents[2]), getFont(), getFontSize() - 2));
		propertyRentCost3->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 3));
		propertyRentCost3->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost4(
			new sf::Text(std::to_string(rents[3]), getFont(), getFontSize() - 2));
		propertyRentCost4->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
		propertyRentCost4->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost5(
			new sf::Text(std::to_string(rents[4]), getFont(), getFontSize() - 2));
		propertyRentCost5->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 5));
		propertyRentCost5->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost6(
			new sf::Text(std::to_string(rents[5]), getFont(), getFontSize() - 2));
		propertyRentCost6->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 6));
		propertyRentCost6->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost7(
			new sf::Text(std::to_string(rents[6]), getFont(), getFontSize() - 2));
		propertyRentCost7->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 7));
		propertyRentCost7->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHousePrice(
			new sf::Text(std::to_string(housePrice), getFont(), getFontSize() - 2));
		propertyHousePrice->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 9));
		propertyHousePrice->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHotelPrice(
			new sf::Text(std::to_string(hotelPrice), getFont(), getFontSize() - 2));
		propertyHotelPrice->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 10));
		propertyHotelPrice->setColor(sf::Color::Black);

		if (isPropertyShownToBuy) {
			propertyDataTexts_.push_back(propertyRent1);
			propertyDataTexts_.push_back(propertyRent2);
			propertyDataTexts_.push_back(propertyRent3);
			propertyDataTexts_.push_back(propertyRent4);
			propertyDataTexts_.push_back(propertyRent5);
			propertyDataTexts_.push_back(propertyRent6);
			propertyDataTexts_.push_back(propertyRent7);
			propertyDataTexts_.push_back(propertyHouseCost);
			propertyDataTexts_.push_back(propertyRentCost1);
			propertyDataTexts_.push_back(propertyRentCost2);
			propertyDataTexts_.push_back(propertyRentCost3);
			propertyDataTexts_.push_back(propertyRentCost4);
			propertyDataTexts_.push_back(propertyRentCost5);
			propertyDataTexts_.push_back(propertyRentCost6);
			propertyDataTexts_.push_back(propertyRentCost7);
			propertyDataTexts_.push_back(propertyHousePrice);
			propertyDataTexts_.push_back(propertyHotelPrice);
		} else {
			allPropertyDataTexts_.push_back(propertyRent1);
			allPropertyDataTexts_.push_back(propertyRent2);
			allPropertyDataTexts_.push_back(propertyRent3);
			allPropertyDataTexts_.push_back(propertyRent4);
			allPropertyDataTexts_.push_back(propertyRent5);
			allPropertyDataTexts_.push_back(propertyRent6);
			allPropertyDataTexts_.push_back(propertyRent7);
			allPropertyDataTexts_.push_back(propertyHouseCost);
			allPropertyDataTexts_.push_back(propertyHotelCost);
			allPropertyDataTexts_.push_back(propertyRentCost1);
			allPropertyDataTexts_.push_back(propertyRentCost2);
			allPropertyDataTexts_.push_back(propertyRentCost3);
			allPropertyDataTexts_.push_back(propertyRentCost4);
			allPropertyDataTexts_.push_back(propertyRentCost5);
			allPropertyDataTexts_.push_back(propertyRentCost6);
			allPropertyDataTexts_.push_back(propertyRentCost7);
			allPropertyDataTexts_.push_back(propertyHousePrice);
			allPropertyDataTexts_.push_back(propertyHotelPrice);
		}
	} else if (fieldType == STATION) {
		std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Rent: ", getFont(), getFontSize() - 2));
		propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
		propertyRent1->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent2(new sf::Text(" if 2 are owned:", getFont(), getFontSize() - 2));
		propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
		propertyRent2->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent3(new sf::Text(" if 3 are owned:", getFont(), getFontSize() - 2));
		propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
		propertyRent3->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent4(new sf::Text(" if 4 are owned:", getFont(), getFontSize() - 2));
		propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
		propertyRent4->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost1(
			new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
		propertyRentCost1->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 1));
		propertyRentCost1->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost2(
			new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
		propertyRentCost2->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 2));
		propertyRentCost2->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost3(
			new sf::Text(std::to_string(rents[2]), getFont(), getFontSize() - 2));
		propertyRentCost3->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 3));
		propertyRentCost3->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost4(
			new sf::Text(std::to_string(rents[3]), getFont(), getFontSize() - 2));
		propertyRentCost4->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
		propertyRentCost4->setColor(sf::Color::Black);

		if (isPropertyShownToBuy) {
			propertyDataTexts_.push_back(propertyRent1);
			propertyDataTexts_.push_back(propertyRent2);
			propertyDataTexts_.push_back(propertyRent3);
			propertyDataTexts_.push_back(propertyRent4);
			propertyDataTexts_.push_back(propertyRentCost1);
			propertyDataTexts_.push_back(propertyRentCost2);
			propertyDataTexts_.push_back(propertyRentCost3);
			propertyDataTexts_.push_back(propertyRentCost4);
		} else {
			allPropertyDataTexts_.push_back(propertyRent1);
			allPropertyDataTexts_.push_back(propertyRent2);
			allPropertyDataTexts_.push_back(propertyRent3);
			allPropertyDataTexts_.push_back(propertyRent4);
			allPropertyDataTexts_.push_back(propertyRentCost1);
			allPropertyDataTexts_.push_back(propertyRentCost2);
			allPropertyDataTexts_.push_back(propertyRentCost3);
			allPropertyDataTexts_.push_back(propertyRentCost4);
		}
	} else {
		std::shared_ptr<sf::Text> propertyRent(new sf::Text("Rent:", getFont(), getFontSize() - 2));
		propertyRent->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
		propertyRent->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent1(new sf::Text("1 utility is owned: ", getFont(), getFontSize() - 2));
		propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
		propertyRent1->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost1(
			new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
		propertyRentCost1->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
		propertyRentCost1->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent2(new sf::Text("2 utilities are owned: ", getFont(), getFontSize() - 2));
		propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
		propertyRent2->setColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost2(
			new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
		propertyRentCost2->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 7));
		propertyRentCost2->setColor(sf::Color::Black);

		if (isPropertyShownToBuy) {
			propertyDataTexts_.push_back(propertyRent);
			propertyDataTexts_.push_back(propertyRent1);
			propertyDataTexts_.push_back(propertyRent2);
			propertyDataTexts_.push_back(propertyRentCost1);
			propertyDataTexts_.push_back(propertyRentCost2);
		} else {
			allPropertyDataTexts_.push_back(propertyRent);
			allPropertyDataTexts_.push_back(propertyRent1);
			allPropertyDataTexts_.push_back(propertyRent2);
			allPropertyDataTexts_.push_back(propertyRentCost1);
			allPropertyDataTexts_.push_back(propertyRentCost2);
		}
	}
}

unsigned int monopolyGameEngine::getFieldPriceByPosition(unsigned int pos) {
	FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	if (fieldType == STREET) {
		StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
		return field.getPrice();
	} else if (fieldType == STATION) {
		StationField field = std::get<StationField>(getBoard()->getFieldById(pos));
		return field.getPrice();
	} else	// fieldType == UTILITY
	{
		UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
		return field.getPrice();
	}
	return 0;
}

void monopolyGameEngine::addOwnerToPropertyField(std::shared_ptr<Player> player, unsigned int pos) {
	FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	if (fieldType == STREET) {
		std::get<StreetField>(getBoard()->getFieldById(pos)).setOwner(player);
	} else if (fieldType == STATION) {
		std::get<StationField>(getBoard()->getFieldById(pos)).setOwner(player);
	} else	// fieldType == UTILITY
	{
		std::get<UtilityField>(getBoard()->getFieldById(pos)).setOwner(player);
	}
}

NotificationWall& monopolyGameEngine::getNotificationsWall() {
	return notificationsWall_;
}

void monopolyGameEngine::notificationAdd(unsigned int index, std::string text) {
	notificationsWall_.addToWall("Player " + std::to_string(players_[index]->getId() + 1) + ": " + text);
}

sf::Text monopolyGameEngine::getPropertyNameToDraw(sf::Text text, sf::Sprite& sprite, float rotation) {
	text.setFont(getFont());
	text.setCharacterSize(getFontSize() - 15);
	float newXpos = text.getPosition().x;
	float newYpos = text.getPosition().y;
	if (rotation == 0) {
		newXpos += sprite.getGlobalBounds().getSize().x / 2.f;
		newYpos += 25;
	} else if (rotation == 90) {
		newYpos += sprite.getGlobalBounds().getSize().y / 2.f;
		newXpos -= 25;
	} else if (rotation == 180) {
		newXpos -= sprite.getGlobalBounds().getSize().x / 2.f;
		newYpos -= 25;
	} else {
		newYpos -= sprite.getGlobalBounds().getSize().y / 2.f;
		newXpos += 25;
	}
	text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
	text.setPosition(sf::Vector2f(newXpos, newYpos));
	text.setRotation(rotation);
	return text;
}

bool monopolyGameEngine::makePlayerBankrupt(unsigned int playerIndexTurn) {
	// check if index is legal
	// remove player from playif(rotation == 0)
	// {

	// }ers vector
	// remove all ownerships
	return true;
}

sf::Texture& monopolyGameEngine::getHouseTexture() {
	return houseTexture_;
}

sf::Texture& monopolyGameEngine::getHotelTexture() {
	return hotelTexture_;
}

sf::Vector2f& monopolyGameEngine::getHouseSize() {
	return houseSize_;
}

sf::Sprite monopolyGameEngine::getHouseSprite(StreetField& field, unsigned int housesNumber) {
	float rotation = field.getRotation();
	sf::Vector2i position = field.getPosition();
	sf::Sprite house_sprite;
	sf::Texture house_texture = getHouseTexture();
	house_sprite.setTexture(house_texture, true);
	sf::Vector2u texture_dim = house_texture.getSize();
	float scale_x = (float)getHouseSize().x / (float)texture_dim.x;
	float scale_y = (float)getHouseSize().y / (float)texture_dim.y;
	const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
	house_sprite.setScale(SCALE_VECT);
	house_sprite.setRotation(rotation);
	int xOffset = 0;
	int yOffset = 0;
	if (rotation == 0) {
		position.y += 2;
		position.x -= 3;
		xOffset = -7 + house_sprite.getGlobalBounds().getSize().x;
		yOffset = 0;
	} else if (rotation == 90) {
		position.y -= 3;
		position.x -= 2;
		xOffset = 0;
		yOffset = -7 + house_sprite.getGlobalBounds().getSize().y;
	} else if (rotation == 180) {
		position.y -= 2;
		position.x += 3;
		xOffset = 7 - house_sprite.getGlobalBounds().getSize().x;
		yOffset = 0;
	} else if (rotation == 270) {
		position.y += 3;
		position.x += 2;
		xOffset = 0;
		yOffset = 7 - house_sprite.getGlobalBounds().getSize().y;
	}

	position.x += xOffset * (housesNumber - 1);
	position.y += yOffset * (housesNumber - 1);

	house_sprite.setPosition(position.x, position.y);

	return house_sprite;
}

sf::Sprite monopolyGameEngine::getHotelSprite(StreetField& field) {
	float rotation = field.getRotation();
	sf::Vector2i position = field.getPosition();
	sf::Sprite hotel_sprite;
	sf::Texture hotel_texture = getHouseTexture();
	sf::Vector2u texture_dim = hotel_texture.getSize();
	float scale_x = (float)getHouseSize().x / (float)texture_dim.x;
	float scale_y = (float)getHouseSize().y / (float)texture_dim.y;
	const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
	hotel_sprite.setScale(SCALE_VECT);
	hotel_sprite.setRotation(rotation);
	int xOffset = 0;
	int yOffset = 0;
	if (rotation == 0)	// as in hotel sprite calculations to keep the same flow of offsetting
	{
		position.y += 2;
		position.x -= 3;
	} else if (rotation == 90) {
		position.y -= 3;
		position.x -= 2;
	} else if (rotation == 180) {
		position.y -= 2;
		position.x += 3;
	} else if (rotation == 270) {
		position.y += 3;
		position.x += 2;
	}

	hotel_sprite.setPosition(position.x, position.y);

	return hotel_sprite;
}