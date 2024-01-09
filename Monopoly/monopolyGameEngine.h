#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <vector>
#include "Board.h"
#include "NotificationWall.h"
#include "Withdraw.h"
#include "activeScreen.h"
#include "main.h"

class monopolyGameEngine {
	GameScreenType screenType_ = Boardgame;
	const unsigned int PLAYER_MONEY_DEFAULT_ = 1500;
	const unsigned int START_PASSING_MONEY_ = 200;

	const std::string GAMEBOARD_FILE_PATH_ = "Monopoly/game_config_json/board.json";
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
	sf::Vector2f PLAYERS_INFO_TEXT_POSITION = sf::Vector2f(850, 800);
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
	sf::Vector2f NEXT_PROPERTY_BUTTON_POSITION = sf::Vector2f(1595, 700);
	sf::Vector2f PREVIOUS_PROPERTY_BUTTON_POSITION = sf::Vector2f(1445, 700);
	std::shared_ptr<Button> nextPropertyButton_;
	std::shared_ptr<Button> previousPropertyButton_;
	unsigned int currentPropertyShowed_ = 1;

	std::shared_ptr<sf::Text> availableHousesText_;
	std::shared_ptr<sf::Text> availableHotelsText_;
	sf::Vector2f AVAILABLE_HOUSE_TEXT_POSITION = sf::Vector2f(1675, 110);

	std::shared_ptr<sf::Text> houseText_;
	std::shared_ptr<sf::Text> hotelText_;
	sf::Vector2f HOUSE_TEXT_POSITION = sf::Vector2f(1445, 80);
	sf::Vector2f HOTEL_TEXT_POSITION = sf::Vector2f(1595, 80);
	std::shared_ptr<Button> buyHouseButton_;
	std::shared_ptr<Button> sellHouseButton_;
	std::shared_ptr<Button> buyHotelButton_;
	std::shared_ptr<Button> sellHotelButton_;
	sf::Vector2f BUY_HOUSE_BUTTON_POSITION = sf::Vector2f(1445, 120);
	sf::Vector2f SELL_HOUSE_BUTTON_POSITION = sf::Vector2f(1445, 180);
	sf::Vector2f BUY_HOTEL_BUTTON_POSITION = sf::Vector2f(1595, 120);
	sf::Vector2f SELL_HOTEL_BUTTON_POSITION = sf::Vector2f(1595, 180);

	std::shared_ptr<Button> bankruptButton_;
	std::shared_ptr<Button> nextTurnButton_;
	sf::Vector2f BANKRUPT_BUTTON_POSITION = sf::Vector2f(1245, 120);
	sf::Vector2f NEXT_TURN_BUTTON_POSITION = sf::Vector2f(1245, 180);

	// housese and hotels
	sf::Vector2f houseSize_ = sf::Vector2f(22, 13);
	sf::Texture houseTexture_;
	sf::Texture hotelTexture_;

	// pay jail button
	std::shared_ptr<Button> jailPayButton_;
	sf::Vector2f JAIL_PAY_BUTTON_POSITION = sf::Vector2f(1000, 250);
	const unsigned int JAIL_PAY_MONEY = 50;

	// withdraw
	Withdraw withdraw_;
	std::shared_ptr<Button> withdrawButton_;
	sf::Vector2f WITHDRAW_BUTTON_POSITION = sf::Vector2f(400, 900);

	NotificationWall notificationsWall_;

	// game staff
	TurnState turnState_;
	const unsigned int PLAYERS_MAX_ = 4;
	const unsigned int PLAYERS_MIN_ = 2;
	unsigned int playersStartingAmount_ = 0;
	unsigned int house_count_ = 32;
	unsigned int hotel_count_ = 12;
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

	bool isButtonClicked(std::shared_ptr<Button> button_ptr);
	unsigned int rollDice() const;

	void movePlayer(unsigned int turnIndex, unsigned int positionIncrement);
	void sendToJail(unsigned int turnIndex);
	void notificationAdd(unsigned int index, std::string text);
	void handlePassingStart(unsigned int oldPos, unsigned int newPos);
	bool makePlayerBankrupt(unsigned int playerIndexTurn);
	void showAllPropertiesWorker();

	// withdraw
	void withdrawWorker();

   public:
	monopolyGameEngine();

	void setScreenType(GameScreenType newScreenType);
	const GameScreenType getScreenType() const;

	// gui
	void createButtonRollDice();
	void createTextTurnInfo();
	void createTextRolledValue();
	void createTextPlayersInfo();
	void updateTextPlayersInfo();
	void createButtonBuyResign();
	void createButtonNextProperty();
	void createButtonPerviousProperty();
	void createButtonsBuySellHouseHotel();
	void createButtonsBankrupt();
	void createButtonsNextTurn();
	void createButtonsJailPay();
	void createButtonWithdraw();
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
	sf::Texture& getHouseTexture();
	sf::Texture& getHotelTexture();
	sf::Vector2f& getHouseSize();

	// game staff
	void createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list);
	void clearPlayers();
	void createBoard();
	void clearBoard();
	std::shared_ptr<Board> getBoard();
	std::vector<std::shared_ptr<Player>>& getPlayers();
	void setPlayerIndexTurn(unsigned int indx);
	TurnState getTurnState() const;
	unsigned int getHouseCount();
	unsigned int getHotelCount();
	void setHouseCount(unsigned int new_count);
	void setHotelCount(unsigned int new_count);

	void addHouses(unsigned int added_amount);
	void substractHouses(unsigned int substracted_amount);
	void addHotels(unsigned int added_amount);
	void substractHotels(unsigned int substracted_amount);

	unsigned int calculateGroupFieldsOwned(std::vector<unsigned int> player_fields, PropertyField& field) const;
	bool groupCompleted(std::vector<unsigned int> player_fields, PropertyField& field) const;

	bool isBuildingLegal(std::shared_ptr<Player> builder, StreetField& field);
	bool isDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field);
	bool isHotelBuildingLegal(std::shared_ptr<Player> builder, StreetField& field);
	bool isHotelDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field);
	sf::Sprite getHouseSprite(StreetField& field, unsigned int housesNumber);
	sf::Sprite getHotelSprite(StreetField& field);
	void createAvailableHousesHotelText();
	void updateAvailableHousesHotelText();

	unsigned int calculateRent(unsigned int rolledVal, int pos);
	void buildingsManagingWorker();
	void monopolyGameWorker();

	// withdraw
	Withdraw& getWithdraw();
};
