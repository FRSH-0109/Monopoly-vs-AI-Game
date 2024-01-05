#include "monopolyGameEngine.h"

monopolyGameEngine::monopolyGameEngine() {
	turnState_ = RollDice;
}

void monopolyGameEngine::createBoard() {
	gameboard_ = std::make_shared<Board>(GAMEBOARD_FILE_PATH);
}

void monopolyGameEngine::createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list) {
	int i = 0;
	// TODO Parsować po wektorze playerSettings i generować na jego podstawie przeshufflowaną listę graczy)
	int playerId = 0;
	for (auto it : player_settings_list) {
		if (!(it->isNone)) {
			Player new_player = Player(PLAYER_MONEY_DEFAULT);
			new_player.setIsAi(!(it->isHuman));
			new_player.setAiLevel(it->level);
			new_player.setId(playerId);
			players_.push_back(std::make_shared<Player>(new_player));
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
		const float SPRITE_POSITION_Y = (float)BOARD_POSITION.y + (float)FIELD_HEIGHT * player->getSpriteOffsetY() + (float)HEIGHT_OFFSET;
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
	if (indx < PLAYERS_MAX) {
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

bool monopolyGameEngine::isRollDiceButtonClicked() {
	if (rollDiceButton_->getIsActive()) {
		rollDiceButton_->setIsActive(false);
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
	if(!field.getIsMortaged() && groupCompleted(builder_ownes, field) && builder->getMoney() > field.getHousePrice() && field_houses < 4) { // W tym if trzeba będzie dodać kontrolę budynków w puli
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if(field_houses > group_member.getHouseNumber() || group_member.getIsMortaged()) {
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
	if(!field.getIsMortaged() && groupCompleted(builder_ownes, field) && field_houses > 0) {
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if(field_houses < group_member.getHouseNumber() || group_member.getIsMortaged()) {
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
	if(!field.getIsMortaged() && groupCompleted(builder_ownes, field) && builder->getMoney() > field.getHotelPrice() && field_houses == 4) {
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if((group_member.getHouseNumber() < 4 && !group_member.getIsHotel()) || group_member.getIsMortaged()) {
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
	if(!field.getIsMortaged() && groupCompleted(builder_ownes, field) && field.getIsHotel()) {
		for (int i = 0; i < field.getGroupMembers().size(); ++i) {
			StreetField& group_member = std::get<StreetField>(getBoard()->getFieldById(field.getGroupMembers()[i]));
			if(group_member.getIsMortaged()) {
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

void monopolyGameEngine::handlePassingStart(unsigned int oldPos, unsigned int newPos) {
	if (newPos < oldPos) {	// start passed
		players_[playerIndexturn_]->addMoney(START_PASSING_MONEY_);
	}
}

void monopolyGameEngine::showAllPropertiesWorker() {
	static bool allowToClickNext = true;
	static bool allowToClickPrev = true;

	if (nextPropertyButton_->getIsClicked() == false) {
		allowToClickNext = true;
		nextPropertyButton_->setIsActive(false);
	}

	if (previousPropertyButton_->getIsClicked() == false) {
		allowToClickPrev = true;
		previousPropertyButton_->setIsActive(false);
	}

	if (nextPropertyButton_->getIsActive() && allowToClickNext) {
		allowToClickNext = false;
		nextPropertyButton_->setIsActive(false);
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

	if (previousPropertyButton_->getIsActive() && allowToClickPrev) {
		allowToClickPrev = false;
		previousPropertyButton_->setIsActive(false);
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

void monopolyGameEngine::monopolyGameWorker() {
	turnInfoTextWorker();
	updateTextPlayersInfo();
	showAllPropertiesWorker();
	static int rolled_val;

	static bool allowToClickRollDice = true;

	if (rollDiceButton_->getIsClicked() == false) {
		allowToClickRollDice = true;
		rollDiceButton_->setIsActive(false);
	}

	switch (getTurnState()) {
		case RollDice: {
			if (isRollDiceButtonClicked() && allowToClickRollDice) {
				allowToClickRollDice = false;
				unsigned int roll1 = rollDice();
				unsigned int roll2 = rollDice();
				rolled_val = roll1 + roll2;
				std::string rol = "Rolled value: ";
				std::string val = std::to_string(rolled_val);
				rolledValueText_->setString(rol + val);

				notificationAdd(playerIndexturn_, rol + val);

				int oldPos = players_[playerIndexturn_]->getPositon();
				movePlayer(playerIndexturn_, rolled_val);
				int newPos = players_[playerIndexturn_]->getPositon();
				handlePassingStart(oldPos, newPos);

				rollDiceButton_->setIsVisible(false);
				setTurnState(FieldAction);
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
						setTurnState(PayRent);
					}
				} else {
					std::cout << "No action - player owns this field" << field_type << std::endl;
					setTurnState(TurnEnd);
				}
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
				if (players_[playerIndexturn_]->getMoney() >= price) {  // possible to buy property

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
			// Tutaj ideowo gracz ma być zmuszony do zrobienia wymiany, sprzedania domków/hoteli i/lub zastawienia nieruchomości
			std::cout << "Gracz ma problemy finansowe" << std::endl;
			setTurnState(TurnEnd);
		} break;

		case TurnEnd:
			rollDiceButton_->setIsVisible(true);
			rolledValueText_->setString("");
			resignBuyFieldButton_->setIsVisible(false);
			buyFieldButton_->setIsVisible(false);
			incPlayerIndexTurn();
			setTurnState(RollDice);
			break;
		default:
			break;
	}
}

sf::Vector2f monopolyGameEngine::getUpdatePlayerSpritePosition() {
	float x_offset;
	float y_offset;
	const float HEIGHT_OFFSET = 20.0; // If we want piece to go lower we increase this value.
	unsigned int player_position = players_[playerIndexturn_]->getPositon();
	PossibleFields& curr_field = getBoard()->getFieldById(player_position);
	unsigned int curr_field_width = std::visit([](Field& field) { return field.getWidth(); }, curr_field);
	unsigned int curr_field_height = std::visit([](Field& field) { return field.getHeight(); }, curr_field);
	if (player_position <= 10) {
		x_offset = (float)curr_field_width * players_[playerIndexturn_]->getSpriteOffsetX();
		y_offset = (float)curr_field_height * players_[playerIndexturn_]->getSpriteOffsetY() + HEIGHT_OFFSET;
	} else if (player_position > 10 && player_position <= 20) {
		x_offset = -(float)curr_field_height * players_[playerIndexturn_]->getSpriteOffsetX() - HEIGHT_OFFSET;
		y_offset = (float)curr_field_width * players_[playerIndexturn_]->getSpriteOffsetY();
	} else if (player_position > 20 && player_position <= 30) {
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