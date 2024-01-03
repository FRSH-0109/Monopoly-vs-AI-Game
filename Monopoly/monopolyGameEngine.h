#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <vector>
#include "Board.h"
#include "NotificationWall.h"
#include "activeScreen.h"
#include "main.h"

enum TurnState {
	RollDice,
	FieldAction,
	BuyAction,
	PayRent,
	TurnEnd,
};

class monopolyGameEngine {
	const unsigned int PLAYER_MONEY_DEFAULT = 1500;
	const unsigned int START_PASSING_MONEY_ = 200;

	const std::string GAMEBOARD_FILE_PATH = "Monopoly/game_config_json/board.json";
	std::shared_ptr<Board> gameboard_;
	std::vector<Player> players_;

	// gui
	const unsigned int FONT_SIZE = 30;
	sf::Font font_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::vector<std::shared_ptr<sf::Text>> texts_;
	sf::Vector2f ROLL_DICE_BUTTON_POSITION = sf::Vector2f(965, 120);
	std::shared_ptr<Button> rollDiceButton_;
	sf::Vector2f TURN_INFO_TEXT_POSITION = sf::Vector2f(900, 50);
	std::shared_ptr<sf::Text> turnInfoText_;
	sf::Vector2f ROLLED_VALUE_TEXT_POSITION = sf::Vector2f(900, 100);
	std::shared_ptr<sf::Text> rolledValueText_;
	sf::Vector2f PLAYERS_INFO_TEXT_POSITION = sf::Vector2f(850, 700);
	std::array<std::vector<std::shared_ptr<sf::Text>>, 4> playerInfoText_;

	sf::Vector2f BUY_BUTTON_POSITION = sf::Vector2f(965, 220);
	std::shared_ptr<Button> buyFieldButton_;
	std::shared_ptr<Button> resignBuyFieldButton_;

	sf::Vector2f PROPERTY_DATA_POSITION = sf::Vector2f(910, 260);
	float PROPERTY_DATA_SCALE = 4;
	sf::Sprite propertyDataSprite_;
	sf::Texture propertyDataTexture_;
	std::vector<std::shared_ptr<sf::Text>> propertyDataTexts_;

	NotificationWall notificationsWall_;

	// game staff
	TurnState turnState_;
	const unsigned int PLAYERS_MAX = 4;
	const unsigned int PLAYERS_MIN = 2;
	unsigned int playersStartingAmount_ = 0;
	unsigned int playerIndexturn_;
	unsigned int getPlayerIndexTurn() const;
	void incPlayerIndexTurn();
	void setTurnState(TurnState newState);
	unsigned int getFontSize() const;
	void buttonsWorker();
	void turnInfoTextWorker();
	sf::Vector2f getUpdatePlayerSpritePosition();
	void clearPropertyData();
	void showPropertyData(unsigned int pos);
	unsigned int getFieldPriceByPosition(unsigned int pos);
	void addOwnerToPropertyField(std::shared_ptr<Player> player, unsigned int pos);

	bool isRollDiceButtonClicked();
	unsigned int rollDice() const;

	void movePlayer(unsigned int turnIndex, unsigned int positionIncrement);
	void notificationAdd(unsigned int index, std::string text);
	void handlePassingStart(unsigned int oldPos, unsigned int newPos);

   public:
	monopolyGameEngine();

	// gui
	void createButtonRollDice();
	void createTextTurnInfo();
	void createTextRolledValue();
	void createTextPlayersInfo();
	void updateTextPlayersInfo();
	void createButtonBuyResign();
	sf::Font& getFont();
	void setFont(sf::Font font);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);
	std::vector<std::shared_ptr<Button>>& getButtons();
	std::vector<std::shared_ptr<sf::Text>>& getTexts();
	sf::Sprite& getPropertyDataSprite();
	std::vector<std::shared_ptr<sf::Text>>& getPropertyDataTexts();
	NotificationWall& getNotificationsWall();

	// game staff
	void createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list);
	void clearPlayers();
	void createBoard();
	void clearBoard();
	std::shared_ptr<Board> getBoard();
	std::vector<Player>& getPlayers();
	void setPlayerIndexTurn(unsigned int indx);
	TurnState getTurnState() const;
	unsigned int calculateGroupFieldsOwned(std::vector<unsigned int> player_fields, PropertyField& field) const;
	bool groupCompleted(std::vector<unsigned int> player_fields, PropertyField& field) const;
	unsigned int calculateRent(unsigned int rolledVal, int pos);
	void monopolyGameWorker();
};
