#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <vector>
#include <random>
#include "Board.h"
#include "activeScreen.h"
#include "main.h"

enum TurnState {
	RollDice,
	MovePlayer,

	TurnEnd,
};

class monopolyGameEngine {
	const unsigned int playersMax_ = 4;
	const unsigned int playersMin_ = 2;

	const std::string GAMEBOARD_FILE_PATH = "Monopoly/game_config_json/board.json";
	std::shared_ptr<Board> gameboard_;
	std::vector<Player> players_;

	// gui
	const unsigned int FONT_SIZE = 30;
	sf::Font font_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::vector<std::shared_ptr<sf::Text>> texts_;
	sf::Vector2f ROLL_DICE_BUTTON_POSITION = sf::Vector2f(865, 120);
	std::shared_ptr<Button> rollDiceButton_;
	sf::Vector2f TURN_INFO_TEXT_POSITION = sf::Vector2f(800, 50);
	std::shared_ptr<sf::Text> turnInfoText_;
	sf::Vector2f ROLLED_VALUE_TEXT_POSITION = sf::Vector2f(800, 100);
	std::shared_ptr<sf::Text> rolledValueText_;

	// game staff
	TurnState turnState_;
	unsigned int PLAYERS_MAX = 4;
	unsigned int PLAYERS_MIN = 2;
	unsigned int playerIndexturn_;
	unsigned int getPlayerIndexTurn() const;
	void incPlayerIndexTurn();
	void setTurnState(TurnState newState);
	TurnState getTurnState() const;
	unsigned int getFontSize() const;
	void buttonsWorker();
	void turnInfoTextWorker();
	sf::Vector2f getUpdatePlayerSpritePosition();

	unsigned int rollDice() const;

   public:
	monopolyGameEngine();

	// gui
	void createButtonRollDice();
	void createTextTurnInfo();
	void createTextRolledValue();
	sf::Font& getFont();
	void setFont(sf::Font font);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);
	std::vector<std::shared_ptr<Button>>& getButtons();
	std::vector<std::shared_ptr<sf::Text>>& getTexts();

	// game staff
	void createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list);
	void clearPlayers();
	void createBoard();
	void clearBoard();
	std::shared_ptr<Board> getBoard();
	std::vector<Player>& getPlayers();
	void setPlayerIndexTurn(unsigned int indx);
	void monopolyGameWorker();
};
