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
			players_.push_back(new_player);
			++i;
		}
		++playerId;
	};
	playersStartingAmount_ = i;
	i = 0;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(players_), std::end(players_), g);
	for (Player& player : players_) {
		player.createSprite();
		if (i % 2 == 0) {
			player.setSpriteOffsetX(0.33f);
		} else {
			player.setSpriteOffsetX(0.66f);
		}
		if (i >= 0 && i < 2) {
			player.setSpriteOffsetY(0.33f);
		} else if (i >= 2 && i < 4) {
			player.setSpriteOffsetY(0.66f);
		}
		const sf::Vector2i BOARD_POSITION = gameboard_->getBoardPosition();
		const unsigned int FIELD_WIDTH =
			std::visit([](Field& field) { return field.getWidth(); }, gameboard_->getFieldById(0));
		const unsigned int FIELD_HEIGHT =
			std::visit([](Field& field) { return field.getHeight(); }, gameboard_->getFieldById(0));
		const float SPRITE_POSITION_X = (float)BOARD_POSITION.x + (float)FIELD_WIDTH * player.getSpriteOffsetX();
		const float SPRITE_POSITION_Y = (float)BOARD_POSITION.y + (float)FIELD_HEIGHT * player.getSpriteOffsetY();
		player.setSpritePosition(sf::Vector2f(SPRITE_POSITION_X, SPRITE_POSITION_Y));
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

std::vector<Player>& monopolyGameEngine::getPlayers() {
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

unsigned int monopolyGameEngine::rollDice() const {	 // dices roll for 1-12 move
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(2, 12);
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
	turnInfoText_->setString("Turn: Player " + std::to_string(getPlayerIndexTurn() + 1));
}

void monopolyGameEngine::monopolyGameWorker() {
	turnInfoTextWorker();
	updateTextPlayersInfo();

	switch (getTurnState()) {
		case RollDice: {
			rollDiceButton_->setIsVisible(true);
			if (rollDiceButton_->getIsActive()) {
				rollDiceButton_->setIsActive(false);
				int rolledVal = rollDice();
				std::string rol = "Rolled Value: ";
				std::string val = std::to_string(rolledVal);
				rolledValueText_->setString(rol + val);
				int oldPos = players_[playerIndexturn_].getPositon();
				int newPos = (oldPos + rolledVal) % 40;
				players_[playerIndexturn_].setPositon(newPos);
				sf::Vector2f newPlayerSpritePos = getUpdatePlayerSpritePosition();
				players_[playerIndexturn_].setSpritePosition(newPlayerSpritePos);
				rollDiceButton_->setIsVisible(false);
				setTurnState(FieldAction);
			}
		} break;
		case FieldAction: {
			int pos = players_[playerIndexturn_].getPositon();
			FieldType fieldType =
				std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
			if (fieldType == STREET || fieldType == STATION || fieldType == UTILITY) {
				Player* owner = nullptr;
				if (fieldType == STREET) {
					StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
					owner = field.getOwner();
				} else if (fieldType == STATION) {
					StationField field = std::get<StationField>(getBoard()->getFieldById(pos));
					owner = field.getOwner();
				} else {
					UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
					owner = field.getOwner();
				}

				if (owner == nullptr) {
					setTurnState(BuyAction);
					clearPropertyData();
					showPropertyData(pos);
				} else {
					std::cout << "No action, field taken" << fieldType << std::endl;
					setTurnState(TurnEnd);
				}
			} else {
				std::cout << "No action" << fieldType << std::endl;
				setTurnState(TurnEnd);
			}
		} break;
		case BuyAction: {
			int pos = players_[playerIndexturn_].getPositon();
			unsigned int price = getFieldPriceByPosition(pos);
			FieldType fieldType =
				std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
			resignBuyFieldButton_->setIsVisible(true);
			buyFieldButton_->setIsVisible(true);
			if (buyFieldButton_->getIsActive()) {
				if (players_[playerIndexturn_].getMoney() >= price) {  // possible to buy property
					players_[playerIndexturn_].setMoney(players_[playerIndexturn_].getMoney() - price);
					players_[playerIndexturn_].addFieldOwnedId(pos);
					addOwnerToPropertyField(&players_[playerIndexturn_], pos);

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
				resignBuyFieldButton_->setIsActive(false);
				resignBuyFieldButton_->setIsVisible(false);
				buyFieldButton_->setIsVisible(false);
				setTurnState(TurnEnd);
			}

		} break;
		case TurnEnd:
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
	unsigned int player_position = players_[playerIndexturn_].getPositon();
	PossibleFields& curr_field = getBoard()->getFieldById(player_position);
	unsigned int curr_field_width = std::visit([](Field& field) { return field.getWidth(); }, curr_field);
	unsigned int curr_field_height = std::visit([](Field& field) { return field.getHeight(); }, curr_field);
	if (player_position <= 10) {
		x_offset = (float)curr_field_width * players_[playerIndexturn_].getSpriteOffsetX();
		y_offset = (float)curr_field_height * players_[playerIndexturn_].getSpriteOffsetY();
	} else if (player_position > 10 && player_position <= 20) {
		x_offset = -(float)curr_field_height * players_[playerIndexturn_].getSpriteOffsetX();
		y_offset = (float)curr_field_width * players_[playerIndexturn_].getSpriteOffsetY();
	} else if (player_position > 20 && player_position <= 30) {
		x_offset = -(float)curr_field_width * players_[playerIndexturn_].getSpriteOffsetX();
		y_offset = -(float)curr_field_height * players_[playerIndexturn_].getSpriteOffsetY();
	} else if (player_position > 30 && player_position <= 40) {
		x_offset = (float)curr_field_height * players_[playerIndexturn_].getSpriteOffsetX();
		y_offset = -(float)curr_field_width * players_[playerIndexturn_].getSpriteOffsetY();
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
	buttonRollDice->setIsVisible(false);
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
	for (int i = 0; i < players_.size(); ++i) {
		int id = players_[i].getId();
		if (i > 0) {
			defPos.x += 180;
		}
		std::shared_ptr<sf::Text> playerText(
			new sf::Text("Player " + std::to_string(id + 1), getFont(), getFontSize()));
		playerText->setPosition(defPos);
		playerText->setColor(players_[id].getColor());
		playerText->setOutlineColor(sf::Color::Black);
		playerText->setOutlineThickness(2);
		addText(playerText);

		std::shared_ptr<sf::Text> playerMoneyText(
			new sf::Text("Money: " + std::to_string(players_[id].getMoney()), getFont(), getFontSize() - 7));
		playerMoneyText->setPosition(sf::Vector2f(defPos.x, defPos.y + 50));
		playerMoneyText->setColor(sf::Color::Black);
		addText(playerMoneyText);

		std::shared_ptr<sf::Text> playerPositionText(
			new sf::Text("Position: " + std::to_string(players_[id].getPositon() + 1), getFont(), getFontSize() - 7));
		playerPositionText->setPosition(sf::Vector2f(defPos.x, defPos.y + 80));
		playerPositionText->setColor(sf::Color::Black);
		addText(playerPositionText);

		const std::string streetName = std::visit(
			[](Field& field) { return field.getName(); }, getBoard()->getFieldById(players_[id].getPositon()));
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
	}
}

void monopolyGameEngine::updateTextPlayersInfo() {
	bool isPlayerinGame[4] = {false, false, false, false};
	for (auto player : players_) {
		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(player.getPositon()));
		int id = player.getId();
		playerInfoText_[id][1]->setString("Money: " + std::to_string(player.getMoney()));
		playerInfoText_[id][2]->setString("Position: " + std::to_string(player.getPositon() + 1));
		playerInfoText_[id][3]->setString(streetName);
		isPlayerinGame[id] = true;
	}

	for (int i = 0; i < playersStartingAmount_; ++i) {
		if (!isPlayerinGame[i]) {
			int id = players_[i].getId();
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

void monopolyGameEngine::clearPropertyData() {
	propertyDataTexts_.clear();
}

void monopolyGameEngine::showPropertyData(unsigned int pos) {
	FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	unsigned int price;
	unsigned int rents[5] = {0};
	if (fieldType == STREET) {
		StreetField field = std::get<StreetField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
	} else if (fieldType == STATION) {
		StationField field = std::get<StationField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
	} else	// fieldType == UTILITY
	{
		UtilityField field = std::get<UtilityField>(getBoard()->getFieldById(pos));
		price = field.getPrice();
	}

	unsigned int width = std::visit([](Field& field) { return field.getWidth(); }, getBoard()->getFieldById(pos));
	unsigned int height = std::visit([](Field& field) { return field.getHeight(); }, getBoard()->getFieldById(pos));
	std::string graphic_path =
		std::visit([](Field& field) { return field.getGraphicPath(); }, getBoard()->getFieldById(pos));
	if (!propertyDataTexture_.loadFromFile(graphic_path)) {
		propertyDataSprite_.setColor(sf::Color::Green);
	}
	propertyDataSprite_.setTexture(propertyDataTexture_, true);
	sf::Vector2u texture_dim = propertyDataTexture_.getSize();
	float scale_x = (float)width / (float)texture_dim.x;
	float scale_y = (float)height / (float)texture_dim.y;
	const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
	propertyDataSprite_.setScale(SCALE_VECT * PROPERTY_DATA_SCALE);
	propertyDataSprite_.setPosition(PROPERTY_DATA_POSITION.x, PROPERTY_DATA_POSITION.y);
	propertyDataSprite_.setRotation(0);

	const std::string streetName =
		std::visit([](Field& field) { return field.getName(); }, getBoard()->getFieldById(pos));
	std::shared_ptr<sf::Text> propertyName(new sf::Text(streetName, getFont(), getFontSize() - 2));
	propertyName->setOrigin(
		propertyName->getGlobalBounds().getSize() / 2.f + propertyName->getLocalBounds().getPosition());
	propertyName->setPosition(
		sf::Vector2f(PROPERTY_DATA_POSITION.x + (propertyDataSprite_.getGlobalBounds().getSize().x / 2.f),
			PROPERTY_DATA_POSITION.y + 80));
	propertyName->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyName);

	std::shared_ptr<sf::Text> propertyPrice(
		new sf::Text("Price: " + std::to_string(price), getFont(), getFontSize() - 2));
	propertyPrice->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 90));
	propertyPrice->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyPrice);

	std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Rent: ", getFont(), getFontSize() - 2));
	propertyRent1->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 115));
	propertyRent1->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent1);

	std::shared_ptr<sf::Text> propertyRent2(new sf::Text("  with color set: ", getFont(), getFontSize() - 2));
	propertyRent2->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 140));
	propertyRent2->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent2);

	std::shared_ptr<sf::Text> propertyRent3(new sf::Text("  with 1 house: ", getFont(), getFontSize() - 2));
	propertyRent3->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 165));
	propertyRent3->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent3);

	std::shared_ptr<sf::Text> propertyRent4(new sf::Text("  with 2 houses: ", getFont(), getFontSize() - 2));
	propertyRent4->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 190));
	propertyRent4->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent4);

	std::shared_ptr<sf::Text> propertyRent5(new sf::Text("  with 3 houses: ", getFont(), getFontSize() - 2));
	propertyRent5->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 215));
	propertyRent5->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent5);

	std::shared_ptr<sf::Text> propertyRent6(new sf::Text("  with 4 houses: ", getFont(), getFontSize() - 2));
	propertyRent6->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 240));
	propertyRent6->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent6);

	std::shared_ptr<sf::Text> propertyRent7(new sf::Text("  with hotel: ", getFont(), getFontSize() - 2));
	propertyRent7->setPosition(sf::Vector2f(PROPERTY_DATA_POSITION.x + 20, PROPERTY_DATA_POSITION.y + 265));
	propertyRent7->setColor(sf::Color::Black);
	propertyDataTexts_.push_back(propertyRent7);
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

void monopolyGameEngine::addOwnerToPropertyField(Player* player, unsigned int pos) {
	// FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, getBoard()->getFieldById(pos));
	// if (fieldType == STREET)
	// {
	// 	static_cast<StreetField>(getBoard()->getFieldById(pos))
	// 	std::get<PropertyField>(getBoard()->getFieldById(pos).setOwner(player));
	// }
	// else if(fieldType == STATION)
	// {
	// 	std::get<StationField>(getBoard()->getFieldById(pos).setOwner(player));
	// }
	// else	//fieldType == UTILITY
	// {
	// 	std::get<UtilityField>(getBoard()->getFieldById(pos).setOwner(player));
	// }
}