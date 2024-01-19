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

// =============================================================================
// monopolyGameEngine Class Implementation
// =============================================================================

MonopolyGameEngine::MonopolyGameEngine() {
	turn_state_ = ROLL_DICE;
	// if (!house_texture_.loadFromFile("textures_and_fonts/textures/house.png")) {
	// 	// TODO: exception
	// }
	// if (!hotel_texture_.loadFromFile("textures_and_fonts/textures/hotel.png")) {
	// 	// TODO: exception
	// }

	fileLoggerOpen();
}

void MonopolyGameEngine::setScreenType(GameScreenType new_screen_type) {
	screen_type_ = new_screen_type;
}

GameScreenType MonopolyGameEngine::getScreenType() const {
	return screen_type_;
}

void MonopolyGameEngine::createBoard() {
	gameboard_ = std::make_shared<Board>(GAMEBOARD_FILE_PATH_);
}

void MonopolyGameEngine::createPlayers(std::vector<std::shared_ptr<Player>>& players_from_game_engine) {
	players_.clear();
	game_turn_by_player_done_ = {false, false, false, false};
	game_turns_globally_done_ = 0;
	players_starting_amount_ = 0;
	int playerId = 0;
	is_ai_game_only_ = true;
	for (auto player_ptr : players_from_game_engine) {
		++players_starting_amount_;
		if (player_ptr->getIsAi() == false) {
			is_ai_game_only_ = false;
			Player new_player = Player(PLAYER_MONEY_DEFAULT_);
			new_player.setIsAi(false);
			new_player.setAiLevel(0);
			new_player.setId(playerId);
			players_.push_back(std::make_shared<Player>(new_player));
		} else {
			AiPlayer new_player = AiPlayer(PLAYER_MONEY_DEFAULT_);
			new_player.setIsAi(true);
			new_player.setAiLevel(player_ptr->getAiLevel());
			new_player.setId(playerId);
			players_.push_back(std::make_shared<AiPlayer>(new_player));
		}
		++playerId;
	};

	int i = 0;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(players_), std::end(players_), g);

	for (int j = 0; j < PLAYERS_MAX_; ++j) {
		players_starting_Ids_[j] = 255;  // mark no player in game with this ID
	}

	for (unsigned int j = 0; j < players_.size(); ++j) {
		// save id of starting pplayers to future usage of data display
		players_starting_Ids_[j] = players_[j]->getId();
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

void MonopolyGameEngine::clearPlayers() {
	players_.clear();
}

void MonopolyGameEngine::clearBoard() {
	gameboard_->clearBoard();
}

std::shared_ptr<Board> MonopolyGameEngine::getBoard() {
	return gameboard_;
}

std::vector<std::shared_ptr<Player>>& MonopolyGameEngine::getPlayers() {
	return players_;
}

std::vector<std::shared_ptr<Player>> MonopolyGameEngine::getPlayersResult() {
	return players_bankrupted_;
}

void MonopolyGameEngine::setplayer_index_turn(unsigned int indx) {
	if (indx < PLAYERS_MAX_) {
		player_index_turn_ = indx;
	}
}

unsigned int MonopolyGameEngine::getplayer_index_turn() const {
	return player_index_turn_;
}

void MonopolyGameEngine::incplayer_index_turn() {
	++player_index_turn_;
	if (player_index_turn_ >= players_.size()) {
		player_index_turn_ = 0;
	}
}

bool MonopolyGameEngine::isButtonClicked(std::shared_ptr<Button> button_ptr) {
	if (button_ptr->getIsActive() && button_ptr->getWasReleased()) {
		button_ptr->setWasReleased(false);
		button_ptr->setIsActive(false);
		return true;
	}
	return false;
}

unsigned int MonopolyGameEngine::rollDice() const {	 // dices roll for 1-12 move
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 6);
	return dist(gen);
}

void MonopolyGameEngine::setTurnState(TurnState new_state) {
	turn_state_ = new_state;
}

TurnState MonopolyGameEngine::getTurnState() const {
	return turn_state_;
}

void MonopolyGameEngine::setAuctionState(AuctionState new_state) {
	auction_state_ = new_state;
}

AuctionState MonopolyGameEngine::getAuctionState() {
	return auction_state_;
}

unsigned int MonopolyGameEngine::getFontSize() const {
	return FONT_SIZE;
}

void MonopolyGameEngine::turnInfoTextShow() {
	turn_info_text_->setString("Tura: Gracz " + std::to_string(players_[getplayer_index_turn()]->getId() + 1));
}

unsigned int MonopolyGameEngine::getHouseCount() {
	return house_count_;
}

unsigned int MonopolyGameEngine::getHotelCount() {
	return hotel_count_;
}

void MonopolyGameEngine::setHouseCount(unsigned int new_count) {
	house_count_ = new_count;
}

void MonopolyGameEngine::setHotelCount(unsigned int new_count) {
	hotel_count_ = new_count;
}

void MonopolyGameEngine::addHouses(unsigned int added_amount) {
	setHouseCount(house_count_ + added_amount);
}

void MonopolyGameEngine::substractHouses(unsigned int substracted_amount) {
	setHouseCount(house_count_ - substracted_amount);
}

void MonopolyGameEngine::addHotels(unsigned int added_amount) {
	setHotelCount(hotel_count_ + added_amount);
}

void MonopolyGameEngine::substractHotels(unsigned int substracted_amount) {
	setHotelCount(hotel_count_ - substracted_amount);
}

void MonopolyGameEngine::performAuction() {
	static unsigned int current_bid;
	static unsigned int current_offer;
	static unsigned int bidded_property_id;
	static std::shared_ptr<Player> highest_bidder;
	static unsigned int player_bidding;
	static std::vector<std::shared_ptr<Player>> players_bidding;
	switch (getAuctionState()) {
		case INITIALIZATION: {
			current_bid = 10;
			bidded_property_id = players_[player_index_turn_]->getPosition();
			highest_bidder = nullptr;
			player_bidding = player_index_turn_;
			players_bidding = players_;
			bidder_info_text_->setString("Tura: Gracz " + std::to_string(players_bidding[player_bidding]->getId() + 1));
			highest_bid_info_text_->setString("Najwyzsza oferta: " + std::to_string(current_bid));
			if (highest_bidder != nullptr) {
				leading_bidder_info_text_->setString("Prowadzacy: Gracz " + std::to_string(highest_bidder->getId() + 1));
			}
			setAuctionState(PASS_BIDDING_TURN);
			break;
		}
		case PASS_BIDDING_TURN: {
			current_offer = current_bid + 1;
			setAuctionState(BIDDING);
			break;
		}
		case BIDDING: {
			// Główna logika aukcji
			if (players_bidding[player_bidding]->getIsAi()) {
				FieldType field_type = std::visit(
					[](Field& field) { return field.getType(); }, getBoard()->getFieldById(bidded_property_id));
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

				if (current_offer > players_bidding[player_bidding]->getMoney()) {
					current_offer = players_bidding[player_bidding]->getMoney();
				}

				players_bidding[player_bidding]->getAdapter().setSelectionState(bidded_property_id, 1);
			}
			if ((isButtonClicked(auction_bid_button_) || players_bidding[player_bidding]->getIsAi()) &&
				current_offer > current_bid) {
				highest_bidder = players_bidding[player_bidding];
				current_bid = current_offer;
				if (player_bidding == players_bidding.size() - 1) {
					player_bidding = 0;
				} else {
					++player_bidding;
				}
				bidder_info_text_->setString(
					"Tura: Gracz " + std::to_string(players_bidding[player_bidding]->getId() + 1));
				highest_bid_info_text_->setString("Najwyzsza oferta: " + std::to_string(current_bid));
				if (highest_bidder != nullptr) {
					leading_bidder_info_text_->setString(
						"Prowadzacy: Gracz " + std::to_string(highest_bidder->getId() + 1));
				}
				setAuctionState(PASS_BIDDING_TURN);
			}
			if (isButtonClicked(auction_resign_button_) || players_bidding[player_bidding]->getMoney() < current_bid ||
				(players_bidding[player_bidding]->getIsAi() && current_offer <= current_bid)) {
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
				bidder_info_text_->setString(
					"Tura: Gracz " + std::to_string(players_bidding[player_bidding]->getId() + 1));
			}
			if (isButtonClicked(add_1_to_offer_button_)) {
				if (current_offer + 1 <= players_bidding[player_bidding]->getMoney()) {
					current_offer += 1;
				} else {
					current_offer = players_bidding[player_bidding]->getMoney();
				}
			}
			if (isButtonClicked(add_10_to_offer_button_)) {
				if (current_offer + 10 <= players_bidding[player_bidding]->getMoney()) {
					current_offer += 10;
				} else {
					current_offer = players_bidding[player_bidding]->getMoney();
				}
			}
			if (isButtonClicked(add_100_to_offer_button_)) {
				if (current_offer + 100 <= players_bidding[player_bidding]->getMoney()) {
					current_offer += 100;
				} else {
					current_offer = players_bidding[player_bidding]->getMoney();
				}
			}
			if (isButtonClicked(substract_1_from_offer_button_)) {
				if (current_offer - 1 >= current_bid + 1 && current_offer > 1) {
					current_offer -= 1;
				} else {
					current_offer = current_bid + 1;
				}
			}
			if (isButtonClicked(substract_10_from_offer_button_)) {
				if (current_offer - 10 >= current_bid + 1 && current_offer > 10) {
					current_offer -= 10;
				} else {
					current_offer = current_bid + 1;
				}
			}
			if (isButtonClicked(substract_100_from_offer_button_)) {
				if (current_offer - 100 >= current_bid + 1 && current_offer > 100) {
					current_offer -= 100;
				} else {
					current_offer = current_bid + 1;
				}
			}
			current_offer_info_text_->setString("Aktualna oferta: " + std::to_string(current_offer));

			if ((players_bidding.size() == 1 && highest_bidder != nullptr) || players_bidding.size() == 0) {
				setAuctionState(ENDING);
			}
			break;
		}
		case ENDING: {
			if (highest_bidder != nullptr) {
				std::shared_ptr<Player> winner = players_bidding[0];
				addOwnerToPropertyField(winner, bidded_property_id);
				winner->addFieldOwnedId(bidded_property_id);
				winner->substractMoney(current_bid);
			} else {
				notificationAdd(player_index_turn_, "Aukcja - brak zwyciezcy!");
			}
			current_bid = 10;
			highest_bidder = nullptr;
			player_bidding = player_index_turn_;
			setAuctionState(NO_AUCTION);
		} break;
		default:
			break;
	}
}

bool MonopolyGameEngine::groupCompleted(std::vector<unsigned int> player_fields, PropertyField& field) const {
	std::vector<unsigned int> field_group_members = field.getGroupMembers();
	for (auto group_member : field_group_members) {
		if (std::find(player_fields.cbegin(), player_fields.cend(), group_member) == player_fields.cend()) {
			return false;
		}
	}
	return true;
}

unsigned int MonopolyGameEngine::calculateGroupFieldsOwned(std::vector<unsigned int> player_fields,
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

bool MonopolyGameEngine::isBuildingLegal(std::shared_ptr<Player> builder, StreetField field) {
	std::vector<unsigned int> builder_ownes = builder->getFieldOwnedId();
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

bool MonopolyGameEngine::isDestroyingLegal(std::shared_ptr<Player> builder, StreetField field) {
	std::vector<unsigned int> builder_ownes = builder->getFieldOwnedId();
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

bool MonopolyGameEngine::isHotelBuildingLegal(std::shared_ptr<Player> builder, StreetField& field) {
	std::vector<unsigned int> builder_ownes = builder->getFieldOwnedId();
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

bool MonopolyGameEngine::isHotelDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field) {
	std::vector<unsigned int> builder_ownes = builder->getFieldOwnedId();
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

bool MonopolyGameEngine::colorGroupEmpty(std::shared_ptr<Player> mortaging, StreetField& field) {
	std::vector<unsigned int> mortaging_ownes = mortaging->getFieldOwnedId();
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

unsigned int MonopolyGameEngine::calculateRent(unsigned int rolled_val, int pos) {
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
		} else if (groupCompleted(field.getOwner()->getFieldOwnedId(), field)) {
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
			std::vector<unsigned int> player_owns = field.getOwner()->getFieldOwnedId();
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
			std::vector<unsigned int> player_owns = field.getOwner()->getFieldOwnedId();
			const unsigned int utility_owned = calculateGroupFieldsOwned(player_owns, field);
			std::map<unsigned int, UtilityTiers> utility_number_map = {{1, ONE_UTILITY}, {2, TWO_UTILITIES}};
			UtilityTiers utility_tier = utility_number_map[utility_owned];
			rent_to_pay = rolled_val * rent_multipliers[utility_tier];
		}
	}
	return rent_to_pay;
}

void MonopolyGameEngine::movePlayer(unsigned int turn_index, unsigned int position_increment) {
	int old_pos = players_[turn_index]->getPosition();
	int new_pos = (old_pos + position_increment) % 40;
	players_[turn_index]->setPosition(new_pos);
	sf::Vector2f newPlayerSpritePos = getUpdatePlayerSpritePosition();
	players_[turn_index]->setSpritePosition(newPlayerSpritePos);
}

void MonopolyGameEngine::sendToJail(unsigned int turn_index) {
	const unsigned int JAIL_ID = 10;
	players_[turn_index]->setPosition(JAIL_ID);
	sf::Vector2f newPlayerSpritePos = getUpdatePlayerSpritePosition();
	players_[turn_index]->setSpritePosition(newPlayerSpritePos);
}

void MonopolyGameEngine::handlePassingStart(unsigned int old_pos, unsigned int new_pos) {
	if (new_pos < old_pos) {	// start passed
		players_[player_index_turn_]->addMoney(START_PASSING_MONEY_);
	}
}

void MonopolyGameEngine::showAllPropertiesWorker() {
	if (isButtonClicked(next_property_button_)) {
		++current_property_showed_;
		if (current_property_showed_ == 40) {
			current_property_showed_ = 1;
		}
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		while (!(field_type == STREET || field_type == STATION || field_type == UTILITY)) {
			++current_property_showed_;
			if (current_property_showed_ == 40) {
				current_property_showed_ = 1;
			}
			field_type = std::visit(
				[](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		}
		clearPropertyData(false);
		showPropertyData(current_property_showed_, false);
	}

	if (isButtonClicked(previous_property_button_)) {
		if (current_property_showed_ == 0) {
			current_property_showed_ = 39;
		} else {
			--current_property_showed_;
		}
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		while (!(field_type == STREET || field_type == STATION || field_type == UTILITY)) {
			if (current_property_showed_ == 0) {
				current_property_showed_ = 39;
			} else {
				--current_property_showed_;
			}
			field_type = std::visit(
				[](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		}
		clearPropertyData(false);
		showPropertyData(current_property_showed_, false);
	}
}

void MonopolyGameEngine::buildingsManagingWorker() {
	if (isButtonClicked(buy_house_button_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		if (field_type == STREET) {
			// Ostatnia poprawka - dodałem referencje - sprawdzić czy poprawa
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(current_property_showed_));
			if (isBuildingLegal(players_[player_index_turn_], field)) {
				players_[player_index_turn_]->substractMoney(field.getHousePrice());
				field.setHouseNumber(field.getHouseNumber() + 1);
				substractHouses(1);
				notificationAdd(player_index_turn_, "Kupiono dom!");
			} else {
				notificationAdd(player_index_turn_, "Nie mozna kupic domu");
			}
		} else {
			notificationAdd(player_index_turn_, "Nie mozna kupic domu na tej nieruchomosci");
		}
	}
	if (isButtonClicked(sell_house_button_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(current_property_showed_));
			if (isDestroyingLegal(players_[player_index_turn_], field)) {
				players_[player_index_turn_]->addMoney(field.getHousePrice() / 2);
				field.setHouseNumber(field.getHouseNumber() - 1);
				addHouses(1);
				notificationAdd(player_index_turn_, "Sprzedano dom!");
			} else {
				notificationAdd(player_index_turn_, "Nie mozna sprzedac domu");
			}
		} else {
			notificationAdd(player_index_turn_, "Nie mozna sprzedac domu na tej nieruchomosci");
		}
	}
	if (isButtonClicked(buy_hotel_button_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(current_property_showed_));
			if (isHotelBuildingLegal(players_[player_index_turn_], field)) {
				players_[player_index_turn_]->substractMoney(field.getHotelPrice());
				field.setIsHotel(true);
				substractHotels(1);
				addHouses(4);
				notificationAdd(player_index_turn_, "Kupiono hotel!");
			} else {
				notificationAdd(player_index_turn_, "Nie mozna kupic hotelu");
			}
		} else {
			notificationAdd(player_index_turn_, "Nie mozna kupic hotelu na tej nieruchomosci");
		}
	}
	if (isButtonClicked(sell_hotel_button_)) {
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(current_property_showed_));
			if (isHotelDestroyingLegal(players_[player_index_turn_], field)) {
				players_[player_index_turn_]->addMoney(field.getHotelPrice() / 2);
				field.setIsHotel(false);
				addHotels(1);
				substractHouses(4);
				notificationAdd(player_index_turn_, "Sprzedano hotel!");
			} else {
				notificationAdd(player_index_turn_, "Nie mozna sprzedac hotelu");
			}
		} else {
			notificationAdd(player_index_turn_, "Nie mozna sprzedac hotelu na tej nieruchomosci");
		}
	}
	if (isButtonClicked(mortgage_button_)) {
		std::shared_ptr<Player> curr_player = players_[player_index_turn_];
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(current_property_showed_));
			if (field.getOwner() == curr_player && !field.getIsMortgaged() && colorGroupEmpty(curr_player, field)) {
				curr_player->addMoney(field.getMortgage());
				field.setIsMortgaged(true);
				notificationAdd(player_index_turn_, "Zastawiono nieruchomosc " + field.getName());
			} else {
				notificationAdd(player_index_turn_, "Nie moza zastawic nieruchomosci " + field.getName());
			}
		} else if (field_type == STATION) {
			StationField& field = std::get<StationField>(getBoard()->getFieldById(current_property_showed_));
			if (field.getOwner() == curr_player && !field.getIsMortgaged()) {
				curr_player->addMoney(field.getMortgage());
				field.setIsMortgaged(true);
				notificationAdd(player_index_turn_, "Zastawiono nieruchomosc " + field.getName());
			} else {
				notificationAdd(player_index_turn_, "Nie moza zastawic nieruchomosci " + field.getName());
			}
		} else if (field_type == UTILITY) {
			UtilityField& field = std::get<UtilityField>(getBoard()->getFieldById(current_property_showed_));
			if (field.getOwner() == curr_player && !field.getIsMortgaged()) {
				curr_player->addMoney(field.getMortgage());
				field.setIsMortgaged(true);
				notificationAdd(player_index_turn_, "Zastawiono nieruchomosc " + field.getName());
			} else {
				notificationAdd(player_index_turn_, "Nie moza zastawic nieruchomosci " + field.getName());
			}
		}
	}
	if (isButtonClicked(unmortgage_button_)) {
		std::shared_ptr<Player> curr_player = players_[player_index_turn_];
		FieldType field_type =
			std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(current_property_showed_));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(current_property_showed_));
			if (field.getOwner() == curr_player && field.getIsMortgaged() &&
				curr_player->getMoney() >= field.getUnmortgageValue()) {
				curr_player->substractMoney(field.getUnmortgageValue());
				field.setIsMortgaged(false);
				notificationAdd(player_index_turn_, "Wykupiono nieruchomosc " + field.getName());
			} else {
				notificationAdd(player_index_turn_, "Nie mozna wykupic nieruchomosci " + field.getName());
			}
		} else if (field_type == STATION) {
			StationField& field = std::get<StationField>(getBoard()->getFieldById(current_property_showed_));
			if (field.getOwner() == curr_player && field.getIsMortgaged() &&
				curr_player->getMoney() >= field.getUnmortgageValue()) {
				curr_player->substractMoney(field.getUnmortgageValue());
				field.setIsMortgaged(false);
				notificationAdd(player_index_turn_, "Wykupiono nieruchomosc " + field.getName());
			} else {
				notificationAdd(player_index_turn_, "Nie mozna wykupic nieruchomosci " + field.getName());
			}
		} else if (field_type == UTILITY) {
			UtilityField& field = std::get<UtilityField>(getBoard()->getFieldById(current_property_showed_));
			if (field.getOwner() == curr_player && field.getIsMortgaged() &&
				curr_player->getMoney() >= field.getUnmortgageValue()) {
				curr_player->substractMoney(field.getUnmortgageValue());
				field.setIsMortgaged(false);
				notificationAdd(player_index_turn_, "Wykupiono nieruchomosc " + field.getName());
			} else {
				notificationAdd(player_index_turn_, "Nie mozna wykupic nieruchomosci " + field.getName());
			}
		}
	}
}

void MonopolyGameEngine::aiBuildingsMangingWorker() {
	std::vector<std::vector<unsigned int>> SETS = {
		{1, 3}, {6, 8, 9}, {11, 13, 14}, {16, 18, 19}, {21, 23, 24}, {26, 27, 29}, {31, 32, 34}, {37, 39}};
	std::shared_ptr<Player> curr_player = players_[player_index_turn_];
	std::vector<unsigned int> fields_owned = curr_player->getFieldOwnedId();
	unsigned int owned_amount = fields_owned.size();

	for (unsigned int set = 0; set < SETS.size(); ++set) {
		if (std::includes(fields_owned.begin(), fields_owned.end(), SETS[set].begin(), SETS[set].end())) {
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

			if (set == 0 || set == 7) {
				last = 1;
			}

			for (unsigned int house = 0; house < decision; ++house) {
				// find field with highest amount of houses
				unsigned int highest_field_id = 0;
				StreetField highest_field =
					std::get<StreetField>(gameboard_->getFieldById(SETS[set][highest_field_id]));

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

				StreetField& field_to_build =
					std::get<StreetField>(gameboard_->getFieldById(SETS[set][highest_field_id]));

				if (isHotelDestroyingLegal(curr_player, field_to_build)) {
					curr_player->addMoney(field_to_build.getHotelPrice() / 2);
					field_to_build.setIsHotel(false);
					addHotels(1);
					substractHouses(4);
					notificationAdd(player_index_turn_, "Hotel zburzony na polu " + field_to_build.getName());
				} else if (isDestroyingLegal(curr_player, field_to_build)) {
					curr_player->addMoney(field_to_build.getHousePrice() / 2);
					field_to_build.setHouseNumber(field_to_build.getHouseNumber() - 1);
					addHouses(1);
					notificationAdd(player_index_turn_, "Dom zburzony na polu " + field_to_build.getName());
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
			unsigned int afford_max = (unsigned int)floor(curr_player->getMoney() / (float)house_price);

			max_build = std::min(max_build, afford_max);

			unsigned int player_id = curr_player->getId();
			curr_player->getAdapter().setTurn(player_id);
			curr_player->getAdapter().setSelectionState(SETS[set][0], 1);

			unsigned int decision = curr_player->decideBuildHouse();
			decision = std::min(decision, max_build);

			curr_player->getAdapter().setSelectionState(SETS[set][0], 0);

			unsigned int last = 2;

			if (set == 0 || set == 7) {
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

				StreetField& field_to_build =
					std::get<StreetField>(gameboard_->getFieldById(SETS[set][lowest_field_id]));

				if (isHotelBuildingLegal(curr_player, field_to_build)) {
					players_[player_index_turn_]->substractMoney(field_to_build.getHotelPrice());
					field_to_build.setIsHotel(true);
					substractHotels(1);
					addHouses(4);
					notificationAdd(player_index_turn_, "Hotel zbudowany na polu " + field_to_build.getName());
				} else if (isBuildingLegal(curr_player, lowest_field)) {
					players_[player_index_turn_]->substractMoney(field_to_build.getHousePrice());
					field_to_build.setHouseNumber(field_to_build.getHouseNumber() + 1);
					substractHouses(1);
					notificationAdd(player_index_turn_, "Dom zbudowany na polu " + field_to_build.getName());
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
						notificationAdd(player_index_turn_, "Wykupiono nieruchomosc " + field.getName());
					}
				}
			} else if (colorGroupEmpty(curr_player, field)) {
				unsigned int player_id = curr_player->getId();
				curr_player->getAdapter().setTurn(player_id);
				curr_player->getAdapter().setSelectionState(field.getId(), 1);
				Decision mortgage_decision = curr_player->decideMortgage(field.getId());
				curr_player->getAdapter().setSelectionState(field.getId(), 0);

				if (mortgage_decision == YES) {
					curr_player->addMoney(field.getMortgage());
					field.setIsMortgaged(true);
					notificationAdd(player_index_turn_, "Zastawiono nieruchomosc " + field.getName());
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
						notificationAdd(player_index_turn_, "Wykupiono nieruchomosc " + field.getName());
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
					notificationAdd(player_index_turn_, "Zastawiono nieruchomosc " + field.getName());
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
						notificationAdd(player_index_turn_, "Wykupiono nieruchomosc " + field.getName());
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
					notificationAdd(player_index_turn_, "Zastawiono nieruchomosc " + field.getName());
				}
			}
		}
	}
}

void MonopolyGameEngine::boardToAuctionSwitchHandler(bool is_auction) {
	roll_dice_button_->setIsVisible(!is_auction);
	buy_field_button_->setIsVisible(!is_auction);
	resign_buy_field_button_->setIsVisible(!is_auction);
	next_property_button_->setIsVisible(!is_auction);
	previous_property_button_->setIsVisible(!is_auction);
	buy_house_button_->setIsVisible(!is_auction);
	sell_house_button_->setIsVisible(!is_auction);
	buy_hotel_button_->setIsVisible(!is_auction);
	sell_hotel_button_->setIsVisible(!is_auction);
	bankrupt_button_->setIsVisible(!is_auction);
	next_turn_button_->setIsVisible(!is_auction);
	withdraw_button_->setIsVisible(!is_auction);

	if (is_auction) {
		PROPERTY_DATA_POSITION = sf::Vector2f(200, 300);
	} else {
		PROPERTY_DATA_POSITION = sf::Vector2f(910, 260);
	}

	clearPropertyData(true);
	showPropertyData(players_[player_index_turn_]->getPosition(), true);

	add_1_to_offer_button_->setIsVisible(is_auction);
	add_10_to_offer_button_->setIsVisible(is_auction);
	add_100_to_offer_button_->setIsVisible(is_auction);
	substract_1_from_offer_button_->setIsVisible(is_auction);
	substract_10_from_offer_button_->setIsVisible(is_auction);
	substract_100_from_offer_button_->setIsVisible(is_auction);
	auction_bid_button_->setIsVisible(is_auction);
	auction_resign_button_->setIsVisible(is_auction);
}

void MonopolyGameEngine::withdrawWorker() {
	if (isButtonClicked(withdraw_button_)) {	 // player decided to trade
		if (getTurnState() == ROLL_DICE || getTurnState() == TURN_END || getTurnState() == PAY_RENT) {
			getWithdraw().setTurnState(getTurnState());
			setTurnState(WITHDRAW_ONGOING);
			setScreenType(WITHDRAW_CHOOSE_PLAYER);
			getWithdraw().setChooseScreenVisible(true);
			getWithdraw().setPlayer1ToWithdraw(players_[player_index_turn_]);
			getWithdraw().getPlayer1Button()->setIsVisible(false);
			getWithdraw().getPlayer2Button()->setIsVisible(false);
			getWithdraw().getPlayer3Button()->setIsVisible(false);
			getWithdraw().getPlayer4Button()->setIsVisible(false);
			for (auto player_ptr : getPlayers()) {
				if (player_ptr->getId() == 0 && player_ptr->getId() != players_[player_index_turn_]->getId()) {
					getWithdraw().getPlayer1Button()->setIsVisible(true);
				} else if (player_ptr->getId() == 1 && player_ptr->getId() != players_[player_index_turn_]->getId()) {
					getWithdraw().getPlayer2Button()->setIsVisible(true);
				} else if (player_ptr->getId() == 2 && player_ptr->getId() != players_[player_index_turn_]->getId()) {
					getWithdraw().getPlayer3Button()->setIsVisible(true);
				} else if (player_ptr->getId() == 3 && player_ptr->getId() != players_[player_index_turn_]->getId()) {
					getWithdraw().getPlayer4Button()->setIsVisible(true);
				}
			}
		} else {
			notificationAdd(player_index_turn_, " jest zajety, nie moze teraz dokonac wymiany");
		}
	}
}

void MonopolyGameEngine::aiWithdrawWorker() {
	const unsigned int TRADE_ATTEMPTS = 4;
	const unsigned int TRADE_ITEM_MAX = 6;
	const unsigned int TRADE_MONEY_MAX = 500;

	std::vector<std::shared_ptr<Player>> candidates;
	for (auto player : players_) {
		if (player != players_[player_index_turn_]) {
			candidates.push_back(player);
		}
	}

	if ((getTurnState() == TURN_END || getTurnState() == PAY_RENT) && players_[player_index_turn_]->getIsAi()) {
		getWithdraw().setTurnState(getTurnState());
		for (unsigned int t = 0; t < TRADE_ATTEMPTS; ++t) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist_offer(
				0, std::min((unsigned int)players_[player_index_turn_]->getFieldOwnedId().size(), TRADE_ITEM_MAX));
			unsigned int offer = dist_offer(gen);

			std::uniform_int_distribution<> dist_select(0, candidates.size() - 1);
			unsigned int selected_player_index = dist_select(gen);
			std::shared_ptr<Player> selected_player = candidates[selected_player_index];
			bool trade_with_ai = selected_player->getIsAi();

			std::uniform_int_distribution<> dist_rec(
				0, std::min((unsigned int)selected_player->getFieldOwnedId().size(), TRADE_ITEM_MAX));
			unsigned int receive = dist_rec(gen);

			std::uniform_int_distribution<> dist_money_given(
				0, std::min((unsigned int)players_[player_index_turn_]->getMoney(), TRADE_MONEY_MAX));
			unsigned int money_given = dist_money_given(gen);

			std::uniform_int_distribution<> dist_money_receiven(
				0, std::min((unsigned int)players_[player_index_turn_]->getMoney(), TRADE_MONEY_MAX));
			unsigned int money_receiven = dist_money_receiven(gen);

			int money_balance = money_given - money_receiven;

			if ((offer == 0 && money_given == 0) || (receive == 0 && money_receiven == 0)) {
				continue;
			}

			std::vector<unsigned int> properties_offered;
			std::vector<unsigned int> possible_to_offer = players_[player_index_turn_]->getFieldOwnedId();

			for (unsigned int i = 0; i < offer; ++i) {
				std::uniform_int_distribution<> dist_possible(0, (unsigned int)possible_to_offer.size() - 1);
				unsigned int selection = dist_possible(gen);

				properties_offered.push_back(possible_to_offer[selection]);
				possible_to_offer.erase(
					std::find(possible_to_offer.begin(), possible_to_offer.end(), possible_to_offer[selection]));
			}

			std::vector<unsigned int> properties_receiven;
			std::vector<unsigned int> possible_to_receive = selected_player->getFieldOwnedId();

			for (unsigned int i = 0; i < receive; ++i) {
				std::uniform_int_distribution<> dist_possible(0, (unsigned int)possible_to_receive.size() - 1);
				unsigned int selection = dist_possible(gen);

				properties_receiven.push_back(possible_to_receive[selection]);
				possible_to_receive.erase(
					std::find(possible_to_receive.begin(), possible_to_receive.end(), possible_to_receive[selection]));
			}

			// neurons setup for trade
			for (int i = 0; i < properties_offered.size(); ++i) {
				players_[player_index_turn_]->getAdapter().setSelectionState(properties_offered[i], 1);
				if (trade_with_ai) {
					selected_player->getAdapter().setSelectionState(properties_offered[i], 1);
				}
			}

			for (int i = 0; i < properties_receiven.size(); ++i) {
				players_[player_index_turn_]->getAdapter().setSelectionState(properties_receiven[i], 1);
				if (trade_with_ai) {
					selected_player->getAdapter().setSelectionState(properties_receiven[i], 1);
				}
			}

			players_[player_index_turn_]->getAdapter().setMoneyContext(money_balance);
			if (trade_with_ai) {
				selected_player->getAdapter().setMoneyContext(money_balance);
			}

			Decision player_1_decision = players_[player_index_turn_]->decideOfferTrade();

			if (player_1_decision == NO) {
				players_[player_index_turn_]->getAdapter().clearSelectionState();
				if (trade_with_ai) {
					selected_player->getAdapter().clearSelectionState();
				}
				continue;
			}

			// Perform trade
			getWithdraw().setPlayer1ToWithdraw(players_[player_index_turn_]);
			getWithdraw().setPlayer2ToWithdraw(selected_player);

			setTurnState(WITHDRAW_ONGOING);
			setScreenType(WITHDRAW_ADD_VALUE);
			getWithdraw().setChooseScreenVisible(false);
			getWithdraw().setValueScreenVisible(true);

			getWithdraw().moneyTransferIndex(1, money_given);
			getWithdraw().moneyTransferIndex(2, money_receiven);

			getWithdraw().setPlayer1IndexProperties(properties_offered);
			getWithdraw().setPlayer2IndexProperties(properties_receiven);

			if (trade_with_ai) {
				Decision player_2_decision = selected_player->decideAcceptTrade();

				if (player_2_decision == NO) {
					players_[player_index_turn_]->getAdapter().clearSelectionState();
					selected_player->getAdapter().clearSelectionState();

					getWithdraw().setChooseScreenVisible(false);
					getWithdraw().setValueScreenVisible(false);
					getWithdraw().setDecisionScreenVisible(false);
					setScreenType(BOARDGAME);
					setTurnState(getWithdraw().getTurnState());
					getWithdraw().setPlayer2ToWithdraw(nullptr);
					continue;
				}

				std::string trade_raport = "Przeprowadzil wymiane z graczem " +
										   std::to_string(selected_player->getId()) +
										   " Oddal: " + std::to_string(money_given) + " i pola: ";
				for (auto field_id : properties_offered) {
					trade_raport +=
						std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(field_id));
					trade_raport += ", ";
				}
				trade_raport += "Otrzymal: " + std::to_string(money_receiven) + " i pola: ";
				for (auto field_id : properties_receiven) {
					trade_raport +=
						std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(field_id));
					trade_raport += ", ";
				}
				trade_raport += ".";
				notificationAdd(player_index_turn_, trade_raport);

				getWithdraw().makeWithdraw();
				getWithdraw().setChooseScreenVisible(false);
				getWithdraw().setValueScreenVisible(false);
				getWithdraw().setDecisionScreenVisible(false);
				setScreenType(BOARDGAME);
				setTurnState(getWithdraw().getTurnState());
				getWithdraw().setPlayer2ToWithdraw(nullptr);

			} else {
				setScreenType(WITHDRAW_DECISION);
				getWithdraw().setDecisionScreenVisible(true);
				getWithdraw().setValueScreenVisible(false);
			}
		}
	}
}

bool MonopolyGameEngine::monopolyGameWorker() {
	try {
		updateTextPlayersInfo();
		updateAvailableHousesHotelText();
		showAllPropertiesWorker();
		if (players_[player_index_turn_]->getIsAi()) {
			aiWithdrawWorker();
		} else {
			withdrawWorker();
		}
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

		if (isButtonClicked(bankrupt_button_) ||
			(players_[player_index_turn_]->getIsAi() && ai_bankrupted)) {	 // player decied to go bankrupt
			playerBankrutedNow = true;
			notificationAdd(player_index_turn_, "Oglosil bankructwo!");
			roll_dice_button_->setIsVisible(false);
			buy_field_button_->setIsVisible(false);
			resign_buy_field_button_->setIsVisible(false);
			if (players_[player_index_turn_]->getIsAi()) {
				ai_bankrupted = false;
			}
			setTurnState(TURN_END);	 // for next player
		}

		switch (getTurnState()) {
			case ROLL_DICE: {
				playerBankrutedNow = false;
				unsigned int player_jail_status = players_[player_index_turn_]->getJailStatus();
				if (!players_[player_index_turn_]->getIsAi()) {
					buildingsManagingWorker();
				} else {
					aiBuildingsMangingWorker();
				}
				if (player_jail_status != 0) {
					jail_pay_button_->setIsVisible(true);
				} else {
					jail_pay_button_->setIsVisible(false);
				}

				if (player_jail_status != 0) {
					JailDecision decision;
					if (players_[player_index_turn_]->getIsAi()) {
						decision = players_[player_index_turn_]->decideJail();
					}
					if (isButtonClicked(jail_pay_button_) || (players_[player_index_turn_]->getIsAi() && decision == PAY))
						if (players_[player_index_turn_]->getMoney() < JAIL_PAY_MONEY) {
							std::string notification_msg = "Brak kasy aby zaplacic kaucje";
							notificationAdd(player_index_turn_, notification_msg);
						} else {
							players_[player_index_turn_]->setJailStatus(0);
							players_[player_index_turn_]->substractMoney(JAIL_BAILOUT);
							std::string notification_msg = "Wychodzi z wiezienia przez kaucje";
							notificationAdd(player_index_turn_, notification_msg);
							jail_pay_button_->setIsVisible(false);
						}
				}

				if (isButtonClicked(roll_dice_button_) || players_[player_index_turn_]->getIsAi()) {
					unsigned int roll1 = rollDice();
					unsigned int roll2 = rollDice();
					std::string val1 = std::to_string(roll1);
					std::string val2 = std::to_string(roll2);
					rolled_val = roll1 + roll2;
					std::string rol = "Rzucona liczba: ";
					std::string val = std::to_string(rolled_val);
					rolled_value_text_->setString(rol + val);
					notificationAdd(player_index_turn_, rol + val + " -> (" + val1 + ", " + val2 + ")");

					if (player_jail_status == 0) {
						if (roll1 == roll2) {
							isDouble = true;
							double_turns += 1;
						} else {
							isDouble = false;
						}

						if (double_turns == 3) {
							std::string notification_msg = "Idzie do wiezienia przez rzuty kostkami";
							sendToJail(player_index_turn_);
							players_[player_index_turn_]->setJailStatus(3);
							notificationAdd(player_index_turn_, notification_msg);

							roll_dice_button_->setIsVisible(false);
							setTurnState(TURN_END);
						} else {
							int old_pos = players_[player_index_turn_]->getPosition();
							movePlayer(player_index_turn_, rolled_val);
							int new_pos = players_[player_index_turn_]->getPosition();
							handlePassingStart(old_pos, new_pos);

							roll_dice_button_->setIsVisible(false);
							setTurnState(FIELD_ACTION);
						}
					} else {
						if (roll1 == roll2) {
							players_[player_index_turn_]->setJailStatus(0);

							int old_pos = players_[player_index_turn_]->getPosition();
							movePlayer(player_index_turn_, rolled_val);
							int new_pos = players_[player_index_turn_]->getPosition();
							handlePassingStart(old_pos, new_pos);

							std::string notification_msg = "Wychodzi z wiezienia przez rzuty kostkami";
							notificationAdd(player_index_turn_, notification_msg);

							roll_dice_button_->setIsVisible(false);
							jail_pay_button_->setIsVisible(false);
							setTurnState(FIELD_ACTION);
						} else if (player_jail_status == 1) {
							if (players_[player_index_turn_]->getMoney() < JAIL_PAY_MONEY) {
								money_to_find = JAIL_PAY_MONEY;
								bank_pay_rent = true;
								players_to_pay_rent.clear();
								setTurnState(PAY_RENT);
								std::string notification_msg = "Brak kasy aby wyjsc z wiezienia";
								notificationAdd(player_index_turn_, notification_msg);
							}

							players_[player_index_turn_]->setJailStatus(0);
							players_[player_index_turn_]->substractMoney(JAIL_BAILOUT);

							int old_pos = players_[player_index_turn_]->getPosition();
							movePlayer(player_index_turn_, rolled_val);
							int new_pos = players_[player_index_turn_]->getPosition();
							handlePassingStart(old_pos, new_pos);

							roll_dice_button_->setIsVisible(false);
							jail_pay_button_->setIsVisible(false);
							setTurnState(FIELD_ACTION);

							std::string notification_msg = "Opuszcza wiezienia po czasie odsiadki";
							notificationAdd(player_index_turn_, notification_msg);

						} else {
							players_[player_index_turn_]->reduceJailStatus();

							roll_dice_button_->setIsVisible(false);
							jail_pay_button_->setIsVisible(false);

							setTurnState(TURN_END);
						}
					}
				}
			} break;
			case FIELD_ACTION: {
				int pos = players_[player_index_turn_]->getPosition();
				FieldType field_type =
					std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));

				std::string textWhereIsPlayer("jest na polu " + std::visit([](Field& field) { return field.getName(); },
																	getBoard()->getFieldById(pos)));
				notificationAdd(player_index_turn_, textWhereIsPlayer);

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
					} else if (owner->getId() != players_[player_index_turn_]->getId()) {
						unsigned int rent_to_pay = calculateRent(rolled_val, pos);
						std::string notification("Czynsz do zaplaty: " + std::to_string(rent_to_pay));
						notificationAdd(player_index_turn_, notification);
						if (players_[player_index_turn_]->getMoney() >= rent_to_pay) {
							players_[player_index_turn_]->substractMoney(rent_to_pay);
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
						notificationAdd(player_index_turn_, "Brak akcji - gracz jest wlascicielem pola");
						setTurnState(TURN_END);
					}
				} else if (field_type == TAX) {
					TaxField field = std::get<TaxField>(getBoard()->getFieldById(pos));
					unsigned int tax_to_pay = field.getTaxValue();
					if (players_[player_index_turn_]->getMoney() >= tax_to_pay) {
						players_[player_index_turn_]->substractMoney(tax_to_pay);
						setTurnState(TURN_END);
					} else {
						money_to_find = tax_to_pay;
						bank_pay_rent = true;
						players_to_pay_rent.clear();
						setTurnState(PAY_RENT);
					}
				} else if (field_type == GO_TO_JAIL) {
					std::string notification_msg = "Idzie to wiezienia przez pole IDZ DO WIEZIENIA";
					notificationAdd(player_index_turn_, notification_msg);
					sendToJail(player_index_turn_);
					players_[player_index_turn_]->setJailStatus(3);
					setTurnState(TURN_END);
				} else if (field_type == CHANCE) {
					ChanceCard chance_card = getChanceCard();
					updateChanceCard();
					std::string notification_msg = "Karta Szansy: ";
					notificationAdd(player_index_turn_, notification_msg + chance_card.getText());

					switch (chance_card.getType()) {
						case MovementToProperty: {
							int old_pos = players_[player_index_turn_]->getPosition();
							int posIncrement = (40 + chance_card.getValue()) - old_pos;
							movePlayer(player_index_turn_, posIncrement);
							int new_pos = chance_card.getValue();
							handlePassingStart(old_pos, new_pos);
							setTurnState(FIELD_ACTION);
						} break;

						case BankPaysYou:
							players_[player_index_turn_]->addMoney(chance_card.getValue());
							setTurnState(TURN_END);
							break;

						case GetOutOfJailCard:
							players_[player_index_turn_]->setJailCards(players_[player_index_turn_]->getJailCards() + 1);
							setTurnState(TURN_END);
							break;

						case GoToJail:
							sendToJail(player_index_turn_);
							players_[player_index_turn_]->setJailStatus(3);
							setTurnState(TURN_END);
							break;

						case PayForHouseHotel: {
							unsigned int sum = 0;
							for (auto field_id : players_[player_index_turn_]->getFieldOwnedId()) {
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
								std::string notification_msg = "Kwota do zaplaty: " + std::to_string(sum);
								notificationAdd(player_index_turn_, notification_msg);
								setTurnState(TURN_END);
							} else {
								if (players_[player_index_turn_]->getMoney() >= sum) {
									players_[player_index_turn_]->substractMoney(sum);
									std::string notification_msg = "Zaplacil bankowi: " + std::to_string(sum);
									notificationAdd(player_index_turn_, notification_msg);
									setTurnState(TURN_END);
								} else {
									money_to_find = sum;
									bank_pay_rent = true;
									players_to_pay_rent.clear();
									setTurnState(PAY_RENT);
									std::string notification_msg = "Brak kasy. Potrzeba: " + std::to_string(sum);
									notificationAdd(player_index_turn_, notification_msg);
								}
							}
						} break;

						case Tax: {
							if ((int)players_[player_index_turn_]->getMoney() >= chance_card.getValue()) {
								players_[player_index_turn_]->substractMoney(chance_card.getValue());
								std::string notification_msg =
									"Zaplacil bankowi: " + std::to_string(chance_card.getValue());
								notificationAdd(player_index_turn_, notification_msg);
								setTurnState(TURN_END);
							} else {
								money_to_find = chance_card.getValue();
								;
								bank_pay_rent = true;
								players_to_pay_rent.clear();
								setTurnState(PAY_RENT);
								std::string notification_msg =
									"Brak kasy. Potrzeba: " + std::to_string(chance_card.getValue());
								notificationAdd(player_index_turn_, notification_msg);
							}

						} break;

						case MovementSpaces: {
							int old_pos = players_[player_index_turn_]->getPosition();
							int posIncrement = chance_card.getValue();
							movePlayer(player_index_turn_, posIncrement);
							int new_pos = players_[player_index_turn_]->getPosition();
							if (posIncrement >= 0) {
								handlePassingStart(old_pos, new_pos);
							}
							setTurnState(FIELD_ACTION);
						} break;

						case PayPlayers: {
							unsigned int toPay = chance_card.getValue() * (players_.size() - 1);

							if (players_[player_index_turn_]->getMoney() >= toPay) {
								for (auto player_ptr : players_) {
									if (player_ptr != players_[player_index_turn_]) {
										player_ptr->substractMoney(chance_card.getValue());
										players_[player_index_turn_]->addMoney(chance_card.getValue());
									}
								}
								setTurnState(TURN_END);
							} else {
								players_to_pay_rent.clear();
								money_to_find = toPay;
								bank_pay_rent = false;
								for (auto player_ptr : players_) {
									if (player_ptr != players_[player_index_turn_]) {
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
					notificationAdd(player_index_turn_, "Brak akcji");
					setTurnState(TURN_END);
				}
			} break;

			case BUY_ACTION: {
				bool boughtProp = false;
				int pos = players_[player_index_turn_]->getPosition();
				unsigned int price = getFieldPriceByPosition(pos);
				resign_buy_field_button_->setIsVisible(true);
				buy_field_button_->setIsVisible(true);
				BuyDecision buy_decision = players_[player_index_turn_]->decideBuy(pos);
				if (isButtonClicked(buy_field_button_) ||
					(players_[player_index_turn_]->getIsAi() &&
						buy_decision == BUY)) {	 //  && players_[player_index_turn_]->getMoney() >= price
					if (players_[player_index_turn_]->getMoney() >= price) {	// possible to buy property

						std::string textPlayerBoughtProperty(
							"kupil nieruchomosc " +
							std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
						notificationAdd(player_index_turn_, textPlayerBoughtProperty);

						players_[player_index_turn_]->substractMoney(price);
						players_[player_index_turn_]->addFieldOwnedId(pos);
						std::shared_ptr<Player> player_ptr = players_[player_index_turn_];
						addOwnerToPropertyField(player_ptr, pos);

						resign_buy_field_button_->setIsVisible(false);
						buy_field_button_->setIsVisible(false);
						setTurnState(TURN_END);

						boughtProp = true;
					} else	// NOT possible to buy property
					{
						std::string textPlayerBoughtProperty(
							"Nie moze kupic nieruchomosci: " +
							std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
						notificationAdd(player_index_turn_, textPlayerBoughtProperty);
					}
				}
				if (boughtProp == false) {
					if (isButtonClicked(resign_buy_field_button_) || getAuctionState() != NO_AUCTION ||
						(players_[player_index_turn_]->getIsAi() &&
							(buy_decision == RESIGN || players_[player_index_turn_]->getMoney() < price))) {
						if (getAuctionState() == NO_AUCTION) {
							std::string textPlayerResginedProperty(
								"rezygnuje z kupna nieruchomosci " +
								std::visit(
									[](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos)));
							notificationAdd(player_index_turn_, textPlayerResginedProperty);
							resign_buy_field_button_->setIsVisible(false);
							buy_field_button_->setIsVisible(false);
							boardToAuctionSwitchHandler(true);
							setScreenType(AUCTION);
							setAuctionState(INITIALIZATION);
						}
						performAuction();
						if (getAuctionState() == NO_AUCTION) {
							boardToAuctionSwitchHandler(false);
							resign_buy_field_button_->setIsVisible(false);
							buy_field_button_->setIsVisible(false);
							roll_dice_button_->setIsVisible(false);
							setScreenType(BOARDGAME);
							setTurnState(TURN_END);
						}
					}
				}
			} break;

			case PAY_RENT: {
				if (playerChanged) {
					std::string textPlayerrent("Musi zdobyc kase aby zaplacic czynsz " + std::to_string(money_to_find) +
											   " lub oglsic bankructwo");
					notificationAdd(player_index_turn_, textPlayerrent);
					playerChanged = false;
				}
				if (players_[player_index_turn_]->getMoney() < money_to_find) {  // player trying to get money to pay rent
					if (!players_[player_index_turn_]->getIsAi()) {
						buildingsManagingWorker();
					} else {
						aiBuildingsMangingWorker();
						if (players_[player_index_turn_]->getMoney() < money_to_find) {
							ai_bankrupted = true;
						}
					}
				} else {  // player has got money to pay rent
					players_[player_index_turn_]->substractMoney(money_to_find);
					unsigned int payment_counter = players_to_pay_rent.size();
					if (bank_pay_rent) {
						++payment_counter;
					}
					for (auto player_ptr : players_to_pay_rent) {
						player_ptr->addMoney(money_to_find / payment_counter);
					}
					std::string textPlayerrent("Zaplacil czynsz: " + std::to_string((money_to_find)));
					notificationAdd(player_index_turn_, textPlayerrent);
					setTurnState(TURN_END);
				}
			} break;

			case TURN_END:
				buildingsManagingWorker();
				next_turn_button_->setIsVisible(true);
				if (isButtonClicked(next_turn_button_) ||
					players_[player_index_turn_]->getIsAi()) {  // || players_[player_index_turn_]->getIsAi()
					setTurnState(ROLL_DICE);
					roll_dice_button_->setIsVisible(true);
					rolled_value_text_->setString("");
					resign_buy_field_button_->setIsVisible(false);
					buy_field_button_->setIsVisible(false);
					if (!playerBankrutedNow || !isDouble || players_[player_index_turn_]->getJailStatus() != 0) {
						double_turns = 0;

						if (!playerBankrutedNow) {	// check if current player bankruted, if not inc turn
							gameTurnsCounterHandle();
							incplayer_index_turn();
						} else {
							makePlayerBankrupt(player_index_turn_);
							playerBankrutedNow = false;
							if (player_index_turn_ >=
								players_.size()) {	// check if current player bankruted, if yes verify turn index
								player_index_turn_ = 0;
								game_turn_by_player_done_ = {false, false, false, false};
								++game_turns_globally_done_;
							}
						}

						if (gameFinishedCheckWinner()) {
							removePlayerFromGame(player_index_turn_, false);
							if (is_ai_game_only_) {
								fileLoggerClose();
								return false;  // leave game engine to return result to AI training
							} else {
								updateResultScreenStuff();
								setScreenType(RESULT);
								setTurnState(NO_TURN);
								fileLoggerClose();
							}
						} else if (gameFinishedCheckDraw()) {
							removePlayerFromGame(player_index_turn_, true);
							if (is_ai_game_only_) {
								fileLoggerClose();
								return false;  // leave game engine to return result to AI training
							} else {
								updateResultScreenStuff();
								setScreenType(RESULT);
								setTurnState(NO_TURN);
								fileLoggerClose();
							}
						}
						turnInfoTextShow();
					}

					if (playerBankrutedNow) {
						makePlayerBankrupt(player_index_turn_);
						playerBankrutedNow = false;
						if (player_index_turn_ >=
							players_.size()) {	// check if current player bankruted, if yes verify turn index
							player_index_turn_ = 0;
							game_turn_by_player_done_ = {false, false, false, false};
							++game_turns_globally_done_;
						}
					}
					next_turn_button_->setIsVisible(false);
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

						if (getScreenType() == WITHDRAW_CHOOSE_PLAYER &&
							getWithdraw().getPlayer2ToWithdraw() != nullptr) {
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
	} catch (std::exception& e) {
		std::string text = ("Blad! Gracz: " + std::to_string(players_[player_index_turn_]->getId()) +
							"Pozycja: " + std::to_string(players_[player_index_turn_]->getPosition()) +
							" Kasa: " + std::to_string(players_[player_index_turn_]->getMoney()));
		std::cout << e.what() << std::endl;
		std::cout << text << std::endl;
		notificationAdd(player_index_turn_, text);
	}
	return true;
}

void MonopolyGameEngine::updateAvailableHousesHotelText() {
	available_houses_text_->setString("Baza domow: " + std::to_string(getHouseCount()));
	availableH_hotels_text_->setString("Baza hoteli: " + std::to_string(getHotelCount()));
}

void MonopolyGameEngine::createAvailableHousesHotelText() {
	std::shared_ptr<sf::Text> avaHousesText(
		new sf::Text("Baza domow: " + std::to_string(getHouseCount()), getFont(), getFontSize() - 2));
	avaHousesText->setPosition(AVAILABLE_HOUSE_TEXT_POSITION);
	avaHousesText->setFillColor(sf::Color::Black);
	available_houses_text_ = avaHousesText;
	addText(avaHousesText);

	std::shared_ptr<sf::Text> avaHotelsText(
		new sf::Text("Baza hoteli: " + std::to_string(getHotelCount()), getFont(), getFontSize() - 2));
	avaHotelsText->setPosition(sf::Vector2f(AVAILABLE_HOUSE_TEXT_POSITION.x, AVAILABLE_HOUSE_TEXT_POSITION.y + 30));
	avaHotelsText->setFillColor(sf::Color::Black);
	availableH_hotels_text_ = avaHotelsText;
	addText(avaHotelsText);
}

sf::Vector2f MonopolyGameEngine::getUpdatePlayerSpritePosition() {
	float x_offset = 0;
	float y_offset = 0;
	const float HEIGHT_OFFSET = 20.0;  // If we want piece to go lower we increase this value.
	unsigned int player_position = players_[player_index_turn_]->getPosition();
	PossibleFields& curr_field = getBoard()->getFieldById(player_position);
	unsigned int curr_field_width = std::visit([](Field& field) { return field.getWidth(); }, curr_field);
	unsigned int curr_field_height = std::visit([](Field& field) { return field.getHeight(); }, curr_field);
	if (player_position <= 10) {
		x_offset = (float)curr_field_width * players_[player_index_turn_]->getSpriteOffsetX();
		y_offset = (float)curr_field_height * players_[player_index_turn_]->getSpriteOffsetY() + HEIGHT_OFFSET;
	} else if (player_position > 10 && player_position < 20) {
		x_offset = -(float)curr_field_height * players_[player_index_turn_]->getSpriteOffsetX() - HEIGHT_OFFSET;
		y_offset = (float)curr_field_width * players_[player_index_turn_]->getSpriteOffsetY();
	} else if (player_position >= 20 && player_position <= 30) {
		x_offset = -(float)curr_field_width * players_[player_index_turn_]->getSpriteOffsetX();
		y_offset = -(float)curr_field_height * players_[player_index_turn_]->getSpriteOffsetY() - HEIGHT_OFFSET;
	} else if (player_position > 30 && player_position <= 40) {
		x_offset = (float)curr_field_height * players_[player_index_turn_]->getSpriteOffsetX() + HEIGHT_OFFSET;
		y_offset = -(float)curr_field_width * players_[player_index_turn_]->getSpriteOffsetY();
	}
	float pos_x = (float)std::visit([](Field& field) { return field.getPosition().x; }, curr_field) + x_offset;
	float pos_y = (float)std::visit([](Field& field) { return field.getPosition().y; }, curr_field) + y_offset;
	return sf::Vector2f(pos_x, pos_y);
}

void MonopolyGameEngine::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& MonopolyGameEngine::getFont() {
	return font_;
}

void MonopolyGameEngine::addButton(std::shared_ptr<Button> button_tmp) {
	std::shared_ptr<Button> button = button_tmp;
	buttons_.push_back(button);
}

void MonopolyGameEngine::addText(std::shared_ptr<sf::Text> text_tmp) {
	std::shared_ptr<sf::Text> text = text_tmp;
	texts_.push_back(text);
}

void MonopolyGameEngine::addAuctionButton(std::shared_ptr<Button> button_tmp) {
	std::shared_ptr<Button> button = button_tmp;
	auction_buttons_.push_back(button);
}

void MonopolyGameEngine::addAuctionText(std::shared_ptr<sf::Text> text_tmp) {
	std::shared_ptr<sf::Text> text = text_tmp;
	auction_texts_.push_back(text);
}

std::vector<std::shared_ptr<Button>>& MonopolyGameEngine::getButtons() {
	return buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& MonopolyGameEngine::getTexts() {
	return texts_;
}

std::vector<std::shared_ptr<Button>>& MonopolyGameEngine::getAuctionButtons() {
	return auction_buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& MonopolyGameEngine::getAuctionTexts() {
	return auction_texts_;
}

std::vector<std::shared_ptr<sf::Text>>& MonopolyGameEngine::getResultTexts() {
	return result_players_places_;
}

sf::Sprite& MonopolyGameEngine::getPropertyDataSprite() {
	return property_data_sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& MonopolyGameEngine::getPropertyDataTexts() {
	return property_data_texts_;
}

sf::Sprite& MonopolyGameEngine::getAllPropertyDataSprite() {
	return all_property_data_sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& MonopolyGameEngine::getAllPropertyDataTexts() {
	return all_property_data_texts_;
}

void MonopolyGameEngine::createButtonRollDice() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonRollDice =
		(std::make_shared<Button>(IDLE, L"Rzut kośćmi", buttonSize, getFontSize()));
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
	roll_dice_button_ = buttonRollDice;
	addButton(buttonRollDice);
}

void MonopolyGameEngine::createTextTurnInfo() {
	std::shared_ptr<sf::Text> turnInfoText(new sf::Text("Tura: ", getFont(), getFontSize()));
	turnInfoText->setPosition(TURN_INFO_TEXT_POSITION);
	turnInfoText->setFillColor(sf::Color::Black);
	turn_info_text_ = turnInfoText;
	addText(turnInfoText);
}

void MonopolyGameEngine::createTextrolled_value() {
	std::shared_ptr<sf::Text> rolled_valueText(new sf::Text("", getFont(), getFontSize()));
	rolled_valueText->setPosition(ROLLED_VALUE_TEXT_POSITION);
	rolled_valueText->setFillColor(sf::Color::Black);
	rolled_value_text_ = rolled_valueText;
	addText(rolled_valueText);
}

void MonopolyGameEngine::createTextPlayersInfo() {
	sf::Vector2f defPos = PLAYERS_INFO_TEXT_POSITION;
	int i = 0;
	for (auto player : players_) {
		int id = player->getId();
		if (i > 0) {
			defPos.x += 180;
		}
		std::shared_ptr<sf::Text> playerText(new sf::Text("Gracz " + std::to_string(id + 1), getFont(), getFontSize()));
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

		std::vector<std::shared_ptr<sf::Text>> playerInfotext_tmp;
		playerInfotext_tmp.push_back(playerText);
		playerInfotext_tmp.push_back(playerMoneyText);
		playerInfotext_tmp.push_back(playerPositionText);
		playerInfotext_tmp.push_back(playerPositionNameText);
		player_info_text_[id].push_back(playerText);
		player_info_text_[id].push_back(playerMoneyText);
		player_info_text_[id].push_back(playerPositionText);
		player_info_text_[id].push_back(playerPositionNameText);
		++i;
	}
}

void MonopolyGameEngine::updateTextPlayersInfo() {
	bool isPlayerinGame[4] = {false};
	for (auto player : players_) {
		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(player->getPosition()));
		int id = player->getId();
		player_info_text_[id][1]->setString("Kasa: " + std::to_string(player->getMoney()));
		player_info_text_[id][2]->setString("Pozycja: " + std::to_string(player->getPosition() + 1));
		player_info_text_[id][3]->setString(streetName);
		isPlayerinGame[id] = true;
	}

	for (unsigned int i = 0; i < PLAYERS_MAX_; ++i) {
		unsigned int id = players_starting_Ids_[i];
		if (id != 255 && !isPlayerinGame[id]) {
			if (player_info_text_[id].size() > 0)	 // check if player id was even in game from begining
			{
				player_info_text_[id][1]->setString("Bankrut");
				player_info_text_[id][2]->setString("");
				player_info_text_[id][3]->setString("");
			}
		}
	}
}

void MonopolyGameEngine::createTextBiddedProperty() {
	std::shared_ptr<sf::Text> biddedPropertyText(new sf::Text("Cel: ", getFont(), getFontSize()));
	biddedPropertyText->setPosition(BIDDED_PROPERTY_TEXT_POSITION);
	biddedPropertyText->setFillColor(sf::Color::Black);
	bidded_property_text_ = biddedPropertyText;
	addAuctionText(biddedPropertyText);
}

void MonopolyGameEngine::createTextBidderInfo() {
	std::shared_ptr<sf::Text> bidderInfoText(new sf::Text("Tura gracza: ", getFont(), getFontSize()));
	bidderInfoText->setPosition(BIDDER_INFO_TEXT_POSITION);
	bidderInfoText->setFillColor(sf::Color::Black);
	bidder_info_text_ = bidderInfoText;
	addAuctionText(bidderInfoText);
}

void MonopolyGameEngine::createTextHighestBidInfo() {
	std::shared_ptr<sf::Text> highestBidText(new sf::Text(L"Najwyższa oferta: ", getFont(), getFontSize()));
	highestBidText->setPosition(HIGHEST_BID_TEXT_POSITION);
	highestBidText->setFillColor(sf::Color::Black);
	highest_bid_info_text_ = highestBidText;
	addAuctionText(highestBidText);
}

void MonopolyGameEngine::createTextLeadingBidderInfo() {
	std::shared_ptr<sf::Text> leadingBidderText(new sf::Text(L"Prowadząca oferta: ", getFont(), getFontSize()));
	leadingBidderText->setPosition(LEADING_BIDDER_TEXT_POSITION);
	leadingBidderText->setFillColor(sf::Color::Black);
	leading_bidder_info_text_ = leadingBidderText;
	addAuctionText(leadingBidderText);
}

void MonopolyGameEngine::createCurrentOfferBidderInfo() {
	std::shared_ptr<sf::Text> currentOfferText(new sf::Text("Aktualna oferta: ", getFont(), getFontSize()));
	currentOfferText->setPosition(CURRENT_OFFER_TEXT_POSITION);
	currentOfferText->setFillColor(sf::Color::Black);
	current_offer_info_text_ = currentOfferText;
	addAuctionText(currentOfferText);
}

void MonopolyGameEngine::createButtonBuyResign() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBuy = (std::make_shared<Button>(IDLE, "Kup", buttonSize, getFontSize()));
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
	buy_field_button_ = buttonBuy;
	addButton(buttonBuy);

	std::shared_ptr<Button> buttonResign = (std::make_shared<Button>(IDLE, "Rezygnuj", buttonSize, getFontSize()));
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
	resign_buy_field_button_ = buttonResign;
	addButton(buttonResign);
}

void MonopolyGameEngine::createButtonNextProperty() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonNext = (std::make_shared<Button>(IDLE, L"Następny", buttonSize, getFontSize()));
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
	next_property_button_ = buttonNext;
	addButton(buttonNext);
}

void MonopolyGameEngine::createButtonPerviousProperty() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonPrev = (std::make_shared<Button>(IDLE, "Poprzedni", buttonSize, getFontSize()));
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
	previous_property_button_ = buttonPrev;
	addButton(buttonPrev);
}

void MonopolyGameEngine::createButtonsBuySellHouseHotel() {
	house_text_ = std::make_shared<sf::Text>("Dom", getFont(), getFontSize() - 2);
	house_text_->setPosition(HOUSE_TEXT_POSITION);
	house_text_->setFillColor(sf::Color::Black);
	house_text_->setOrigin(house_text_->getGlobalBounds().getSize() / 2.f + house_text_->getLocalBounds().getPosition());
	hotel_text_ = std::make_shared<sf::Text>("Hotel", getFont(), getFontSize() - 2);
	hotel_text_->setPosition(HOTEL_TEXT_POSITION);
	hotel_text_->setFillColor(sf::Color::Black);
	hotel_text_->setOrigin(hotel_text_->getGlobalBounds().getSize() / 2.f + hotel_text_->getLocalBounds().getPosition());
	addText(house_text_);
	addText(hotel_text_);
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBuyHouse = (std::make_shared<Button>(IDLE, "Kup", buttonSize, getFontSize()));
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
	buy_house_button_ = buttonBuyHouse;
	addButton(buttonBuyHouse);

	std::shared_ptr<Button> buttonSellHouse = (std::make_shared<Button>(IDLE, "Sprzedaj", buttonSize, getFontSize()));
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
	sell_house_button_ = buttonSellHouse;
	addButton(buttonSellHouse);

	std::shared_ptr<Button> buttonBuyHotel = (std::make_shared<Button>(IDLE, "Kup", buttonSize, getFontSize()));
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
	buy_hotel_button_ = buttonBuyHotel;
	addButton(buttonBuyHotel);

	std::shared_ptr<Button> buttonSellHotel = (std::make_shared<Button>(IDLE, "Sprzedaj", buttonSize, getFontSize()));
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
	sell_hotel_button_ = buttonSellHotel;
	addButton(buttonSellHotel);
}

void MonopolyGameEngine::createButtonsBankrupt() {
	sf::Vector2f buttonSize = sf::Vector2f(160, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonBankrupt = (std::make_shared<Button>(IDLE, "Bankrutuj", buttonSize, getFontSize()));
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
	bankrupt_button_ = buttonBankrupt;
	addButton(buttonBankrupt);
}

void MonopolyGameEngine::createButtonsNextTurn() {
	sf::Vector2f buttonSize = sf::Vector2f(160, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonNextTurn =
		(std::make_shared<Button>(IDLE, "Kolejna tura", buttonSize, getFontSize()));
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
	next_turn_button_ = buttonNextTurn;
	addButton(buttonNextTurn);
}

void MonopolyGameEngine::createButtonsJailPay() {
	sf::Vector2f buttonSize = sf::Vector2f(260, 50);
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
	jail_pay_button_ = buttonJailPay;
	addButton(buttonJailPay);
}

void MonopolyGameEngine::createAuctionOfferButtons() {
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

	std::shared_ptr<Button> buttonAdd100 = (std::make_shared<Button>(IDLE, "+", buttonSize, getFontSize() + 10));
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
	add_100_to_offer_button_ = buttonAdd100;
	addButton(buttonAdd100);
	addAuctionButton(buttonAdd100);

	std::shared_ptr<Button> buttonAdd10 = (std::make_shared<Button>(IDLE, "+", buttonSize, getFontSize() + 10));
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
	add_10_to_offer_button_ = buttonAdd10;
	addButton(buttonAdd10);
	addAuctionButton(buttonAdd10);

	std::shared_ptr<Button> buttonAdd1 = (std::make_shared<Button>(IDLE, "+", buttonSize, getFontSize() + 10));
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
	add_1_to_offer_button_ = buttonAdd1;
	addButton(buttonAdd1);
	addAuctionButton(buttonAdd1);

	std::shared_ptr<Button> buttonSubstract100 = (std::make_shared<Button>(IDLE, "-", buttonSize, getFontSize() + 10));
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
	substract_100_from_offer_button_ = buttonSubstract100;
	addButton(buttonSubstract100);
	addAuctionButton(buttonSubstract100);

	std::shared_ptr<Button> buttonSubstract10 = (std::make_shared<Button>(IDLE, "-", buttonSize, getFontSize() + 10));
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
	substract_10_from_offer_button_ = buttonSubstract10;
	addButton(buttonSubstract10);
	addAuctionButton(buttonSubstract10);

	std::shared_ptr<Button> buttonSubstract1 = (std::make_shared<Button>(IDLE, "-", buttonSize, getFontSize() + 10));
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
	substract_1_from_offer_button_ = buttonSubstract1;
	addButton(buttonSubstract1);
	addAuctionButton(buttonSubstract1);
}

void MonopolyGameEngine::createAuctionBidButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonBidAuction = (std::make_shared<Button>(IDLE, "Przebij", buttonSize, getFontSize()));
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
	auction_bid_button_ = buttonBidAuction;
	addButton(buttonBidAuction);
	addAuctionButton(buttonBidAuction);
}

void MonopolyGameEngine::createAuctionResignButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonResignAuction =
		(std::make_shared<Button>(IDLE, "Rezygnuj", buttonSize, getFontSize()));
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
	auction_resign_button_ = buttonResignAuction;
	addButton(buttonResignAuction);
	addAuctionButton(buttonResignAuction);
}

void MonopolyGameEngine::createButtonWithdraw() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonWithdraw = (std::make_shared<Button>(IDLE, "Wymiana", buttonSize, getFontSize()));
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
	withdraw_button_ = buttonWithdraw;
	addButton(buttonWithdraw);
}

void MonopolyGameEngine::createResultScreenStuff() {
	float x_coord = 960;
	std::shared_ptr<sf::Text> resultGameText = std::make_shared<sf::Text>("Wyniki rozgrywki", getFont(), getFontSize());
	resultGameText->setFillColor(sf::Color::Black);
	resultGameText->setOrigin(
		resultGameText->getGlobalBounds().getSize() / 2.f + resultGameText->getLocalBounds().getPosition());
	resultGameText->setPosition(sf::Vector2f(x_coord, RESULT_DATA_Y));
	result_players_places_.push_back(resultGameText);

	std::shared_ptr<Button> buttonReturn = createDefaultButton(L"Wróc", 120, 50);
	buttonReturn->setEventType(RETURN_TO_MAIN_MENU);
	return_to_main_menu_button_ = buttonReturn;
	addButton(buttonReturn);
}

void MonopolyGameEngine::updateResultScreenStuff() {
	int y_step = 40;
	float x_coord = 960;
	int y_offset = 0;
	for (std::vector<std::shared_ptr<Player>>::reverse_iterator iter = players_bankrupted_.rbegin();
		 iter != players_bankrupted_.rend(); ++iter) {
		y_offset += y_step;
		std::string text =
			std::to_string(iter->get()->getResultPlace()) + ": Gracz " + std::to_string(iter->get()->getId() + 1);
		std::shared_ptr<sf::Text> playerResultText = std::make_shared<sf::Text>(text, getFont(), getFontSize());
		playerResultText->setFillColor(sf::Color::Black);
		playerResultText->setOrigin(
			playerResultText->getGlobalBounds().getSize() / 2.f + playerResultText->getLocalBounds().getPosition());
		playerResultText->setPosition(sf::Vector2f(x_coord, RESULT_DATA_Y + y_offset));
		result_players_places_.push_back(playerResultText);
	}

	y_offset += y_step;
	return_to_main_menu_button_->setPosition(sf::Vector2f(x_coord, RESULT_DATA_Y + y_offset));
	for (auto button_ptr : getButtons()) {
		button_ptr->setIsVisible(false);
	}
	return_to_main_menu_button_->setIsVisible(true);
}

void MonopolyGameEngine::createMortagingButton() {
	sf::Vector2f buttonSize = sf::Vector2f(120, 50);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;

	std::shared_ptr<Button> buttonMortgage = (std::make_shared<Button>(IDLE, "Zastaw", buttonSize, getFontSize()));
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
	mortgage_button_ = buttonMortgage;
	addButton(buttonMortgage);

	std::shared_ptr<Button> buttonUnMortgage = (std::make_shared<Button>(IDLE, "Wykup", buttonSize, getFontSize()));
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
	unmortgage_button_ = buttonUnMortgage;
	addButton(buttonUnMortgage);
}

void MonopolyGameEngine::clearPropertyData(bool is_property_shown_to_buy) {
	if (is_property_shown_to_buy) {
		property_data_texts_.clear();
	} else {
		all_property_data_texts_.clear();
	}
}

void MonopolyGameEngine::showPropertyData(unsigned int pos, bool is_property_shown_to_buy) {
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
	if (is_property_shown_to_buy) {
		dataPos = PROPERTY_DATA_POSITION;
		// if (!property_data_texture_.loadFromFile(graphic_path)) {
		// 	property_data_sprite_.setColor(sf::Color::Green);
		// }
		property_data_sprite_.setTexture(property_data_texture_, true);
		sf::Vector2u texture_dim = property_data_texture_.getSize();
		float scale_x = (float)width / (float)texture_dim.x;
		float scale_y = (float)height / (float)texture_dim.y;
		const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
		property_data_sprite_.setScale(SCALE_VECT * PROPERTY_DATA_SCALE);
		property_data_sprite_.setPosition(dataPos.x, dataPos.y);
		property_data_sprite_.setRotation(0);

	} else {
		dataPos = ALL_PROPERTY_DATA_POSITION;
		// if (!all_property_data_texture_.loadFromFile(graphic_path)) {
		// 	all_property_data_sprite_.setColor(sf::Color::Green);
		// }
		all_property_data_sprite_.setTexture(all_property_data_texture_, true);
		sf::Vector2u texture_dim = all_property_data_texture_.getSize();
		float scale_x = (float)width / (float)texture_dim.x;
		float scale_y = (float)height / (float)texture_dim.y;
		const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
		all_property_data_sprite_.setScale(SCALE_VECT * PROPERTY_DATA_SCALE);
		all_property_data_sprite_.setPosition(dataPos.x, dataPos.y);
		all_property_data_sprite_.setRotation(0);
	}

	const std::string streetName =
		std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos));
	std::shared_ptr<sf::Text> propertyName(new sf::Text(streetName, getFont(), getFontSize() - 2));
	propertyName->setOrigin(
		propertyName->getGlobalBounds().getSize() / 2.f + propertyName->getLocalBounds().getPosition());
	sf::Sprite dataSprite;
	if (is_property_shown_to_buy) {
		dataSprite = property_data_sprite_;
	} else {
		dataSprite = all_property_data_sprite_;
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

	if (is_property_shown_to_buy) {
		property_data_texts_.push_back(propertyName);
		property_data_texts_.push_back(propertyMortgagePrice);
		property_data_texts_.push_back(propertyPrice);
		property_data_texts_.push_back(propertyMortgage);
	} else {
		all_property_data_texts_.push_back(propertyName);
		all_property_data_texts_.push_back(propertyMortgagePrice);
		all_property_data_texts_.push_back(propertyPrice);
		all_property_data_texts_.push_back(propertyMortgage);
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

		if (is_property_shown_to_buy) {
			property_data_texts_.push_back(propertyRent1);
			property_data_texts_.push_back(propertyRent2);
			property_data_texts_.push_back(propertyRent3);
			property_data_texts_.push_back(propertyRent4);
			property_data_texts_.push_back(propertyRent5);
			property_data_texts_.push_back(propertyRent6);
			property_data_texts_.push_back(propertyRent7);
			property_data_texts_.push_back(propertyHouseCost);
			property_data_texts_.push_back(propertyRentCost1);
			property_data_texts_.push_back(propertyRentCost2);
			property_data_texts_.push_back(propertyRentCost3);
			property_data_texts_.push_back(propertyRentCost4);
			property_data_texts_.push_back(propertyRentCost5);
			property_data_texts_.push_back(propertyRentCost6);
			property_data_texts_.push_back(propertyRentCost7);
			property_data_texts_.push_back(propertyHousePrice);
			property_data_texts_.push_back(propertyHotelPrice);
		} else {
			all_property_data_texts_.push_back(propertyRent1);
			all_property_data_texts_.push_back(propertyRent2);
			all_property_data_texts_.push_back(propertyRent3);
			all_property_data_texts_.push_back(propertyRent4);
			all_property_data_texts_.push_back(propertyRent5);
			all_property_data_texts_.push_back(propertyRent6);
			all_property_data_texts_.push_back(propertyRent7);
			all_property_data_texts_.push_back(propertyHouseCost);
			all_property_data_texts_.push_back(propertyHotelCost);
			all_property_data_texts_.push_back(propertyRentCost1);
			all_property_data_texts_.push_back(propertyRentCost2);
			all_property_data_texts_.push_back(propertyRentCost3);
			all_property_data_texts_.push_back(propertyRentCost4);
			all_property_data_texts_.push_back(propertyRentCost5);
			all_property_data_texts_.push_back(propertyRentCost6);
			all_property_data_texts_.push_back(propertyRentCost7);
			all_property_data_texts_.push_back(propertyHousePrice);
			all_property_data_texts_.push_back(propertyHotelPrice);
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

		if (is_property_shown_to_buy) {
			property_data_texts_.push_back(propertyRent1);
			property_data_texts_.push_back(propertyRent2);
			property_data_texts_.push_back(propertyRent3);
			property_data_texts_.push_back(propertyRent4);
			property_data_texts_.push_back(propertyRentCost1);
			property_data_texts_.push_back(propertyRentCost2);
			property_data_texts_.push_back(propertyRentCost3);
			property_data_texts_.push_back(propertyRentCost4);
		} else {
			all_property_data_texts_.push_back(propertyRent1);
			all_property_data_texts_.push_back(propertyRent2);
			all_property_data_texts_.push_back(propertyRent3);
			all_property_data_texts_.push_back(propertyRent4);
			all_property_data_texts_.push_back(propertyRentCost1);
			all_property_data_texts_.push_back(propertyRentCost2);
			all_property_data_texts_.push_back(propertyRentCost3);
			all_property_data_texts_.push_back(propertyRentCost4);
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

		if (is_property_shown_to_buy) {
			property_data_texts_.push_back(propertyRent);
			property_data_texts_.push_back(propertyRent1);
			property_data_texts_.push_back(propertyRent2);
			property_data_texts_.push_back(propertyRentCost1);
			property_data_texts_.push_back(propertyRentCost2);
		} else {
			all_property_data_texts_.push_back(propertyRent);
			all_property_data_texts_.push_back(propertyRent1);
			all_property_data_texts_.push_back(propertyRent2);
			all_property_data_texts_.push_back(propertyRentCost1);
			all_property_data_texts_.push_back(propertyRentCost2);
		}
	}
}

unsigned int MonopolyGameEngine::getFieldPriceByPosition(unsigned int pos) {
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

void MonopolyGameEngine::addOwnerToPropertyField(std::shared_ptr<Player> player, unsigned int pos) {
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

NotificationWall& MonopolyGameEngine::getNotificationsWall() {
	return notifications_wall_;
}

void MonopolyGameEngine::notificationAdd(unsigned int index, std::string text) {
	const unsigned int LINE_LEN = 59;
	const unsigned int LINE_LEN_WITHOUT_PLAYER = LINE_LEN - std::string("Gracz X: ").length();
	unsigned int id = players_[index]->getId();
	if (text.size() <= LINE_LEN_WITHOUT_PLAYER) {
		notifications_wall_.addToWall("Gracz " + std::to_string(id + 1) + ": " + text);
	} else {
		notifications_wall_.addToWall(
			"Gracz " + std::to_string(id + 1) + ": " + text.substr(0, LINE_LEN_WITHOUT_PLAYER) + "-");
		for (unsigned int i = LINE_LEN_WITHOUT_PLAYER; i < text.size(); i += LINE_LEN_WITHOUT_PLAYER + 2) {
			notifications_wall_.addToWall("            " + text.substr(i, LINE_LEN_WITHOUT_PLAYER + 2));
		}
	}
	fileLoggerWrite("Gracz " + std::to_string(id + 1) + ": " + text);
}

sf::Text MonopolyGameEngine::getPropertyNameToDraw(sf::Text text, sf::Sprite& sprite, float rotation) {
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

void MonopolyGameEngine::removePlayerFromGame(unsigned int player_index_turn, bool is_draw) {
	if (!is_draw) {
		// add player to bankrupted players vector and set his result place
		players_[player_index_turn]->setResultPlace(players_.size());
		players_bankrupted_.push_back(players_[player_index_turn]);

		// remove certain player from vector
		players_.erase(std::remove(players_.begin(), players_.end(), players_[player_index_turn]), players_.end());
	} else {
		int size = players_.size();
		for (auto player_ptr : players_) {
			// add all players to bankrupted players vector and set the same place for them
			player_ptr->setResultPlace(size);
			players_bankrupted_.push_back(player_ptr);
		}

		players_.clear();
	}
}

void MonopolyGameEngine::makePlayerBankrupt(unsigned int player_index_turn) {
	// TODO
	// remove ownerships from fields
	for (unsigned int pos = 0; pos < gameboard_->getFieldNumber(); ++pos) {
		FieldType field_type = std::visit([](Field& field) { return field.getType(); }, gameboard_->getFieldById(pos));
		if (field_type == STREET) {
			StreetField& field = std::get<StreetField>(getBoard()->getFieldById(pos));
			if (field.getOwner() == players_[player_index_turn]) {
				field.setOwner(nullptr);
				field.createFlagSprite();
			}
		} else if (field_type == STATION) {
			StationField& field = std::get<StationField>(getBoard()->getFieldById(pos));
			if (field.getOwner() == players_[player_index_turn]) {
				field.setOwner(nullptr);
				field.createFlagSprite();
			}
		} else if (field_type == UTILITY) {
			UtilityField& field = std::get<UtilityField>(getBoard()->getFieldById(pos));
			if (field.getOwner() == players_[player_index_turn]) {
				field.setOwner(nullptr);
				field.createFlagSprite();
			}
		}
	}
	removePlayerFromGame(player_index_turn, false);
}

std::shared_ptr<Button> MonopolyGameEngine::createDefaultButton(sf::String text,
	unsigned int width,
	unsigned int height) {
	sf::Vector2f buttonSize = sf::Vector2f(width, height);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonDefault = (std::make_shared<Button>(IDLE, text, buttonSize, getFontSize()));
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

sf::Texture& MonopolyGameEngine::getHouseTexture() {
	return house_texture_;
}

sf::Texture& MonopolyGameEngine::getHotelTexture() {
	return hotel_texture_;
}

sf::Vector2f& MonopolyGameEngine::getHouseSize() {
	return house_size_;
}

sf::Sprite MonopolyGameEngine::getHouseSprite(StreetField& field, unsigned int houses_number) {
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

	position.x += xOffset * (houses_number - 1);
	position.y += yOffset * (houses_number - 1);

	house_sprite.setPosition(position.x, position.y);

	return house_sprite;
}

sf::Sprite MonopolyGameEngine::getHotelSprite(StreetField& field) {
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

Withdraw& MonopolyGameEngine::getWithdraw() {
	return withdraw_;
}

void MonopolyGameEngine::createChanceCards() {
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
		chance_cards_.push_back(card);
	}

	shuffleChanceCards();
}

void MonopolyGameEngine::shuffleChanceCards() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(chance_cards_), std::end(chance_cards_), g);
}

ChanceCard& MonopolyGameEngine::getChanceCard() {
	return chance_cards_[chance_card_current_];
}

void MonopolyGameEngine::updateChanceCard() {
	++chance_card_current_;
	if (chance_card_current_ == chance_cards_.size()) {
		chance_card_current_ = 0;
		shuffleChanceCards();
	}
}

bool MonopolyGameEngine::gameFinishedCheckWinner() {
	return (players_.size() <= 1);	// check if one (or less???) player only is in game
}

bool MonopolyGameEngine::gameFinishedCheckDraw() {
	return (game_turns_globally_done_ >= GAME_TURNS_MAX);	// check turns global treshold
}

void MonopolyGameEngine::gameTurnsCounterHandle() {
	game_turn_by_player_done_[players_[player_index_turn_]->getId()] = true;

	for (auto player_ptr : players_) {
		if (game_turn_by_player_done_[player_ptr->getId()] == false) {
			return;	 // not all players done move in this turn (active players)
		}
	}

	game_turn_by_player_done_ = {false, false, false, false};
	++game_turns_globally_done_;
}

void MonopolyGameEngine::fileLoggerOpen() {
	file_logger = std::ofstream(FILE_LOGGER_PATH, std::ofstream::out);
}

void MonopolyGameEngine::fileLoggerWrite(std::string text) {
	if (file_logger.is_open()) {
		file_logger << text << std::endl;
	}
}

void MonopolyGameEngine::fileLoggerClose() {
	file_logger.close();
}