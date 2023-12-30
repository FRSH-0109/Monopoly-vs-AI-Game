#include "monopolyGameEngine.h"

monopolyGameEngine::monopolyGameEngine() {
	turnState_ = RollDice;
}

void monopolyGameEngine::createBoard() {
	gameboard_ = std::make_shared<Board>(GAMEBOARD_FILE_PATH);
}

void monopolyGameEngine::createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list) {
	std::vector<sf::Color> PLAYER_COLORS = {sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Yellow};
	int i = 0;
	// TODO Parsować po wektorze playerSettings i generować na jego podstawie przeshufflowaną listę graczy)
	for (auto it : player_settings_list) {
		if (!(it->isNone)) {
			Player new_player = Player();
			new_player.setIsAi(!(it->isHuman));
			new_player.setAiLevel(it->level);
			new_player.setColor(PLAYER_COLORS[i]);
			players_.push_back(new_player);
			++i;
		}
	};
	i = 0;
	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(players_), std::end(players_), rng);
	for (Player& player : players_) {
		player.createSprite();
		if (i % 2 == 0) {
			player.setSpriteOffsetX(0.33f);
		} else {
			player.setSpriteOffsetX(0.66f);
		}
		if (i >= 0 && i < 2) {
			player.setSpriteOffsetY(0.33f);
		} else if (i >=2 && i < 4) {
			player.setSpriteOffsetY(0.66f);
		}
		const sf::Vector2i BOARD_POSITION = gameboard_->getBoardPosition();
		const unsigned int FIELD_WIDTH = std::visit([](Field& field) { return field.getWidth(); }, gameboard_->getFieldById(0));
		const unsigned int FIELD_HEIGHT = std::visit([](Field& field) { return field.getHeight(); }, gameboard_->getFieldById(0));
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
	switch (getPlayerIndexTurn()) {
		case 0:
			turnInfoText_->setString("Turn: Player 1");
			break;

		case 1:
			turnInfoText_->setString("Turn: Player 2");
			break;

		case 2:
			turnInfoText_->setString("Turn: Player 3");
			break;

		case 3:
			turnInfoText_->setString("Turn: Player 4");
			break;
		default:
			break;
	}
}

void monopolyGameEngine::monopolyGameWorker() {
	turnInfoTextWorker();

	switch (getTurnState()) {
		case RollDice:
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
				setTurnState(TurnEnd);
			}
			break;
		case MovePlayer:

			break;
		case TurnEnd:
			incPlayerIndexTurn();
			setTurnState(RollDice);
			break;
		default:
			break;
	}
}

sf::Vector2f monopolyGameEngine::getUpdatePlayerSpritePosition()
{
	float x_offset;
	float y_offset;
	unsigned int player_position = players_[playerIndexturn_].getPositon();
	PossibleFields& curr_field = getBoard()->getFieldById(player_position);
	unsigned int curr_field_width = std::visit([](Field& field) { return field.getWidth(); }, curr_field);
	unsigned int curr_field_height = std::visit([](Field& field) { return field.getHeight(); }, curr_field);
	if (player_position <= 10 ) {
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

void monopolyGameEngine::createTextPlayersInfo()
{
	sf::Vector2f defPos = PLAYERS_INFO_TEXT_POSITION;
	for (int i = 0; i < players_.size(); ++i)
	{
		if(i > 0)
		{
			defPos.x += 140;
		}
		std::shared_ptr<sf::Text> playerText(new sf::Text("Player " + std::to_string(i+1) , getFont(), getFontSize()));
		playerText->setPosition(defPos);
		playerText->setColor(players_[i].getColor());
		playerText->setOutlineColor(sf::Color::Black);
		playerText->setOutlineThickness(2);
		addText(playerText);

		std::shared_ptr<sf::Text> playerMoneyText(new sf::Text("Money " + std::to_string(players_[i].getMoney()) , getFont(), getFontSize()-10));
		playerMoneyText->setPosition(sf::Vector2f(defPos.x, defPos.y + 50));
		playerMoneyText->setColor(sf::Color::Black);
		addText(playerMoneyText);
	}
}