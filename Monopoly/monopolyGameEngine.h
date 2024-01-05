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
	std::vector<std::shared_ptr<Player>> players_;

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

	sf::Vector2f ALL_PROPERTY_DATA_POSITION = sf::Vector2f(1400, 260);
	sf::Sprite allPropertyDataSprite_;
	sf::Texture allPropertyDataTexture_;
	std::vector<std::shared_ptr<sf::Text>> allPropertyDataTexts_;
	sf::Vector2f NEXT_PROPERTY_BUTTON_POSITION = sf::Vector2f(1595, 220);
	sf::Vector2f PREVIOUS_PROPERTY_BUTTON_POSITION = sf::Vector2f(1445, 220);
	std::shared_ptr<Button> nextPropertyButton_;
	std::shared_ptr<Button> previousPropertyButton_;
	unsigned int currentPropertyShowed_ = 1;
	sf::Time BUTTON_CLICK_DELAY_MS = sf::milliseconds(1000);

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
	void buttonsWorker();
	void turnInfoTextWorker();
	sf::Vector2f getUpdatePlayerSpritePosition();
	void clearPropertyData(bool isPropertyShownToBuy);
	unsigned int getFieldPriceByPosition(unsigned int pos);
	void addOwnerToPropertyField(std::shared_ptr<Player> player, unsigned int pos);

	bool isRollDiceButtonClicked();
	unsigned int rollDice() const;

	void movePlayer(unsigned int turnIndex, unsigned int positionIncrement);
	void notificationAdd(unsigned int index, std::string text);
	void handlePassingStart(unsigned int oldPos, unsigned int newPos);
	void showAllPropertiesWorker();

   public:
	monopolyGameEngine();

	// gui
	void createButtonRollDice();
	void createTextTurnInfo();
	void createTextRolledValue();
	void createTextPlayersInfo();
	void updateTextPlayersInfo();
	void createButtonBuyResign();
	void createButtonNextProperty();
	void createButtonPerviousProperty();
	void showPropertyData(unsigned int pos, bool isPropertyShownToBuy);
	sf::Font& getFont();
	unsigned int getFontSize() const;
	void setFont(sf::Font font);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);
	std::vector<std::shared_ptr<Button>>& getButtons();
	std::vector<std::shared_ptr<sf::Text>>& getTexts();
	sf::Sprite& getPropertyDataSprite();
	std::vector<std::shared_ptr<sf::Text>>& getPropertyDataTexts();
	sf::Sprite& getAllPropertyDataSprite();
	std::vector<std::shared_ptr<sf::Text>>& getAllPropertyDataTexts();
	NotificationWall& getNotificationsWall();
	sf::Text getPropertyNameToDraw(sf::Text text, sf::Sprite& sprite, float rotation);

	// game staff
	void createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list);
	void clearPlayers();
	void createBoard();
	void clearBoard();
	std::shared_ptr<Board> getBoard();
	std::vector<std::shared_ptr<Player>>& getPlayers();
	void setPlayerIndexTurn(unsigned int indx);
	TurnState getTurnState() const;

	unsigned int calculateGroupFieldsOwned(std::vector<unsigned int> player_fields, PropertyField& field) const;
	bool groupCompleted(std::vector<unsigned int> player_fields, PropertyField& field) const;

	bool isBuildingLegal(std::shared_ptr<Player> builder, StreetField& field);
	bool isDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field);
	bool isHotelBuildingLegal(std::shared_ptr<Player> builder, StreetField& field);
	bool isHotelDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field);

	unsigned int calculateRent(unsigned int rolledVal, int pos);
	void monopolyGameWorker();
};
