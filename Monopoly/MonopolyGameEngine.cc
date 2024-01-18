/**
 * @file MonopolyGameEngine.cc
 *
 * @brief Source file of class used to handle whole
 * monopoly game process, turns, actions with players,
 * board etc.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "MonopolyGameEngine.h"

monopolyGameEngine::monopolyGameEngine() {
	turnState_ = ROLL_DICE;
	if (!houseTexture_.loadFromFile("textures_and_fonts/textures/house.png")) {
		// TODO: exception
	}
	if (!hotelTexture_.loadFromFile("textures_and_fonts/textures/hotel.png")) {
		// TODO: exception
	}

	fileLoggerOpen();
}

void monopolyGameEngine::setScreenType(GameScreenType newScreenType) {
	screenType_ = newScreenType;
}

GameScreenType monopolyGameEngine::getScreenType() const {
	return screenType_;
}

void monopolyGameEngine::createBoard() {
	gameboard_ = std::make_shared<Board>(GAMEBOARD_FILE_PATH_);
}

void monopolyGameEngine::createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list) {
	gameTurnByPlayerDone_ = {false, false, false, false};
	gameTurnsGloballyDone_ = 0;
	playersStartingAmount_ = 0;
	int playerId = 0;
	isAiGameOnly_ = true;
	for (auto it : player_settings_list) {
		if (!(it->isNone)) {
			++playersStartingAmount_;
			if (it->isHuman) {
				isAiGameOnly_ = false;
				Player new_player = Player(PLAYER_MONEY_DEFAULT_);
				new_player.setIsAi(!(it->isHuman));
				new_player.setAiLevel(it->level);
				new_player.setId(playerId);
				players_.push_back(std::make_shared<Player>(new_player));
			}
			 else {
				AiPlayer new_player = AiPlayer(PLAYER_MONEY_DEFAULT_);
				new_player.setIsAi(!(it->isHuman));
				new_player.setAiLevel(it->level);
				new_player.setId(playerId);
				players_.push_back(std::make_shared<AiPlayer>(new_player));
			}
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

	int i = 0;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(players_), std::end(players_), g);

	for(int j = 0 ; j < PLAYERS_MAX_ ; ++j)
	{
		playersStartingIds_[j] = 255;	//mark no player in game with this ID
	}

	for (unsigned int j = 0; j < players_.size(); ++j) {
		// save id of starting pplayers to future usage of data display
		playersStartingIds_[j] = players_[j]->getId();
	}

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

std::vector<std::shared_ptr<Player>> monopolyGameEngine::getPlayersResult() {
	return playersBankrupted_;
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

void monopolyGameEngine::setAuctionState(AuctionState newState) {
	auctionState_ = newState;
}

AuctionState monopolyGameEngine::getAuctionState() {
	return auctionState_;
}

unsigned int monopolyGameEngine::getFontSize() const {
	return FONT_SIZE;
}

void monopolyGameEngine::turnInfoTextShow() {
	turnInfoText_->setString("Tura: Gracz " + std::to_string(players_[getPlayerIndexTurn()]->getId() + 1));
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

void monopolyGameEngine::performAuction() {
	static unsigned int current_bid;
	static unsigned int current_offer;
	static unsigned int bidded_property_id;
	static std::shared_ptr<Player> highest_bidder;
	static unsigned int player_bidding;
	static std::vector<std::shared_ptr<Player>> players_bidding;
	switch (getAuctionState()) {
		case Initialization: {
			current_bid = 10;
			bidded_property_id = players_[playerIndexturn_]->getPosition();
			highest_bidder = nullptr;
			player_bidding = playerIndexturn_;
			players_bidding = players_;
			bidderInfoText_->setString(
				"Tura: Gracz " + std::to_string(players_bidding[player_bidding]->getId() + 1));
			highestBidInfoText_->setString("Najwyzsza oferta: " + std::to_string(current_bid));
			if (highest_bidder != nullptr) {
				leadingBidderInfoText_->setString(
					"Prowadzacy: Gracz " + std::to_string(highest_bidder->getId() + 1));
			}
			setAuctionState(PassBiddingTurn);
			break;
		}
		case PassBiddingTurn: {
			current_offer = current_bid + 1;
			setAuctionState(Bidding);
			break;
		}
		case Bidding: {
			// Główna logika aukcji
			if (players_bidding[player_bidding]->getIsAi()) {
				FieldType field_type = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(bidded_property_id));
				unsigned int field_price;
				if (field_type == STREET) {
					StreetField field = std::get<StreetField>(getBoard()->getFieldById(bidded_property_id));
					field_price = field.getPrice();
				} else if (field_type == STATION) {
					StationField field = std::get<StationField>(getBoard()->getFieldById(bidded_property_id));
					field_price = field.getPrice();
				} else if (field_type == UTILITY) {
					UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(bidded_property_id));
					field_price = field.getPrice();
				}

				players_bidding[player_bidding]->getAdapter().setTurn(player_bidding);
				players_bidding[player_bidding]->getAdapter().setSelectionState(bidded_property_id, 1);

				current_offer = players_bidding[player_bidding]->decideAuctionBid(field_price);

				if(current_offer > players_bidding[player_bidding]->getMoney()) {
					current_offer = players_bidding[player_bidding]->getMoney();
				}

				players_bidding[player_bidding]->getAdapter().setSelectionState(bidded_property_id, 1);
			}
			if ((isButtonClicked(auctionBidButton_) || players_bidding[player_bidding]->getIsAi()) && current_offer > current_bid) {
				highest_bidder = players_bidding[player_bidding];
				current_bid = current_offer;
				if (player_bidding == players_bidding.size() - 1) {
					player_bidding = 0;
				} else {
					++player_bidding;
				}
				bidderInfoText_->setString(
					"Tura: Gracz " + std::to_string(players_bidding[player_bidding]->getId() + 1));
				highestBidInfoText_->setString("Najwyzsza oferta: " + std::to_string(current_bid));
				if (highest_bidder != nullptr) {
					leadingBidderInfoText_->setString(
						"Prowadzacy: Gracz " + std::to_string(highest_bidder->getId() + 1));
				}
				setAuctionState(PassBiddingTurn);
			}
			if (isButtonClicked(auctionResignButton_) || players_bidding[player_bidding]->getMoney() < current_bid || (players_bidding[player_bidding]->getIsAi() && current_offer <= current_bid)) {
				unsigned int i = 0;
				for (auto it = players_bidding.begin(); it != players_bidding.end(); ++it) {
					if (i == player_bidding) {
						players_bidding.erase(it);
						if (player_bidding == players_bidding.size()) {
							player_bidding = 0;
						}
						break;
					}
					++i;
				}
				bidderInfoText_->setString(
					"Tura: Gracz " + std::to_string(players_bidding[player_bidding]->getId() + 1));
			}
			if (isButtonClicked(add1ToOfferButton_)) {
				if (current_offer + 1 <= players_bidding[player_bidding]->getMoney()) {
					current_offer += 1;
				} else {
					current_offer = players_bidding[player_bidding]->getMoney();
				}
			}
			if (isButtonClicked(add10ToOfferButton_)) {
				if (current_offer + 10 <= players_bidding[player_bidding]->getMoney()) {
					current_offer += 10;
				} else {
					current_offer = players_bidding[player_bidding]->getMoney();
				}
			}
			if (isButtonClicked(add100ToOfferButton_)) {
				if (current_offer + 100 <= players_bidding[player_bidding]->getMoney()) {
					current_offer += 100;
				} else {
					current_offer = players_bidding[player_bidding]->getMoney();
				}
			}
			if (isButtonClicked(substract1FromOfferButton_)) {
				if (current_offer - 1 >= current_bid + 1 && current_offer > 1) {
					current_offer -= 1;
				} else {
					current_offer = current_bid + 1;
				}
			}
			if (isButtonClicked(substract10FromOfferButton_)) {
				if (current_offer - 10 >= current_bid + 1 && current_offer > 10) {
					current_offer -= 10;
				} else {
					current_offer = current_bid + 1;
				}
			}
			if (isButtonClicked(substract100FromOfferButton_)) {
				if (current_offer - 100 >= current_bid + 1 && current_offer > 100) {
					current_offer -= 100;
				} else {
					current_offer = current_bid + 1;
				}
			}
			currentOfferInfoText_->setString("Aktualna oferta: " + std::to_string(current_offer));

			if ((players_bidding.size() == 1 && highest_bidder != nullptr) || players_bidding.size() == 0) {
				setAuctionState(Ending);
			}
			break;
		}
		case Ending: {
			if (highest_bidder != nullptr) {
				std::shared_ptr<Player> winner = players_bidding[0];
				addOwnerToPropertyField(winner, bidded_property_id);
				winner->addFieldOwnedId(bidded_property_id);
				winner->substractMoney(current_bid);
			} else {
				notificationAdd(playerIndexturn_, "Aukcja - brak zwyciezcy!");
			}
			current_bid = 10;
			highest_bidder = nullptr;
			player_bidding = playerIndexturn_;
			setAuctionState(NoAuction);
		} break;
		default:
			break;
	}
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

bool monopolyGameEngine::isBuildingLegal(std::shared_ptr<Player> builder, StreetField field) {
	std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
	unsigned int field_houses = field.getHouseNumber();
	if (!field.getIsMortgaged() && groupCompleted(builder_ownes, field) &&
		builder->getMoney() > field.getHousePrice() && field_houses < 4 &&
		getHouseCount() > 0) {	// W tym if trzeba będzie dodać kontrolę budynków w puli
		for (unsigned int i = 0; i < field.getGroupMembers().size(); ++i) {
			unsigned int group_member_id = field.getGroupMembers()[i];
			PossibleFields& group_member_variant = getBoard()->getFieldById(field.getGroupMembers()[i]);
			StreetField& group_member = std::get<StreetField>(group_member_variant);
			if (field_houses > group_member.getHouseNumber() || group_member.getIsMortgaged()) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

bool monopolyGameEngine::isDestroyingLegal(std::shared_ptr<Player> builder, StreetField field) {
	std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
	unsigned int field_houses = field.getHouseNumber();
	if (!field.getIsMortgaged() && groupCompleted(builder_ownes, field) && field_houses > 0) {
		for (unsigned int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if (field_houses < group_member.getHouseNumber() || group_member.getIsMortgaged()) {
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

	if (!field.getIsMortgaged() && groupCompleted(builder_ownes, field) &&
		builder->getMoney() >= field.getHotelPrice() && field_houses == 4 && getHotelCount() > 0 &&
		!field.getIsHotel()) {
		for (unsigned int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if ((group_member.getHouseNumber() < 4 && !group_member.getIsHotel()) || group_member.getIsMortgaged()) {
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
	if (!field.getIsMortgaged() && groupCompleted(builder_ownes, field) && field.getIsHotel() && getHouseCount() >= 4) {
		for (unsigned int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if (group_member.getIsMortgaged()) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

bool monopolyGameEngine::colorGroupEmpty(std::shared_ptr<Player> mortaging, StreetField& field) {
	std::vector<unsigned int> mortaging_ownes = mortaging->getFiledOwnedId();
	if (groupCompleted(mortaging_ownes, field)) {
		unsigned int field_houses = field.getHouseNumber();
		if (field_houses == 0) {
			for (unsigned int i = 0; i < field.getGroupMembers().size(); ++i) {
				StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
				if (group_member.getHouseNumber() != 0) {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
}

unsigned int monopolyGameEngine::calculateRent(unsigned int rolled_val, int pos) {
	unsigned int rent_to_pay = 0;
	FieldType field_type = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	if (field_type == STREET) {
		StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
		unsigned int house_number = field.getHouseNumber();
		std::map<StreetTiers, unsigned int> rent_values = field.getRentValues();
		if (field.getIsMortgaged()) {
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
		if (field.getIsMortgaged()) {
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
		if (field.getIsMortgaged()) {
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
	int oldPos = players_[turnIndex]->getPosition();
	int newPos = (oldPos + positionIncrement) % 40;
	players_[turnIndex]->setPosition(newPos);
	sf::Vector2f newPlayerSpritePos = getUpdatePlayerSpritePosition();
	players_[turnIndex]->setSpritePosition(newPlayerSpritePos);
}

void monopolyGameEngine::sendToJail(unsigned int turnIndex) {
	const unsigned int JAIL_ID = 10;
	players_[turnIndex]->setPosition(JAIL_ID);
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
	if (isButtonClicked(MortgageButton_)) {
		std::shared_ptr<Player> curr_player = players_[playerIndexturn_];
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (field.getOwner() == curr_player && !field.getIsMortgaged() && colorGroupEmpty(curr_player, field)) {
				curr_player->addMoney(field.getMortgage());
				field.setIsMortgaged(true);
				notificationAdd(playerIndexturn_, "Mortgaged field " + field.getName());
			} else {
				notificationAdd(playerIndexturn_, "Unable to Mortgage field " + field.getName());
			}
		} else if (field_type == STATION) {
			StationField& field = std::get<StationField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (field.getOwner() == curr_player && !field.getIsMortgaged()) {
				curr_player->addMoney(field.getMortgage());
				field.setIsMortgaged(true);
				notificationAdd(playerIndexturn_, "Mortgaged field " + field.getName());
			} else {
				notificationAdd(playerIndexturn_, "Unable to Mortgage field " + field.getName());
			}
		} else if (field_type == UTILITY) {
			UtilityField& field = std::get<UtilityField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (field.getOwner() == curr_player && !field.getIsMortgaged()) {
				curr_player->addMoney(field.getMortgage());
				field.setIsMortgaged(true);
				notificationAdd(playerIndexturn_, "Mortgaged field " + field.getName());
			} else {
				notificationAdd(playerIndexturn_, "Unable to Mortgage field " + field.getName());
			}
		}
	}
	if (isButtonClicked(unMortgageButton_)) {
		std::shared_ptr<Player> curr_player = players_[playerIndexturn_];
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(currentPropertyShowed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (field.getOwner() == curr_player && field.getIsMortgaged() &&
				curr_player->getMoney() >= field.getUnmortgageValue()) {
				curr_player->substractMoney(field.getUnmortgageValue());
				field.setIsMortgaged(false);
				notificationAdd(playerIndexturn_, "UnMortgaged field " + field.getName());
			} else {
				notificationAdd(playerIndexturn_, "Unable to unMortgage field " + field.getName());
			}
		} else if (field_type == STATION) {
			StationField& field = std::get<StationField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (field.getOwner() == curr_player && field.getIsMortgaged() &&
				curr_player->getMoney() >= field.getUnmortgageValue()) {
				curr_player->substractMoney(field.getUnmortgageValue());
				field.setIsMortgaged(false);
				notificationAdd(playerIndexturn_, "UnMortgaged field " + field.getName());
			} else {
				notificationAdd(playerIndexturn_, "Unable to unMortgage field " + field.getName());
			}
		} else if (field_type == UTILITY) {
			UtilityField& field = std::get<UtilityField>(getBoard()->getFieldById(currentPropertyShowed_));
			if (field.getOwner() == curr_player && field.getIsMortgaged() &&
				curr_player->getMoney() >= field.getUnmortgageValue()) {
				curr_player->substractMoney(field.getUnmortgageValue());
				field.setIsMortgaged(false);
				notificationAdd(playerIndexturn_, "UnMortgaged field " + field.getName());
			} else {
				notificationAdd(playerIndexturn_, "Unable to unMortgage field " + field.getName());
			}
		}
	}
}

void monopolyGameEngine::aiBuildingsMangingWorker() {
	std::vector<std::vector<unsigned int>> SETS = {
		{1, 3},
		{6, 8, 9},
		{11, 13, 14},
		{16, 18, 19},
		{21, 23, 24},
		{26, 27, 29},
		{31, 32, 34},
		{37, 39}
	};
	std::shared_ptr<Player> curr_player = players_[playerIndexturn_];
	std::vector<unsigned int> fields_owned = curr_player->getFiledOwnedId();
	unsigned int owned_amount = fields_owned.size();

	for (unsigned int set = 0; set < SETS.size(); ++set) {
		if(std::includes(fields_owned.begin(), fields_owned.end(), SETS[set].begin(), SETS[set].end())) {
			unsigned int houses_builded = 0;
			for (unsigned int i = 0; i < SETS[set].size(); ++i) {
				StreetField field = std::get<StreetField>(gameboard_->getFieldById(SETS[set][i]));
				if (field.getIsHotel()) {
					houses_builded += 5;
				} else {
					houses_builded += field.getHouseNumber();
				}
			}

			unsigned int max_sell = houses_builded;
			unsigned int player_id = curr_player->getId();
			curr_player->getAdapter().setTurn(player_id);
			curr_player->getAdapter().setSelectionState(SETS[set][0], 1);

			unsigned int decision = curr_player->decideSellHouse();
			decision = std::min(decision, max_sell);

			curr_player->getAdapter().setSelectionState(SETS[set][0], 0);

			unsigned int last = 2;

			if(set == 0 || set == 7) {
				last = 1;
			}

			for (unsigned int house = 0; house < decision; ++house) {
				// find field with highest amount of houses
				unsigned int highest_field_id = 0;
				StreetField highest_field = std::get<StreetField>(gameboard_->getFieldById(SETS[set][highest_field_id]));

				for (unsigned int j = 0; j <= last; ++j) {
					StreetField field_checked = std::get<StreetField>(gameboard_->getFieldById(SETS[set][j]));
					if (!highest_field.getIsHotel() && field_checked.getIsHotel()) {
						highest_field = field_checked;
						highest_field_id = j;
					} else if (highest_field.getHouseNumber() < field_checked.getHouseNumber()) {
						highest_field = field_checked;
						highest_field_id = j;
					}
				}

				StreetField& field_to_build = std::get<StreetField>(gameboard_->getFieldById(SETS[set][highest_field_id]));

				if(isHotelDestroyingLegal(curr_player, field_to_build)) {
					curr_player->addMoney(field_to_build.getHotelPrice() / 2);
					field_to_build.setIsHotel(false);
					addHotels(1);
					substractHouses(4);
					notificationAdd(playerIndexturn_, "Hotel zburzony na polu " + field_to_build.getName());
				} else if (isDestroyingLegal(curr_player, field_to_build)) {
					curr_player->addMoney(field_to_build.getHousePrice() / 2);
					field_to_build.setHouseNumber(field_to_build.getHouseNumber() - 1);
					addHouses(1);
					notificationAdd(playerIndexturn_, "Dom zburzony na polu " + field_to_build.getName());
				} else {
					house = decision;
				}
			}
		}
	}

	for (unsigned int set = 0; set < SETS.size(); ++set) {
		if (std::includes(fields_owned.begin(), fields_owned.end(), SETS[set].begin(), SETS[set].end())) {
			unsigned int max_houses = 10;
			unsigned int houses_builded = 0;
			for (unsigned int i = 0; i < SETS[set].size(); ++i) {
				StreetField field = std::get<StreetField>(gameboard_->getFieldById(SETS[set][i]));
				if (field.getIsHotel()) {
					houses_builded += 5;
				} else {
					houses_builded += field.getHouseNumber();
				}
			}

			if (set != 0 && set != 7) {
				max_houses = 15;
			}

			unsigned int house_price = std::get<StreetField>(gameboard_->getFieldById(SETS[set][0])).getHousePrice();

			unsigned int max_build = max_houses - houses_builded;
			unsigned int afford_max = (unsigned int) floor(curr_player->getMoney() / (float)house_price);

			max_build = std::min(max_build, afford_max);

			unsigned int player_id = curr_player->getId();
			curr_player->getAdapter().setTurn(player_id);
			curr_player->getAdapter().setSelectionState(SETS[set][0], 1);

			unsigned int decision = curr_player->decideBuildHouse();
			decision = std::min(decision, max_build);

			curr_player->getAdapter().setSelectionState(SETS[set][0], 0);

			unsigned int last = 2;

			if(set == 0 || set == 7) {
				last = 1;
			}

			for (unsigned int house = 0; house < decision; ++house) {
				// find field with smallest amount of houses
				unsigned int lowest_field_id = last;
				StreetField lowest_field = std::get<StreetField>(gameboard_->getFieldById(SETS[set][lowest_field_id]));

				for (int j = last - 1; j >= 0; --j) {
					StreetField field_checked = std::get<StreetField>(gameboard_->getFieldById(SETS[set][j]));
					if (lowest_field.getIsHotel() && !field_checked.getIsHotel()) {
						lowest_field = field_checked;
						lowest_field_id = j;
					} else if (lowest_field.getHouseNumber() > field_checked.getHouseNumber()) {
						lowest_field = field_checked;
						lowest_field_id = j;
					}
				}

				StreetField& field_to_build = std::get<StreetField>(gameboard_->getFieldById(SETS[set][lowest_field_id]));

				if (isHotelBuildingLegal(curr_player, field_to_build)) {
					players_[playerIndexturn_]->substractMoney(field_to_build.getHotelPrice());
					field_to_build.setIsHotel(true);
					substractHotels(1);
					addHouses(4);
					notificationAdd(playerIndexturn_, "Hotel zbudowany na polu " + field_to_build.getName());
				} else if (isBuildingLegal(curr_player, lowest_field)) {
					players_[playerIndexturn_]->substractMoney(field_to_build.getHousePrice());
					field_to_build.setHouseNumber(field_to_build.getHouseNumber() + 1);
					substractHouses(1);
					notificationAdd(playerIndexturn_, "Dom zbudowany na polu " + field_to_build.getName());
				} else {
					house = decision;
				}
			}
		}
	}

	for (unsigned int i = 0; i < owned_amount; ++i) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(fields_owned[i]));
		if (field_type == STREET) {
			StreetField field = std::get<StreetField>(getBoard()->getFieldById(fields_owned[i]));
			if (field.getIsMortgaged()) {
				unsigned int unmortgage_value = field.getUnmortgageValue();
				if (unmortgage_value <= curr_player->getMoney()) {
					unsigned int player_id = curr_player->getId();
					curr_player->getAdapter().setTurn(player_id);
					curr_player->getAdapter().setSelectionState(field.getId(), 1);
					Decision unmortgage_decision = curr_player->decideUnmortgage(field.getId());
					curr_player->getAdapter().setSelectionState(field.getId(), 0);

					if (unmortgage_decision == YES) {
						curr_player->substractMoney(field.getUnmortgageValue());
						field.setIsMortgaged(false);
						notificationAdd(playerIndexturn_, "UnMortgaged field " + field.getName());
					}
				}
			} else if (colorGroupEmpty(curr_player, field)){
				unsigned int player_id = curr_player->getId();
				curr_player->getAdapter().setTurn(player_id);
				curr_player->getAdapter().setSelectionState(field.getId(), 1);
				Decision mortgage_decision = curr_player->decideMortgage(field.getId());
				curr_player->getAdapter().setSelectionState(field.getId(), 0);

				if (mortgage_decision == YES) {
					curr_player->addMoney(field.getMortgage());
					field.setIsMortgaged(true);
					notificationAdd(playerIndexturn_, "Mortgaged field " + field.getName());
				}
			}
		} else if (field_type == STATION) {
			StationField field = std::get<StationField>(getBoard()->getFieldById(fields_owned[i]));
			if (field.getIsMortgaged()) {
				unsigned int unmortgage_value = field.getUnmortgageValue();
				if (unmortgage_value <= curr_player->getMoney()) {
					unsigned int player_id = curr_player->getId();
					curr_player->getAdapter().setTurn(player_id);
					curr_player->getAdapter().setSelectionState(field.getId(), 1);
					Decision unmortgage_decision = curr_player->decideUnmortgage(field.getId());
					curr_player->getAdapter().setSelectionState(field.getId(), 0);

					if (unmortgage_decision == YES) {
						curr_player->substractMoney(field.getUnmortgageValue());
						field.setIsMortgaged(false);
						notificationAdd(playerIndexturn_, "UnMortgaged field " + field.getName());
					}
				}
			} else {
				unsigned int player_id = curr_player->getId();
				curr_player->getAdapter().setTurn(player_id);
				curr_player->getAdapter().setSelectionState(field.getId(), 1);
				Decision mortgage_decision = curr_player->decideMortgage(field.getId());
				curr_player->getAdapter().setSelectionState(field.getId(), 0);

				if (mortgage_decision == YES) {
					curr_player->addMoney(field.getMortgage());
					field.setIsMortgaged(true);
					notificationAdd(playerIndexturn_, "Mortgaged field " + field.getName());
				}
			}
		} else if (field_type == UTILITY) {
			UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(fields_owned[i]));
			if (field.getIsMortgaged()) {
				unsigned int unmortgage_value = field.getUnmortgageValue();
				if (unmortgage_value <= curr_player->getMoney()) {
					unsigned int player_id = curr_player->getId();
					curr_player->getAdapter().setTurn(player_id);
					curr_player->getAdapter().setSelectionState(field.getId(), 1);
					Decision unmortgage_decision = curr_player->decideUnmortgage(field.getId());
					curr_player->getAdapter().setSelectionState(field.getId(), 0);

					if (unmortgage_decision == YES) {
						curr_player->substractMoney(field.getUnmortgageValue());
						field.setIsMortgaged(false);
						notificationAdd(playerIndexturn_, "UnMortgaged field " + field.getName());
					}
				}
			} else {
				unsigned int player_id = curr_player->getId();
				curr_player->getAdapter().setTurn(player_id);
				curr_player->getAdapter().setSelectionState(field.getId(), 1);
				Decision mortgage_decision = curr_player->decideMortgage(field.getId());
				curr_player->getAdapter().setSelectionState(field.getId(), 0);

				if (mortgage_decision == YES) {
					curr_player->addMoney(field.getMortgage());
					field.setIsMortgaged(true);
					notificationAdd(playerIndexturn_, "Mortgaged field " + field.getName());
				}
			}
		}
	}
}

void monopolyGameEngine::boardToAuctionSwitchHandler(bool is_auction) {
	rollDiceButton_->setIsVisible(!is_auction);
	buyFieldButton_->setIsVisible(!is_auction);
	resignBuyFieldButton_->setIsVisible(!is_auction);
	nextPropertyButton_->setIsVisible(!is_auction);
	previousPropertyButton_->setIsVisible(!is_auction);
	buyHouseButton_->setIsVisible(!is_auction);
	sellHouseButton_->setIsVisible(!is_auction);
	buyHotelButton_->setIsVisible(!is_auction);
	sellHotelButton_->setIsVisible(!is_auction);
	bankruptButton_->setIsVisible(!is_auction);
	nextTurnButton_->setIsVisible(!is_auction);
	withdrawButton_->setIsVisible(!is_auction);

	if (is_auction) {
		PROPERTY_DATA_POSITION = sf::Vector2f(200, 300);
	} else {
		PROPERTY_DATA_POSITION = sf::Vector2f(910, 260);
	}

	clearPropertyData(true);
	showPropertyData(players_[playerIndexturn_]->getPosition(), true);

	add1ToOfferButton_->setIsVisible(is_auction);
	add10ToOfferButton_->setIsVisible(is_auction);
	add100ToOfferButton_->setIsVisible(is_auction);
	substract1FromOfferButton_->setIsVisible(is_auction);
	substract10FromOfferButton_->setIsVisible(is_auction);
	substract100FromOfferButton_->setIsVisible(is_auction);
	auctionBidButton_->setIsVisible(is_auction);
	auctionResignButton_->setIsVisible(is_auction);
}

void monopolyGameEngine::withdrawWorker() {
	if (isButtonClicked(withdrawButton_)) {	 // player decied to go bankrupt
		if (getTurnState() == ROLL_DICE || getTurnState() == TURN_END || getTurnState() == PAY_RENT) {
			getWithdraw().setTurnState(getTurnState());
			setTurnState(WITHDRAW_ONGOING);
			setScreenType(WITHDRAW_CHOOSE_PLAYER);
			getWithdraw().setChooseScreenVisible(true);
			getWithdraw().setPlayer1ToWithdraw(players_[playerIndexturn_]);
			getWithdraw().getPlayer1Button()->setIsVisible(false);
			getWithdraw().getPlayer2Button()->setIsVisible(false);
			getWithdraw().getPlayer3Button()->setIsVisible(false);
			getWithdraw().getPlayer4Button()->setIsVisible(false);
			for (auto player_ptr : getPlayers()) {
				if (player_ptr->getId() == 0 && player_ptr->getId() != players_[playerIndexturn_]->getId()) {
					getWithdraw().getPlayer1Button()->setIsVisible(true);
				} else if (player_ptr->getId() == 1 && player_ptr->getId() != players_[playerIndexturn_]->getId()) {
					getWithdraw().getPlayer2Button()->setIsVisible(true);
				} else if (player_ptr->getId() == 2 && player_ptr->getId() != players_[playerIndexturn_]->getId()) {
					getWithdraw().getPlayer3Button()->setIsVisible(true);
				} else if (player_ptr->getId() == 3 && player_ptr->getId() != players_[playerIndexturn_]->getId()) {
					getWithdraw().getPlayer4Button()->setIsVisible(true);
				}
			}
		} else {
			notificationAdd(playerIndexturn_, " is busy, can not withdraw right now");
		}
	}
}

bool monopolyGameEngine::monopolyGameWorker() {
	try {
		updateTextPlayersInfo();
		updateAvailableHousesHotelText();
		showAllPropertiesWorker();
		withdrawWorker();
		unsigned int JAIL_BAILOUT = 50;
		static int rolled_val;
		static unsigned int money_to_find;
		static std::vector<std::shared_ptr<Player>> players_to_pay_rent;
		static bool bank_pay_rent = false;
		static unsigned int double_turns = 0;
		static bool isDouble;
		static bool playerChanged = true;
		static bool playerBankrutedNow = false;
		static bool ai_bankrupted = false;

		if (isButtonClicked(bankruptButton_) || (players_[playerIndexturn_]->getIsAi() && ai_bankrupted)) {	 // player decied to go bankrupt
			playerBankrutedNow = true;
			notificationAdd(playerIndexturn_, "decided to go bankrupt!");
			rollDiceButton_->setIsVisible(false);
			buyFieldButton_->setIsVisible(false);
			resignBuyFieldButton_->setIsVisible(false);
			if (players_[playerIndexturn_]->getIsAi()) {
				ai_bankrupted = false;
			}
			setTurnState(TURN_END);	// for next player
		}

		switch (getTurnState()) {
			case ROLL_DICE: {
				playerBankrutedNow = false;
				unsigned int player_jail_status = players_[playerIndexturn_]->getJailStatus();
				if (!players_[playerIndexturn_]->getIsAi()) {
					buildingsManagingWorker();
				} else {
					aiBuildingsMangingWorker();
				}
				if (player_jail_status != 0) {
					jailPayButton_->setIsVisible(true);
				} else {
					jailPayButton_->setIsVisible(false);
				}

				if (player_jail_status != 0) {
					JailDecision decision;
					if (players_[playerIndexturn_]->getIsAi()) {
						decision = players_[playerIndexturn_]->decideJail();
					}
					if (isButtonClicked(jailPayButton_) || (players_[playerIndexturn_]->getIsAi() && decision == PAY))
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

				if (isButtonClicked(rollDiceButton_) || players_[playerIndexturn_]->getIsAi()) {
					if (players_[playerIndexturn_]->getIsAi()) {
						std::cout << players_[playerIndexturn_]->getTest() << std::endl;
					}
					unsigned int roll1 = rollDice();
					unsigned int roll2 = rollDice();
					std::string val1 = std::to_string(roll1);
					std::string val2 = std::to_string(roll2);
					rolled_val = roll1 + roll2;
					std::string rol = "Rolled value: ";
					std::string val = std::to_string(rolled_val);
					rolledValueText_->setString(rol + val);
					notificationAdd(playerIndexturn_, rol + val + " -> (" + val1 + ", " + val2 + ")");
					std::cout << rol + val + " -> (" + val1 + ", " + val2 + ")" << std::endl;

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
							setTurnState(TURN_END);
						} else {
							int oldPos = players_[playerIndexturn_]->getPosition();
							movePlayer(playerIndexturn_, rolled_val);
							int newPos = players_[playerIndexturn_]->getPosition();
							handlePassingStart(oldPos, newPos);

							rollDiceButton_->setIsVisible(false);
							setTurnState(FIELD_ACTION);
						}
					} else {
						if (roll1 == roll2) {
							players_[playerIndexturn_]->setJailStatus(0);

							int oldPos = players_[playerIndexturn_]->getPosition();
							movePlayer(playerIndexturn_, rolled_val);
							int newPos = players_[playerIndexturn_]->getPosition();
							handlePassingStart(oldPos, newPos);

							std::string notification_msg = "Leaving jail on doubles";
							notificationAdd(playerIndexturn_, notification_msg);

							rollDiceButton_->setIsVisible(false);
							jailPayButton_->setIsVisible(false);
							setTurnState(FIELD_ACTION);
						} else if (player_jail_status == 1) {
							if (players_[playerIndexturn_]->getMoney() < JAIL_PAY_MONEY) {
								money_to_find = JAIL_PAY_MONEY;
								bank_pay_rent = true;
								players_to_pay_rent.clear();
								setTurnState(PAY_RENT);
								std::string notification_msg = "Not enough money to leave jail";
								notificationAdd(playerIndexturn_, notification_msg);
							}

							players_[playerIndexturn_]->setJailStatus(0);
							players_[playerIndexturn_]->substractMoney(JAIL_BAILOUT);

							int oldPos = players_[playerIndexturn_]->getPosition();
							movePlayer(playerIndexturn_, rolled_val);
							int newPos = players_[playerIndexturn_]->getPosition();
							handlePassingStart(oldPos, newPos);

							rollDiceButton_->setIsVisible(false);
							jailPayButton_->setIsVisible(false);
							setTurnState(FIELD_ACTION);

							std::string notification_msg = "Player left jail on forced bailout";
							notificationAdd(playerIndexturn_, notification_msg);

						} else {
							players_[playerIndexturn_]->reduceJailStatus();

							rollDiceButton_->setIsVisible(false);
							jailPayButton_->setIsVisible(false);

							setTurnState(TURN_END);
						}
					}
				}
			} break;
			case FIELD_ACTION: {
				int pos = players_[playerIndexturn_]->getPosition();
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
						setTurnState(BUY_ACTION);
						clearPropertyData(true);
						showPropertyData(pos, true);
					} else if (owner->getId() != players_[playerIndexturn_]->getId()) {
						unsigned int rent_to_pay = calculateRent(rolled_val, pos);
						std::string notification("Rent to pay: " + std::to_string(rent_to_pay));
						notificationAdd(playerIndexturn_, notification);
						if (players_[playerIndexturn_]->getMoney() >= rent_to_pay) {
							players_[playerIndexturn_]->substractMoney(rent_to_pay);
							owner->addMoney(rent_to_pay);
							setTurnState(TURN_END);
						} else {
							money_to_find = rent_to_pay;
							bank_pay_rent = false;
							players_to_pay_rent.clear();
							players_to_pay_rent.push_back(owner);
							setTurnState(PAY_RENT);
						}
					} else {
						std::cout << "No action - player owns this field" << field_type << std::endl;
						setTurnState(TURN_END);
					}
				} else if (field_type == TAX) {
					TaxField field = std::get<TaxField>(getBoard()->getFieldById(pos));
					unsigned int tax_to_pay = field.getTaxValue();
					if (players_[playerIndexturn_]->getMoney() >= tax_to_pay) {
						players_[playerIndexturn_]->substractMoney(tax_to_pay);
						setTurnState(TURN_END);
					} else {
						money_to_find = tax_to_pay;
						bank_pay_rent = true;
						players_to_pay_rent.clear();
						setTurnState(PAY_RENT);
					}
				} else if (field_type == GO_TO_JAIL) {
					std::string notification_msg = "Goes to jail via GO TO JAIL";
					notificationAdd(playerIndexturn_, notification_msg);
					sendToJail(playerIndexturn_);
					players_[playerIndexturn_]->setJailStatus(3);
					setTurnState(TURN_END);
				} else if (field_type == CHANCE) {
					ChanceCard chance_card = getChanceCard();
					updateChanceCard();
					std::string notification_msg = "Chance Card: ";
					notificationAdd(playerIndexturn_, notification_msg + chance_card.getText());

					switch (chance_card.getType()) {
						case MovementToProperty: {
							int oldPos = players_[playerIndexturn_]->getPosition();
							int posIncrement = (40 + chance_card.getValue()) - oldPos;
							movePlayer(playerIndexturn_, posIncrement);
							int newPos = chance_card.getValue();
							handlePassingStart(oldPos, newPos);
							setTurnState(FIELD_ACTION);
						} break;

						case BankPaysYou:
							players_[playerIndexturn_]->addMoney(chance_card.getValue());
							setTurnState(TURN_END);
							break;

						case GetOutOfJailCard:
							players_[playerIndexturn_]->setJailCards(players_[playerIndexturn_]->getJailCards() + 1);
							setTurnState(TURN_END);
							break;

						case GoToJail:
							sendToJail(playerIndexturn_);
							players_[playerIndexturn_]->setJailStatus(3);
							setTurnState(TURN_END);
							break;

						case PayForHouseHotel: {
							unsigned int sum = 0;
							for (auto field_id : players_[playerIndexturn_]->getFiledOwnedId()) {
								FieldType fieldType = std::visit(
									[](Field& field) { return field.getType(); }, getBoard()->getFieldById(field_id));
								if (fieldType == STREET) {
									StreetField field = std::get<StreetField>(getBoard()->getFieldById(field_id));
									int hotelCost = field.getIsHotel() * 100;
									sum += hotelCost;
									if (hotelCost == 0) {
										int housesCost = field.getHouseNumber() * 25;
										sum += housesCost;
									}
								}
							}

							if (sum == 0) {
								std::string notification_msg = "Amount to pay: " + std::to_string(sum);
								notificationAdd(playerIndexturn_, notification_msg);
								setTurnState(TURN_END);
							} else {
								if (players_[playerIndexturn_]->getMoney() >= sum) {
									players_[playerIndexturn_]->substractMoney(sum);
									std::string notification_msg = "Paid to bank: " + std::to_string(sum);
									notificationAdd(playerIndexturn_, notification_msg);
									setTurnState(TURN_END);
								} else {
									money_to_find = sum;
									bank_pay_rent = true;
									players_to_pay_rent.clear();
									setTurnState(PAY_RENT);
									std::string notification_msg = "Not enough money. Needed: " + std::to_string(sum);
									notificationAdd(playerIndexturn_, notification_msg);
								}
							}
						} break;

						case Tax: {
							if ((int)players_[playerIndexturn_]->getMoney() >= chance_card.getValue()) {
								players_[playerIndexturn_]->substractMoney(chance_card.getValue());
								std::string notification_msg = "Paid to bank: " + std::to_string(chance_card.getValue());
								notificationAdd(playerIndexturn_, notification_msg);
								setTurnState(TURN_END);
							} else {
								money_to_find = chance_card.getValue();
								;
								bank_pay_rent = true;
								players_to_pay_rent.clear();
								setTurnState(PAY_RENT);
								std::string notification_msg =
									"Not enough money. Needed: " + std::to_string(chance_card.getValue());
								notificationAdd(playerIndexturn_, notification_msg);
							}

						} break;

						case MovementSpaces: {
							int oldPos = players_[playerIndexturn_]->getPosition();
							int posIncrement = chance_card.getValue();
							movePlayer(playerIndexturn_, posIncrement);
							int newPos = players_[playerIndexturn_]->getPosition();
							if (posIncrement >= 0) {
								handlePassingStart(oldPos, newPos);
							}
							setTurnState(FIELD_ACTION);
						} break;

						case PayPlayers: {
							unsigned int toPay = chance_card.getValue() * (players_.size() - 1);

							if (players_[playerIndexturn_]->getMoney() >= toPay) {
								for (auto player_ptr : players_) {
									if (player_ptr != players_[playerIndexturn_]) {
										player_ptr->substractMoney(chance_card.getValue());
										players_[playerIndexturn_]->addMoney(chance_card.getValue());
									}
								}
								setTurnState(TURN_END);
							} else {
								players_to_pay_rent.clear();
								money_to_find = toPay;
								bank_pay_rent = false;
								for (auto player_ptr : players_) {
									if (player_ptr != players_[playerIndexturn_]) {
										players_to_pay_rent.push_back(player_ptr);
									}
								}
								setTurnState(PAY_RENT);
							}
						} break;

						default:
							setTurnState(TURN_END);
							break;
					}

				} else {
					std::cout << "No action" << field_type << std::endl;
					setTurnState(TURN_END);
				}
			} break;

			case BUY_ACTION: {
				int pos = players_[playerIndexturn_]->getPosition();
				unsigned int price = getFieldPriceByPosition(pos);
				resignBuyFieldButton_->setIsVisible(true);
				buyFieldButton_->setIsVisible(true);
				BuyDecision buy_decision = players_[playerIndexturn_]->decideBuy(pos);
				if (isButtonClicked(buyFieldButton_) || (players_[playerIndexturn_]->getIsAi() && buy_decision == BUY)) { //  && players_[playerIndexturn_]->getMoney() >= price
					if (players_[playerIndexturn_]->getMoney() >= price) {	// possible to buy property

						std::string textPlayerBoughtProperty(
							"bought field " +
							std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
						notificationAdd(playerIndexturn_, textPlayerBoughtProperty);

						players_[playerIndexturn_]->substractMoney(price);
						players_[playerIndexturn_]->addFieldOwnedId(pos);
						std::shared_ptr<Player> player_ptr = players_[playerIndexturn_];
						addOwnerToPropertyField(player_ptr, pos);

						resignBuyFieldButton_->setIsVisible(false);
						buyFieldButton_->setIsVisible(false);
						setTurnState(TURN_END);
					} else	// NOT possible to buy property
					{
						std::string textPlayerBoughtProperty(
							"Can not buy field: " +
							std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
						notificationAdd(playerIndexturn_, textPlayerBoughtProperty);
					}
				}

				if (isButtonClicked(resignBuyFieldButton_) || getAuctionState() != NoAuction || (players_[playerIndexturn_]->getIsAi() && (buy_decision == RESIGN || players_[playerIndexturn_]->getMoney() < price))) {
					if (getAuctionState() == NoAuction) {
						std::string textPlayerResginedProperty(
							"resigned to buy field " +
							std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
						notificationAdd(playerIndexturn_, textPlayerResginedProperty);
						resignBuyFieldButton_->setIsVisible(false);
						buyFieldButton_->setIsVisible(false);
						boardToAuctionSwitchHandler(true);
						setScreenType(AUCTION);
						setAuctionState(Initialization);
					}
					performAuction();
					if (getAuctionState() == NoAuction) {
						boardToAuctionSwitchHandler(false);
						resignBuyFieldButton_->setIsVisible(false);
						buyFieldButton_->setIsVisible(false);
						rollDiceButton_->setIsVisible(false);
						setScreenType(BOARDGAME);
						setTurnState(TURN_END);
					}
				}
			} break;

			case PAY_RENT: {
				if (playerChanged) {
					std::string textPlayerrent(
						"Must make money to paid rent of " + std::to_string(money_to_find) + " or go bankrupt");
					notificationAdd(playerIndexturn_, textPlayerrent);
					playerChanged = false;
				}
				if (players_[playerIndexturn_]->getMoney() < money_to_find) {  // player trying to get money to pay rent
					if (!players_[playerIndexturn_]->getIsAi()) {
						buildingsManagingWorker();
					} else {
						aiBuildingsMangingWorker();
						if (players_[playerIndexturn_]->getMoney() < money_to_find) {
							ai_bankrupted = true;
						}
					}
				} else {  // player has got money to pay rent
					players_[playerIndexturn_]->substractMoney(money_to_find);
					unsigned int payment_counter = players_to_pay_rent.size();
					if (bank_pay_rent) {
						++payment_counter;
					}
					for (auto player_ptr : players_to_pay_rent) {
						player_ptr->addMoney(money_to_find / payment_counter);
					}
					std::string textPlayerrent("Paid rent of " + std::to_string((money_to_find)));
					notificationAdd(playerIndexturn_, textPlayerrent);
					setTurnState(TURN_END);
				}
			} break;

			case TURN_END:
				buildingsManagingWorker();
				nextTurnButton_->setIsVisible(true);
				if (isButtonClicked(nextTurnButton_) || players_[playerIndexturn_]->getIsAi()) { // || players_[playerIndexturn_]->getIsAi()
					setTurnState(ROLL_DICE);
					rollDiceButton_->setIsVisible(true);
					rolledValueText_->setString("");
					resignBuyFieldButton_->setIsVisible(false);
					buyFieldButton_->setIsVisible(false);
					if (!playerBankrutedNow || !isDouble || players_[playerIndexturn_]->getJailStatus() != 0) {
						double_turns = 0;

						if (!playerBankrutedNow) {	// check if current player bankruted, if not inc turn
							gameTurnsCounterHandle();
							incPlayerIndexTurn();
						} else {
							makePlayerBankrupt(playerIndexturn_);
							playerBankrutedNow = false;
							if (playerIndexturn_ >=
								players_.size()) {	// check if current player bankruted, if yes verify turn index
								playerIndexturn_ = 0;
								gameTurnByPlayerDone_ = {false, false, false, false};
								++gameTurnsGloballyDone_;
							}
						}

						if (gameFinishedCheckWinner()) {
							removePlayerFromGame(playerIndexturn_, false);
							if(isAiGameOnly_)
							{
								fileLoggerClose();
								return false;	//leave game engine to return result to AI training
							}
							else
							{
								updateResultScreenStuff();
								setScreenType(RESULT);
								setTurnState(NO_TURN);
								fileLoggerClose();
							}
						} else if (gameFinishedCheckDraw()) {
							removePlayerFromGame(playerIndexturn_, true);
							if(isAiGameOnly_)
							{
								fileLoggerClose();
								return false;	//leave game engine to return result to AI training
							}
							else
							{
								updateResultScreenStuff();
								setScreenType(RESULT);
								setTurnState(NO_TURN);
								fileLoggerClose();
							}
						}
						turnInfoTextShow();
					}

					if (playerBankrutedNow) {
						makePlayerBankrupt(playerIndexturn_);
						playerBankrutedNow = false;
						if (playerIndexturn_ >=
							players_.size()) {	// check if current player bankruted, if yes verify turn index
							playerIndexturn_ = 0;
							gameTurnByPlayerDone_ = {false, false, false, false};
							++gameTurnsGloballyDone_;
						}
					}
					nextTurnButton_->setIsVisible(false);
					playerChanged = true;
				}
				break;
		case WITHDRAW_ONGOING:
			if (isButtonClicked(getWithdraw().getResignButton()) ||
				isButtonClicked(getWithdraw().getResignValueButton()) ||
				isButtonClicked(getWithdraw().getResignDecisionButton())) {
				getWithdraw().setChooseScreenVisible(false);
				getWithdraw().setValueScreenVisible(false);
				getWithdraw().setDecisionScreenVisible(false);
				setScreenType(BOARDGAME);
				setTurnState(getWithdraw().getTurnState());
				getWithdraw().setPlayer2ToWithdraw(nullptr);
			} else {
				if (getScreenType() == WITHDRAW_CHOOSE_PLAYER) {
					if (isButtonClicked(getWithdraw().getPlayer1Button())) {
						for (auto player_ptr : getPlayers()) {
							if (player_ptr->getId() == 0) {
								getWithdraw().setPlayer2ToWithdraw(player_ptr);
							}
						}
					} else if (isButtonClicked(getWithdraw().getPlayer2Button())) {
						for (auto player_ptr : getPlayers()) {
							if (player_ptr->getId() == 1) {
								getWithdraw().setPlayer2ToWithdraw(player_ptr);
							}
						}
					} else if (isButtonClicked(getWithdraw().getPlayer3Button())) {
						for (auto player_ptr : getPlayers()) {
							if (player_ptr->getId() == 2) {
								getWithdraw().setPlayer2ToWithdraw(player_ptr);
							}
						}
					} else if (isButtonClicked(getWithdraw().getPlayer4Button())) {
						for (auto player_ptr : getPlayers()) {
							if (player_ptr->getId() == 3) {
								getWithdraw().setPlayer2ToWithdraw(player_ptr);
							}
						}
					}

					if (getScreenType() == WITHDRAW_CHOOSE_PLAYER && getWithdraw().getPlayer2ToWithdraw() != nullptr) {
						setScreenType(WITHDRAW_ADD_VALUE);
						getWithdraw().setChooseScreenVisible(false);
						getWithdraw().setValueScreenVisible(true);
					}
				} else if (getScreenType() == WITHDRAW_ADD_VALUE) {
					getWithdraw().moneyTextUpdate();
					if (isButtonClicked(getWithdraw().getPlayer1minus1())) {
						getWithdraw().moneyTransferIndex(1, -1);
					} else if (isButtonClicked(getWithdraw().getPlayer1minus10())) {
						getWithdraw().moneyTransferIndex(1, -10);
					} else if (isButtonClicked(getWithdraw().getPlayer1minus100())) {
						getWithdraw().moneyTransferIndex(1, -100);
					} else if (isButtonClicked(getWithdraw().getPlayer1plus1())) {
						getWithdraw().moneyTransferIndex(1, 1);
					} else if (isButtonClicked(getWithdraw().getPlayer1plus10())) {
						getWithdraw().moneyTransferIndex(1, 10);
					} else if (isButtonClicked(getWithdraw().getPlayer1plus100())) {
						getWithdraw().moneyTransferIndex(1, 100);
					} else if (isButtonClicked(getWithdraw().getPlayer2minus1())) {
						getWithdraw().moneyTransferIndex(2, -1);
					} else if (isButtonClicked(getWithdraw().getPlayer2minus10())) {
						getWithdraw().moneyTransferIndex(2, -10);
					} else if (isButtonClicked(getWithdraw().getPlayer2minus100())) {
						getWithdraw().moneyTransferIndex(2, -100);
					} else if (isButtonClicked(getWithdraw().getPlayer2plus1())) {
						getWithdraw().moneyTransferIndex(2, 1);
					} else if (isButtonClicked(getWithdraw().getPlayer2plus10())) {
						getWithdraw().moneyTransferIndex(2, 10);
					} else if (isButtonClicked(getWithdraw().getPlayer2plus100())) {
						getWithdraw().moneyTransferIndex(2, 100);
					} else if (isButtonClicked(getWithdraw().getPlayer1NextButton())) {
						getWithdraw().addPropertyPlayerShowed(1, 1);
						getWithdraw().showProperty(1);

					} else if (isButtonClicked(getWithdraw().getPlayer1PreviousButton())) {
						getWithdraw().addPropertyPlayerShowed(-1, 1);
						getWithdraw().showProperty(1);
					} else if (isButtonClicked(getWithdraw().getPlayer2NextButton())) {
						getWithdraw().addPropertyPlayerShowed(1, 4);
						getWithdraw().showProperty(4);

					} else if (isButtonClicked(getWithdraw().getPlayer2PreviousButton())) {
						getWithdraw().addPropertyPlayerShowed(-1, 4);
						getWithdraw().showProperty(4);
					} else if (isButtonClicked(getWithdraw().getPlayer1IndexNextButton())) {
						getWithdraw().addPropertyPlayerShowed(1, 2);
						getWithdraw().showProperty(2);

					} else if (isButtonClicked(getWithdraw().getPlayer1IndexPreviousButton())) {
						getWithdraw().addPropertyPlayerShowed(-1, 2);
						getWithdraw().showProperty(2);
					} else if (isButtonClicked(getWithdraw().getPlayer2IndexNextButton())) {
						getWithdraw().addPropertyPlayerShowed(1, 3);
						getWithdraw().showProperty(3);

					} else if (isButtonClicked(getWithdraw().getPlayer2IndexPreviousButton())) {
						getWithdraw().addPropertyPlayerShowed(-1, 3);
						getWithdraw().showProperty(3);
					} else if (isButtonClicked(getWithdraw().getPlayer1AddButton())) {
						getWithdraw().propertyPlayerMoveIndex(1, 1);
						getWithdraw().showProperty(2);
						getWithdraw().showProperty(1);

					} else if (isButtonClicked(getWithdraw().getPlayer1RemoveButton())) {
						getWithdraw().propertyPlayerMoveIndex(-1, 1);
						getWithdraw().showProperty(1);
						getWithdraw().showProperty(2);

					} else if (isButtonClicked(getWithdraw().getPlayer2AddButton())) {
						getWithdraw().propertyPlayerMoveIndex(1, 2);
						getWithdraw().showProperty(3);
						getWithdraw().showProperty(4);

					} else if (isButtonClicked(getWithdraw().getPlayer2RemoveButton())) {
						getWithdraw().propertyPlayerMoveIndex(-1, 2);
						getWithdraw().showProperty(3);
						getWithdraw().showProperty(4);
					} else if (isButtonClicked(getWithdraw().getSubmitValueButton()) &&
							   getWithdraw().isNonZeroValue()) {
						setScreenType(WITHDRAW_DECISION);
						getWithdraw().setDecisionScreenVisible(true);
						getWithdraw().setValueScreenVisible(false);
					}
				} else if (getScreenType() == WITHDRAW_DECISION) {
					if (isButtonClicked(getWithdraw().getAcceptDecisionButton())) {
						getWithdraw().makeWithdraw();
						getWithdraw().setChooseScreenVisible(false);
						getWithdraw().setValueScreenVisible(false);
						getWithdraw().setDecisionScreenVisible(false);
						setScreenType(BOARDGAME);
						setTurnState(getWithdraw().getTurnState());
						getWithdraw().setPlayer2ToWithdraw(nullptr);
					}
				}
			}
			break;
		default:
			break;
		}
	} catch (std::exception& e){
		std::cout << e.what() << std::endl;
		std::cout << "Crash. Player: " << players_[playerIndexturn_]->getId() << "Position: " << players_[playerIndexturn_]->getPosition() << " Money: " << players_[playerIndexturn_]->getMoney() << std::endl;
	}
	return true;
}

void monopolyGameEngine::updateAvailableHousesHotelText() {
	availableHousesText_->setString("Houses bank: " + std::to_string(getHouseCount()));
	availableHotelsText_->setString("Hotels bank: " + std::to_string(getHotelCount()));
}

void monopolyGameEngine::createAvailableHousesHotelText() {
	std::shared_ptr<sf::Text> avaHousesText(
		new sf::Text("Houses bank: " + std::to_string(getHouseCount()), getFont(), getFontSize() - 2));
	avaHousesText->setPosition(AVAILABLE_HOUSE_TEXT_POSITION);
	avaHousesText->setFillColor(sf::Color::Black);
	availableHousesText_ = avaHousesText;
	addText(avaHousesText);

	std::shared_ptr<sf::Text> avaHotelsText(
		new sf::Text("Hotels bank: " + std::to_string(getHotelCount()), getFont(), getFontSize() - 2));
	avaHotelsText->setPosition(sf::Vector2f(AVAILABLE_HOUSE_TEXT_POSITION.x, AVAILABLE_HOUSE_TEXT_POSITION.y + 30));
	avaHotelsText->setFillColor(sf::Color::Black);
	availableHotelsText_ = avaHotelsText;
	addText(avaHotelsText);
}

sf::Vector2f monopolyGameEngine::getUpdatePlayerSpritePosition() {
	float x_offset = 0;
	float y_offset = 0;
	const float HEIGHT_OFFSET = 20.0;  // If we want piece to go lower we increase this value.
	unsigned int player_position = players_[playerIndexturn_]->getPosition();
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

void monopolyGameEngine::addAuctionButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	auctionButtons_.push_back(button);
}

void monopolyGameEngine::addAuctionText(std::shared_ptr<sf::Text> textTmp) {
	std::shared_ptr<sf::Text> text = textTmp;
	auctionTexts_.push_back(text);
}

std::vector<std::shared_ptr<Button>>& monopolyGameEngine::getButtons() {
	return buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& monopolyGameEngine::getTexts() {
	return texts_;
}

std::vector<std::shared_ptr<Button>>& monopolyGameEngine::getAuctionButtons() {
	return auctionButtons_;
}

std::vector<std::shared_ptr<sf::Text>>& monopolyGameEngine::getAuctionTexts() {
	return auctionTexts_;
}

std::vector<std::shared_ptr<sf::Text>>& monopolyGameEngine::getResultTexts() {
	return resultPlayersPlaces_;
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
	std::shared_ptr<Button> buttonRollDice(new Button(IDLE, L"Rzut kośćmi", buttonSize, getFontSize()));
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
	std::shared_ptr<sf::Text> turnInfoText(new sf::Text("Tura: ", getFont(), getFontSize()));
	turnInfoText->setPosition(TURN_INFO_TEXT_POSITION);
	turnInfoText->setFillColor(sf::Color::Black);
	turnInfoText_ = turnInfoText;
	addText(turnInfoText);
}

void monopolyGameEngine::createTextRolledValue() {
	std::shared_ptr<sf::Text> rolledValueText(new sf::Text("", getFont(), getFontSize()));
	rolledValueText->setPosition(ROLLED_VALUE_TEXT_POSITION);
	rolledValueText->setFillColor(sf::Color::Black);
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
			new sf::Text("Gracz " + std::to_string(id + 1), getFont(), getFontSize()));
		playerText->setPosition(defPos);
		playerText->setFillColor(player->getColor());
		playerText->setOutlineColor(sf::Color::Black);
		playerText->setOutlineThickness(2);
		addText(playerText);

		std::shared_ptr<sf::Text> playerMoneyText(
			new sf::Text("Kasa: " + std::to_string(player->getMoney()), getFont(), getFontSize() - 7));
		playerMoneyText->setPosition(sf::Vector2f(defPos.x, defPos.y + 50));
		playerMoneyText->setFillColor(sf::Color::Black);
		addText(playerMoneyText);

		std::shared_ptr<sf::Text> playerPositionText(
			new sf::Text("Pozycja: " + std::to_string(player->getPosition() + 1), getFont(), getFontSize() - 7));
		playerPositionText->setPosition(sf::Vector2f(defPos.x, defPos.y + 80));
		playerPositionText->setFillColor(sf::Color::Black);
		addText(playerPositionText);

		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(player->getPosition()));
		std::shared_ptr<sf::Text> playerPositionNameText(new sf::Text(streetName, getFont(), getFontSize() - 7));
		playerPositionNameText->setPosition(sf::Vector2f(defPos.x, defPos.y + 110));
		playerPositionNameText->setFillColor(sf::Color::Black);
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
	bool isPlayerinGame[4] = {false};
	for (auto player : players_) {
		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(player->getPosition()));
		int id = player->getId();
		playerInfoText_[id][1]->setString("Kasa: " + std::to_string(player->getMoney()));
		playerInfoText_[id][2]->setString("Pozycja: " + std::to_string(player->getPosition() + 1));
		playerInfoText_[id][3]->setString(streetName);
		isPlayerinGame[id] = true;
	}

	for(unsigned int i = 0 ; i < PLAYERS_MAX_ ; ++i)
	{
		unsigned int id = playersStartingIds_[i];
		if (id != 255 && !isPlayerinGame[id]) {
			if(playerInfoText_[id].size() > 0)	//check if player id was even in game from begining
			{
				playerInfoText_[id][1]->setString("Bankrut");
				playerInfoText_[id][2]->setString("");
				playerInfoText_[id][3]->setString("");
			}
		}
	}
}

void monopolyGameEngine::createTextBiddedProperty() {
	std::shared_ptr<sf::Text> biddedPropertyText(new sf::Text("Cel: ", getFont(), getFontSize()));
	biddedPropertyText->setPosition(BIDDED_PROPERTY_TEXT_POSITION);
	biddedPropertyText->setFillColor(sf::Color::Black);
	biddedPropertyText_ = biddedPropertyText;
	addAuctionText(biddedPropertyText);
}

void monopolyGameEngine::createTextBidderInfo() {
	std::shared_ptr<sf::Text> bidderInfoText(new sf::Text("Tura gracza: ", getFont(), getFontSize()));
	bidderInfoText->setPosition(BIDDER_INFO_TEXT_POSITION);
	bidderInfoText->setFillColor(sf::Color::Black);
	bidderInfoText_ = bidderInfoText;
	addAuctionText(bidderInfoText);
}

void monopolyGameEngine::createTextHighestBidInfo() {
	std::shared_ptr<sf::Text> highestBidText(new sf::Text(L"Najwyższa oferta: ", getFont(), getFontSize()));
	highestBidText->setPosition(HIGHEST_BID_TEXT_POSITION);
	highestBidText->setFillColor(sf::Color::Black);
	highestBidInfoText_ = highestBidText;
	addAuctionText(highestBidText);
}

void monopolyGameEngine::createTextLeadingBidderInfo() {
	std::shared_ptr<sf::Text> leadingBidderText(new sf::Text(L"Prowadząca oferta: ", getFont(), getFontSize()));
	leadingBidderText->setPosition(LEADING_BIDDER_TEXT_POSITION);
	leadingBidderText->setFillColor(sf::Color::Black);
	leadingBidderInfoText_ = leadingBidderText;
	addAuctionText(leadingBidderText);
}

void monopolyGameEngine::createCurrentOfferBidderInfo() {
	std::shared_ptr<sf::Text> currentOfferText(new sf::Text("Aktualna oferta: ", getFont(), getFontSize()));
	currentOfferText->setPosition(CURRENT_OFFER_TEXT_POSITION);
	currentOfferText->setFillColor(sf::Color::Black);
	currentOfferInfoText_ = currentOfferText;
	addAuctionText(currentOfferText);
}

void monopolyGameEngine::createButtonBuyResign() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBuy(new Button(IDLE, "Kup", buttonSize, getFontSize()));
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

	std::shared_ptr<Button> buttonResign(new Button(IDLE, "Rezygnuj", buttonSize, getFontSize()));
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
	std::shared_ptr<Button> buttonNext(new Button(IDLE, L"Następny", buttonSize, getFontSize()));
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
	std::shared_ptr<Button> buttonPrev(new Button(IDLE, "Poprzedni", buttonSize, getFontSize()));
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
	houseText_ = std::make_shared<sf::Text>("Dom", getFont(), getFontSize() - 2);
	houseText_->setPosition(HOUSE_TEXT_POSITION);
	houseText_->setFillColor(sf::Color::Black);
	houseText_->setOrigin(houseText_->getGlobalBounds().getSize() / 2.f + houseText_->getLocalBounds().getPosition());
	hotelText_ = std::make_shared<sf::Text>("Hotel", getFont(), getFontSize() - 2);
	hotelText_->setPosition(HOTEL_TEXT_POSITION);
	hotelText_->setFillColor(sf::Color::Black);
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
	std::shared_ptr<Button> buttonBuyHouse(new Button(IDLE, "Kup", buttonSize, getFontSize()));
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

	std::shared_ptr<Button> buttonSellHouse(new Button(IDLE, "Sprzedaj", buttonSize, getFontSize()));
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

	std::shared_ptr<Button> buttonBuyHotel(new Button(IDLE, "Kup", buttonSize, getFontSize()));
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

	std::shared_ptr<Button> buttonSellHotel(new Button(IDLE, "Sprzedaj", buttonSize, getFontSize()));
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
	sf::Vector2f buttonSize = sf::Vector2f(160, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBankrupt(new Button(IDLE, "Bankrutuj", buttonSize, getFontSize()));
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
	sf::Vector2f buttonSize = sf::Vector2f(160, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonNextTurn(new Button(IDLE, "Kolejna tura", buttonSize, getFontSize()));
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
	sf::String jail_cost = (std::to_string(JAIL_PAY_MONEY));
	std::shared_ptr<Button> buttonJailPay(
		new Button(IDLE, L"Zaplać " + jail_cost + L" aby wyjść z więzienia", buttonSize, getFontSize()));
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
	Text100_->setFillColor(sf::Color::Black);
	Text100_->setOrigin(Text100_->getGlobalBounds().getSize() / 2.f + Text100_->getLocalBounds().getPosition());
	addAuctionText(Text100_);

	Text10_ = std::make_shared<sf::Text>("10", getFont(), getFontSize());
	Text10_->setPosition(TEXT_10_POSITION);
	Text10_->setFillColor(sf::Color::Black);
	Text10_->setOrigin(Text10_->getGlobalBounds().getSize() / 2.f + Text10_->getLocalBounds().getPosition());
	addAuctionText(Text10_);

	Text1_ = std::make_shared<sf::Text>("1", getFont(), getFontSize());
	Text1_->setPosition(TEXT_1_POSITION);
	Text1_->setFillColor(sf::Color::Black);
	Text1_->setOrigin(Text1_->getGlobalBounds().getSize() / 2.f + Text1_->getLocalBounds().getPosition());
	addAuctionText(Text1_);

	sf::Vector2f buttonSize = sf::Vector2f(50, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonAdd100(new Button(IDLE, "+", buttonSize, getFontSize() + 10));
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
	buttonAdd100->setIsVisible(false);
	buttonAdd100->setIsActive(false);
	buttonAdd100->setIsFocus(false);
	add100ToOfferButton_ = buttonAdd100;
	addButton(buttonAdd100);
	addAuctionButton(buttonAdd100);

	std::shared_ptr<Button> buttonAdd10(new Button(IDLE, "+", buttonSize, getFontSize() + 10));
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
	buttonAdd10->setIsVisible(false);
	buttonAdd10->setIsActive(false);
	buttonAdd10->setIsFocus(false);
	add10ToOfferButton_ = buttonAdd10;
	addButton(buttonAdd10);
	addAuctionButton(buttonAdd10);

	std::shared_ptr<Button> buttonAdd1(new Button(IDLE, "+", buttonSize, getFontSize() + 10));
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
	buttonAdd1->setIsVisible(false);
	buttonAdd1->setIsActive(false);
	buttonAdd1->setIsFocus(false);
	add1ToOfferButton_ = buttonAdd1;
	addButton(buttonAdd1);
	addAuctionButton(buttonAdd1);

	std::shared_ptr<Button> buttonSubstract100(new Button(IDLE, "-", buttonSize, getFontSize() + 10));
	buttonSubstract100->setFont(getFont());
	buttonSubstract100->setPosition(SUBSTRACT_100_BUTTON_POSITION);
	buttonSubstract100->getText().setPosition(
		SUBSTRACT_100_BUTTON_POSITION.x - 6, SUBSTRACT_100_BUTTON_POSITION.y - 28);
	buttonSubstract100->setActiveBackColor(activeButtonBackColor);
	buttonSubstract100->setActiveTextColor(activeButtonTextColor);
	buttonSubstract100->setInactiveBackColor(inActiveButtonBackColor);
	buttonSubstract100->setInactiveTextColor(inActiveButtonTextColor);
	buttonSubstract100->setFocusBackColor(FocusButtonBackColor);
	buttonSubstract100->setFocusTextColor(FocusButtonTextColor);
	buttonSubstract100->setIsClicked(false);
	buttonSubstract100->setIsVisible(false);
	buttonSubstract100->setIsActive(false);
	buttonSubstract100->setIsFocus(false);
	substract100FromOfferButton_ = buttonSubstract100;
	addButton(buttonSubstract100);
	addAuctionButton(buttonSubstract100);

	std::shared_ptr<Button> buttonSubstract10(new Button(IDLE, "-", buttonSize, getFontSize() + 10));
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
	buttonSubstract10->setIsVisible(false);
	buttonSubstract10->setIsActive(false);
	buttonSubstract10->setIsFocus(false);
	substract10FromOfferButton_ = buttonSubstract10;
	addButton(buttonSubstract10);
	addAuctionButton(buttonSubstract10);

	std::shared_ptr<Button> buttonSubstract1(new Button(IDLE, "-", buttonSize, getFontSize() + 10));
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
	buttonSubstract1->setIsVisible(false);
	buttonSubstract1->setIsActive(false);
	buttonSubstract1->setIsFocus(false);
	substract1FromOfferButton_ = buttonSubstract1;
	addButton(buttonSubstract1);
	addAuctionButton(buttonSubstract1);
}

void monopolyGameEngine::createAuctionBidButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonBidAuction(new Button(IDLE, "Przebij", buttonSize, getFontSize()));
	buttonBidAuction->setFont(getFont());
	buttonBidAuction->setPosition(AUCTION_BID_BUTTON_POSITION);
	buttonBidAuction->setActiveBackColor(activeButtonBackColor);
	buttonBidAuction->setActiveTextColor(activeButtonTextColor);
	buttonBidAuction->setInactiveBackColor(inActiveButtonBackColor);
	buttonBidAuction->setInactiveTextColor(inActiveButtonTextColor);
	buttonBidAuction->setFocusBackColor(FocusButtonBackColor);
	buttonBidAuction->setFocusTextColor(FocusButtonTextColor);
	buttonBidAuction->setIsClicked(false);
	buttonBidAuction->setIsVisible(false);
	buttonBidAuction->setIsActive(false);
	buttonBidAuction->setIsFocus(false);
	auctionBidButton_ = buttonBidAuction;
	addButton(buttonBidAuction);
	addAuctionButton(buttonBidAuction);
}

void monopolyGameEngine::createAuctionResignButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonResignAuction(new Button(IDLE, "Rezygnuj", buttonSize, getFontSize()));
	buttonResignAuction->setFont(getFont());
	buttonResignAuction->setPosition(AUCTION_RESIGN_BUTTON_POSITION);
	buttonResignAuction->setActiveBackColor(activeButtonBackColor);
	buttonResignAuction->setActiveTextColor(activeButtonTextColor);
	buttonResignAuction->setInactiveBackColor(inActiveButtonBackColor);
	buttonResignAuction->setInactiveTextColor(inActiveButtonTextColor);
	buttonResignAuction->setFocusBackColor(FocusButtonBackColor);
	buttonResignAuction->setFocusTextColor(FocusButtonTextColor);
	buttonResignAuction->setIsClicked(false);
	buttonResignAuction->setIsVisible(false);
	buttonResignAuction->setIsActive(false);
	buttonResignAuction->setIsFocus(false);
	auctionResignButton_ = buttonResignAuction;
	addButton(buttonResignAuction);
	addAuctionButton(buttonResignAuction);
}

void monopolyGameEngine::createButtonWithdraw() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonWithdraw(new Button(IDLE, "Wymiana", buttonSize, getFontSize()));
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

void monopolyGameEngine::createResultScreenStuff()
{
	float x_coord = 960;
	std::shared_ptr<sf::Text> resultGameText = std::make_shared<sf::Text>("Wyniki rozgrywki", getFont(), getFontSize());
	resultGameText->setFillColor(sf::Color::Black);
	resultGameText->setOrigin(resultGameText->getGlobalBounds().getSize() / 2.f +
										  resultGameText->getLocalBounds().getPosition());
	resultGameText->setPosition(sf::Vector2f(x_coord, RESULT_DATA_Y));
	resultPlayersPlaces_.push_back(resultGameText);

	std::shared_ptr<Button> buttonReturn = createDefaultButton(L"Wróc", 120, 50);
	buttonReturn->setEventType(RETURN_TO_MAIN_MENU);
	returnToMainMenuButton_ = buttonReturn;
	addButton(buttonReturn);
}

void monopolyGameEngine::updateResultScreenStuff()
{
	int y_step = 40;
	float x_coord = 960;
	int y_offset = 0;
	for(std::vector<std::shared_ptr<Player>>::reverse_iterator iter = playersBankrupted_.rbegin(); iter != playersBankrupted_.rend(); ++iter)
	{
		y_offset += y_step;
		std::string text = std::to_string(iter->get()->getResultPlace()) + ": Gracz " + std::to_string(iter->get()->getId()+1);
		std::shared_ptr<sf::Text> playerResultText = std::make_shared<sf::Text>(text , getFont(), getFontSize());
		playerResultText->setFillColor(sf::Color::Black);
		playerResultText->setOrigin(playerResultText->getGlobalBounds().getSize() / 2.f +
											playerResultText->getLocalBounds().getPosition());
		playerResultText->setPosition(sf::Vector2f(x_coord, RESULT_DATA_Y + y_offset));
		resultPlayersPlaces_.push_back(playerResultText);
	}

	y_offset += y_step;
	returnToMainMenuButton_->setPosition(sf::Vector2f(x_coord, RESULT_DATA_Y + y_offset));
	for(auto button_ptr : getButtons())
	{
		button_ptr->setIsVisible(false);
	}
	returnToMainMenuButton_->setIsVisible(true);
}

void monopolyGameEngine::createMortagingButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonMortgage(new Button(IDLE, "Zastaw", buttonSize, getFontSize()));
	buttonMortgage->setFont(getFont());
	buttonMortgage->setPosition(MORTGAGE_BUTTON_POSITION);
	buttonMortgage->setActiveBackColor(activeButtonBackColor);
	buttonMortgage->setActiveTextColor(activeButtonTextColor);
	buttonMortgage->setInactiveBackColor(inActiveButtonBackColor);
	buttonMortgage->setInactiveTextColor(inActiveButtonTextColor);
	buttonMortgage->setFocusBackColor(FocusButtonBackColor);
	buttonMortgage->setFocusTextColor(FocusButtonTextColor);
	buttonMortgage->setIsClicked(false);
	buttonMortgage->setIsVisible(true);
	buttonMortgage->setIsActive(false);
	buttonMortgage->setIsFocus(false);
	MortgageButton_ = buttonMortgage;
	addButton(buttonMortgage);

	std::shared_ptr<Button> buttonUnMortgage(new Button(IDLE, "Wykup", buttonSize, getFontSize()));
	buttonUnMortgage->setFont(getFont());
	buttonUnMortgage->setPosition(UNMORTGAGE_BUTTON_POSITION);
	buttonUnMortgage->setActiveBackColor(activeButtonBackColor);
	buttonUnMortgage->setActiveTextColor(activeButtonTextColor);
	buttonUnMortgage->setInactiveBackColor(inActiveButtonBackColor);
	buttonUnMortgage->setInactiveTextColor(inActiveButtonTextColor);
	buttonUnMortgage->setFocusBackColor(FocusButtonBackColor);
	buttonUnMortgage->setFocusTextColor(FocusButtonTextColor);
	buttonUnMortgage->setIsClicked(false);
	buttonUnMortgage->setIsVisible(true);
	buttonUnMortgage->setIsActive(false);
	buttonUnMortgage->setIsFocus(false);
	unMortgageButton_ = buttonUnMortgage;
	addButton(buttonUnMortgage);
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
	unsigned int price = 0;
	unsigned int Mortgage = 0;
	unsigned int housePrice = 0;
	unsigned int hotelPrice = 0;
	unsigned int rents[7] = {0};
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
		Mortgage = field.getMortgage();
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
		Mortgage = field.getMortgage();
	} else	// fieldType == UTILITY
	{
		UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
		std::map<UtilityTiers, unsigned int> rentsMap = field.getRentMultipliers();
		rents[0] = rentsMap[ONE_UTILITY];
		rents[1] = rentsMap[TWO_UTILITIES];
		Mortgage = field.getMortgage();
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
	propertyName->setFillColor(sf::Color::Black);

	const float yOffset = 90;
	const float yOffset_step = 25;
	const float rentPricesOffsetX = 180;

	std::shared_ptr<sf::Text> propertyPrice(
		new sf::Text("Cena: " + std::to_string(price), getFont(), getFontSize() - 2));
	propertyPrice->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset));
	propertyPrice->setFillColor(sf::Color::Black);

	std::shared_ptr<sf::Text> propertyMortgage(new sf::Text("Zastaw:", getFont(), getFontSize() - 2));
	propertyMortgage->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 8));
	propertyMortgage->setFillColor(sf::Color::Black);

	std::shared_ptr<sf::Text> propertyMortgagePrice(
		new sf::Text(std::to_string(Mortgage), getFont(), getFontSize() - 2));
	propertyMortgagePrice->setPosition(
		sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 8));
	propertyMortgagePrice->setFillColor(sf::Color::Black);

	if (isPropertyShownToBuy) {
		propertyDataTexts_.push_back(propertyName);
		propertyDataTexts_.push_back(propertyMortgagePrice);
		propertyDataTexts_.push_back(propertyPrice);
		propertyDataTexts_.push_back(propertyMortgage);
	} else {
		allPropertyDataTexts_.push_back(propertyName);
		allPropertyDataTexts_.push_back(propertyMortgagePrice);
		allPropertyDataTexts_.push_back(propertyPrice);
		allPropertyDataTexts_.push_back(propertyMortgage);
	}

	if (fieldType == STREET) {
		std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Czynsz: ", getFont(), getFontSize() - 2));
		propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
		propertyRent1->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent2(new sf::Text("  kompletny kolor:", getFont(), getFontSize() - 2));
		propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
		propertyRent2->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent3(new sf::Text("  z 1 domem:", getFont(), getFontSize() - 2));
		propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
		propertyRent3->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent4(new sf::Text("  z 2 domami:", getFont(), getFontSize() - 2));
		propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
		propertyRent4->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent5(new sf::Text("  z 3 domami:", getFont(), getFontSize() - 2));
		propertyRent5->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 5));
		propertyRent5->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent6(new sf::Text("  z 4 domami:", getFont(), getFontSize() - 2));
		propertyRent6->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
		propertyRent6->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent7(new sf::Text("  z hotelem:", getFont(), getFontSize() - 2));
		propertyRent7->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 7));
		propertyRent7->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHouseCost(new sf::Text("Cena domu:", getFont(), getFontSize() - 2));
		propertyHouseCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 9));
		propertyHouseCost->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHotelCost(new sf::Text("Cena hotelu:", getFont(), getFontSize() - 2));
		propertyHotelCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 10));
		propertyHotelCost->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost1(
			new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
		propertyRentCost1->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 1));
		propertyRentCost1->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost2(
			new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
		propertyRentCost2->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 2));
		propertyRentCost2->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost3(
			new sf::Text(std::to_string(rents[2]), getFont(), getFontSize() - 2));
		propertyRentCost3->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 3));
		propertyRentCost3->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost4(
			new sf::Text(std::to_string(rents[3]), getFont(), getFontSize() - 2));
		propertyRentCost4->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
		propertyRentCost4->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost5(
			new sf::Text(std::to_string(rents[4]), getFont(), getFontSize() - 2));
		propertyRentCost5->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 5));
		propertyRentCost5->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost6(
			new sf::Text(std::to_string(rents[5]), getFont(), getFontSize() - 2));
		propertyRentCost6->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 6));
		propertyRentCost6->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost7(
			new sf::Text(std::to_string(rents[6]), getFont(), getFontSize() - 2));
		propertyRentCost7->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 7));
		propertyRentCost7->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHousePrice(
			new sf::Text(std::to_string(housePrice), getFont(), getFontSize() - 2));
		propertyHousePrice->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 9));
		propertyHousePrice->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyHotelPrice(
			new sf::Text(std::to_string(hotelPrice), getFont(), getFontSize() - 2));
		propertyHotelPrice->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 10));
		propertyHotelPrice->setFillColor(sf::Color::Black);

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
		std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Czynsz: ", getFont(), getFontSize() - 2));
		propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
		propertyRent1->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent2(new sf::Text(" posiadane 2:", getFont(), getFontSize() - 2));
		propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
		propertyRent2->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent3(new sf::Text(" posaidane 3:", getFont(), getFontSize() - 2));
		propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
		propertyRent3->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent4(new sf::Text(" posiadane 4:", getFont(), getFontSize() - 2));
		propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
		propertyRent4->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost1(
			new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
		propertyRentCost1->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 1));
		propertyRentCost1->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost2(
			new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
		propertyRentCost2->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 2));
		propertyRentCost2->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost3(
			new sf::Text(std::to_string(rents[2]), getFont(), getFontSize() - 2));
		propertyRentCost3->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 3));
		propertyRentCost3->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost4(
			new sf::Text(std::to_string(rents[3]), getFont(), getFontSize() - 2));
		propertyRentCost4->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
		propertyRentCost4->setFillColor(sf::Color::Black);

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
		std::shared_ptr<sf::Text> propertyRent(new sf::Text("Czynsz:", getFont(), getFontSize() - 2));
		propertyRent->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
		propertyRent->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent1(new sf::Text("posiadane 1: ", getFont(), getFontSize() - 2));
		propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
		propertyRent1->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost1(
			new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
		propertyRentCost1->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
		propertyRentCost1->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRent2(new sf::Text("posiadane 2: ", getFont(), getFontSize() - 2));
		propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
		propertyRent2->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyRentCost2(
			new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
		propertyRentCost2->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 7));
		propertyRentCost2->setFillColor(sf::Color::Black);

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
	const unsigned int LINE_LEN = 59;
	const unsigned int LINE_LEN_WITHOUT_PLAYER = LINE_LEN - std::string("Gracz X: ").length();
	unsigned int id = players_[index]->getId();
	if (std::string("Gracz " + std::to_string(id + 1) + ": " + text).size() <= LINE_LEN_WITHOUT_PLAYER) {
		notificationsWall_.addToWall("Gracz " + std::to_string(id + 1) + ": " + text);
	} else {
		notificationsWall_.addToWall(
			"Gracz " + std::to_string(id + 1) + ": " + text.substr(0, LINE_LEN_WITHOUT_PLAYER) + "-");
		for (unsigned int i = LINE_LEN_WITHOUT_PLAYER; i < text.size(); i += LINE_LEN_WITHOUT_PLAYER + 2) {
			notificationsWall_.addToWall("            " + text.substr(i, LINE_LEN_WITHOUT_PLAYER + 2));
		}
	}
	fileLoggerWrite("Gracz " + std::to_string(id + 1) + ": " + text);
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

void monopolyGameEngine::removePlayerFromGame(unsigned int playerIndexTurn, bool isDraw) {
	if (!isDraw) {
		// add player to bankrupted players vector and set his result place
		players_[playerIndexTurn]->setResultPlace(players_.size());
		playersBankrupted_.push_back(players_[playerIndexTurn]);

		// remove certain player from vector
		players_.erase(std::remove(players_.begin(), players_.end(), players_[playerIndexTurn]), players_.end());
	} else {
		int size = players_.size();
		for (auto player_ptr : players_) {
			// add all players to bankrupted players vector and set the same place for them
			player_ptr->setResultPlace(size);
			playersBankrupted_.push_back(player_ptr);
		}

		players_.clear();
	}
}

void monopolyGameEngine::makePlayerBankrupt(unsigned int playerIndexTurn) {
	// TODO
	// remove ownerships from fields
	for (unsigned int pos = 0; pos < gameboard_->getFieldNumber(); ++pos) {
		FieldType field_type = std::visit([](Field& field) { return field.getType(); }, gameboard_->getFieldById(pos));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(pos));
			if (field.getOwner() == players_[playerIndexTurn]) {
				field.setOwner(nullptr);
				field.createFlagSprite();
			}
		} else if (field_type == STATION) {
			StationField& field = std::get<StationField>(getBoard()->getFieldById(pos));
			if (field.getOwner() == players_[playerIndexTurn]) {
				field.setOwner(nullptr);
				field.createFlagSprite();
			}
		} else if (field_type == UTILITY) {
			UtilityField& field = std::get<UtilityField>(getBoard()->getFieldById(pos));
			if (field.getOwner() == players_[playerIndexTurn]) {
				field.setOwner(nullptr);
				field.createFlagSprite();
			}
		}
	}
	removePlayerFromGame(playerIndexTurn, false);
}

std::shared_ptr<Button> monopolyGameEngine::createDefaultButton(sf::String text, unsigned int width, unsigned int height) {
	sf::Vector2f buttonSize = sf::Vector2f(width, height);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonDefault(new Button(IDLE, text, buttonSize, getFontSize()));
	buttonDefault->setFont(getFont());
	buttonDefault->setActiveBackColor(activeButtonBackColor);
	buttonDefault->setActiveTextColor(activeButtonTextColor);
	buttonDefault->setInactiveBackColor(inActiveButtonBackColor);
	buttonDefault->setInactiveTextColor(inActiveButtonTextColor);
	buttonDefault->setFocusBackColor(FocusButtonBackColor);
	buttonDefault->setFocusTextColor(FocusButtonTextColor);
	buttonDefault->setIsClicked(false);
	buttonDefault->setIsVisible(false);
	buttonDefault->setIsActive(false);
	buttonDefault->setIsFocus(false);

	return buttonDefault;
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

Withdraw& monopolyGameEngine::getWithdraw() {
	return withdraw_;
}

void monopolyGameEngine::createChanceCards() {
	const std::string file_path = CHANCE_FILE_PATH_;
	const std::string graphic_path = "textures_and_fonts/textures/monopoly_single_square_chance.png";
	sf::Vector2f position = sf::Vector2f(500, 500);
	std::map<std::string, ChanceType> str_to_type = {{"MovementToProperty", MovementToProperty},
		{"MovementWithBuyOrPay", MovementWithBuyOrPay}, {"BankPaysYou", BankPaysYou},
		{"GetOutOfJailCard", GetOutOfJailCard}, {"MovementSpaces", MovementSpaces}, {"GoToJail", GoToJail},
		{"PayForHouseHotel", PayForHouseHotel}, {"Tax", Tax}, {"PayPlayers", PayPlayers}};

	std::ifstream f(file_path);
	json data = json::parse(f);

	for (auto& element : data) {
		unsigned int id = element["id"];
		std::string type_in_str = element["type"];
		unsigned int value = element["value"];
		std::string text_str = element["text"];
		sf::String text = (text_str);
		unsigned int width_from_file = element["width"];
		unsigned int height_from_file = element["height"];
		ChanceType type = str_to_type[type_in_str];

		ChanceCard card(id, type, value, text, graphic_path, width_from_file, height_from_file, position);
		chanceCards_.push_back(card);
	}

	shuffleChanceCards();
}

void monopolyGameEngine::shuffleChanceCards() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(chanceCards_), std::end(chanceCards_), g);
}

ChanceCard& monopolyGameEngine::getChanceCard() {
	return chanceCards_[chanceCardCurrent_];
}

void monopolyGameEngine::updateChanceCard() {
	++chanceCardCurrent_;
	if (chanceCardCurrent_ == chanceCards_.size()) {
		chanceCardCurrent_ = 0;
		shuffleChanceCards();
	}
}

bool monopolyGameEngine::gameFinishedCheckWinner() {
	return (players_.size() <= 1);	// check if one (or less???) player only is in game
}

bool monopolyGameEngine::gameFinishedCheckDraw() {
	return (gameTurnsGloballyDone_ >= GAME_TURNS_MAX);	// check turns global treshold
}

void monopolyGameEngine::gameTurnsCounterHandle() {
	gameTurnByPlayerDone_[players_[playerIndexturn_]->getId()] = true;

	for (auto player_ptr : players_) {
		if (gameTurnByPlayerDone_[player_ptr->getId()] == false) {
			return;	 // not all players done move in this turn (active players)
		}
	}

	gameTurnByPlayerDone_ = {false, false, false, false};
	++gameTurnsGloballyDone_;
}

void monopolyGameEngine::fileLoggerOpen()
{
	fileLogger =  std::ofstream(FILE_LOGGER_PATH, std::ofstream::out);
}

void monopolyGameEngine::fileLoggerWrite(std::string text)
{
	if(fileLogger.is_open())
	{
		fileLogger << text << std::endl;
	}
}

void monopolyGameEngine::fileLoggerClose()
{
	fileLogger.close();
}