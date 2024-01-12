/**
 * @file Withdraw.h
 *
 * @brief Header file for trade/withdraw mehanism
 * in monopoly game between players
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <variant>
#include <vector>
#include "Board.h"
#include "Button.h"
#include "Field.h"
#include "Player.h"
#include "main.h"

class Withdraw {
   private:
	bool ALLOW_ONE_SIDE_BENEFIT_WITHDRAW = 1;
	float PROPERTY_SHOW_SCALE = 4;
	std::shared_ptr<Board> gameboard_;
	const unsigned int FONT_SIZE = 30;
	sf::Font font_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::vector<std::shared_ptr<sf::Text>> texts_;

	// choose player screen
	std::shared_ptr<sf::Text> choosePlayerText_;
	sf::Vector2f CHOOSE_PLAYER_TEXT_POSITION = sf::Vector2f(980, 200);
	std::shared_ptr<Button> choosePlayer1Button_;
	std::shared_ptr<Button> choosePlayer2Button_;
	std::shared_ptr<Button> choosePlayer3Button_;
	std::shared_ptr<Button> choosePlayer4Button_;
	sf::Vector2f RESIGN_BUTTON_POSITION = sf::Vector2f(980, 700);
	std::shared_ptr<Button> resignButton_;

	// add value screen
	sf::Vector2f RESIGN_VALUE_BUTTON_POSITION = sf::Vector2f(820, 100);
	std::shared_ptr<Button> resignValueButton_;
	std::shared_ptr<Button> submitValueButton_;

	sf::Vector2f COLUMN_1_POSITION = sf::Vector2f(200, 200);
	std::shared_ptr<sf::Text> player1Text_;
	std::shared_ptr<sf::Text> player1Money_;
	unsigned int player1MoneyBuffer_;
	unsigned int player1MoneyIndexBuffer_;
	std::vector<unsigned int> player1Properties_;
	std::shared_ptr<Button> plus1Player1Button_;
	std::shared_ptr<Button> plus10Player1Button_;
	std::shared_ptr<Button> plus100Player1Button_;
	std::shared_ptr<Button> minus1Player1Button_;
	std::shared_ptr<Button> minus10Player1Button_;
	std::shared_ptr<Button> minus100Player1Button_;
	sf::Vector2f PROPERTY_PLAYER1_POSITION = sf::Vector2f(150, 450);
	sf::Sprite propertyPlayer1Sprite_;
	std::shared_ptr<sf::Texture> propertyPlayer1Texture_;
	std::vector<std::shared_ptr<sf::Text>> propertyPlayer1Texts_;
	sf::Vector2f NEXT_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(350, 900);
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(200, 900);
	std::shared_ptr<Button> nextPropertyPlayer1Button_;
	std::shared_ptr<Button> previousPropertyPlayer1Button_;
	unsigned int currentPropertyPlayer1Showed_;

	sf::Vector2f COLUMN_2_POSITION = sf::Vector2f(600, 200);
	std::shared_ptr<sf::Text> player1IndexText_;
	std::shared_ptr<sf::Text> player1IndexMoney_;
	std::vector<unsigned int> player1IndexProperties_;
	sf::Vector2f PROPERTY_PLAYER1_INDEX_POSITION = sf::Vector2f(550, 450);
	sf::Sprite propertyPlayer1IndexSprite_;
	std::shared_ptr<sf::Texture> propertyPlayer1IndexTexture_;
	std::vector<std::shared_ptr<sf::Text>> propertyPlayer1IndexTexts_;
	sf::Vector2f NEXT_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION = sf::Vector2f(750, 900);
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION = sf::Vector2f(600, 900);
	std::shared_ptr<Button> nextPropertyPlayer1IndexButton_;
	std::shared_ptr<Button> previousPropertyPlayer1IndexButton_;
	unsigned int currentPropertyPlayer1IndexShowed_;

	sf::Vector2f COLUMN_3_POSITION = sf::Vector2f(1000, 200);
	std::shared_ptr<sf::Text> player2IndexText_;
	std::shared_ptr<sf::Text> player2IndexMoney_;
	std::vector<unsigned int> player2IndexProperties_;
	sf::Vector2f PROPERTY_PLAYER2_INDEX_POSITION = sf::Vector2f(950, 450);
	sf::Sprite propertyPlayer2IndexSprite_;
	std::shared_ptr<sf::Texture> propertyPlayer2IndexTexture_;
	std::vector<std::shared_ptr<sf::Text>> propertyPlayer2IndexTexts_;
	sf::Vector2f NEXT_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION = sf::Vector2f(1150, 900);
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION = sf::Vector2f(1000, 900);
	std::shared_ptr<Button> nextPropertyPlayer2IndexButton_;
	std::shared_ptr<Button> previousPropertyPlayer2IndexButton_;
	unsigned int currentPropertyPlayer2IndexShowed_;

	sf::Vector2f COLUMN_4_POSITION = sf::Vector2f(1400, 200);
	std::shared_ptr<sf::Text> player2Text_;
	std::shared_ptr<sf::Text> player2Money_;
	unsigned int player2MoneyBuffer_;
	unsigned int player2MoneyIndexBuffer_;
	std::vector<unsigned int> player2Properties_;
	std::shared_ptr<Button> plus1Player2Button_;
	std::shared_ptr<Button> plus10Player2Button_;
	std::shared_ptr<Button> plus100Player2Button_;
	std::shared_ptr<Button> minus1Player2Button_;
	std::shared_ptr<Button> minus10Player2Button_;
	std::shared_ptr<Button> minus100Player2Button_;
	sf::Vector2f PROPERTY_PLAYER2_POSITION = sf::Vector2f(1350, 450);
	sf::Sprite propertyPlayer2Sprite_;
	std::shared_ptr<sf::Texture> propertyPlayer2Texture_;
	std::vector<std::shared_ptr<sf::Text>> propertyPlayer2Texts_;
	sf::Vector2f NEXT_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1550, 900);
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1400, 900);
	std::shared_ptr<Button> nextPropertyPlayer2Button_;
	std::shared_ptr<Button> previousPropertyPlayer2Button_;
	unsigned int currentPropertyPlayer2Showed_;

	sf::Vector2f ADD_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(475, 550);
	sf::Vector2f REMOVE_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(475, 650);
	std::shared_ptr<Button> addPropertyPlayer1Button_;
	std::shared_ptr<Button> removePropertyPlayer1Button_;
	sf::Vector2f ADD_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1275, 550);
	sf::Vector2f REMOVE_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1275, 650);
	std::shared_ptr<Button> addPropertyPlayer2Button_;
	std::shared_ptr<Button> removePropertyPlayer2Button_;

	// submit
	sf::Vector2f DECISION_TEXT_POSITION = sf::Vector2f(980, 200);
	std::shared_ptr<sf::Text> playerInfoText_;
	std::shared_ptr<sf::Text> playerInfoMoney_;
	std::shared_ptr<sf::Text> playerInfoGetProperties_;
	std::shared_ptr<sf::Text> playerInfoLoseProperties_;
	sf::Vector2f RESIGN_DECISION_BUTTON_POSITION = sf::Vector2f(820, 400);
	sf::Vector2f ACCEPT_DECISION_BUTTON_POSITION = sf::Vector2f(1140, 400);
	std::shared_ptr<Button> resignDecisionButton_;
	std::shared_ptr<Button> acceptDecisionButton_;

	std::shared_ptr<Button> createDefaultButton(std::string text, unsigned int width, unsigned int height);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);

	TurnState turnStatePrev_;
	std::shared_ptr<Player> player1ToWithDraw_;
	std::shared_ptr<Player> player2ToWithDraw_;

   public:
	Withdraw();
	void setBoard(std::shared_ptr<Board> board_ptr);
	void setTurnState(TurnState state);
	TurnState getTurnState();
	sf::Font& getFont();
	unsigned int getFontSize() const;
	void setFont(sf::Font font);
	std::vector<std::shared_ptr<Button>>& getButtons();
	std::vector<std::shared_ptr<sf::Text>>& getTexts();
	std::shared_ptr<Button> getPlayer1Button();
	std::shared_ptr<Button> getPlayer2Button();
	std::shared_ptr<Button> getPlayer3Button();
	std::shared_ptr<Button> getPlayer4Button();
	std::shared_ptr<Button> getResignButton();
	void createChoosePlayerScreen();
	void createValuePlayerScreen();
	void createDecisionPlayerScreen();
	void setChooseScreenVisible(bool isVisible);
	void setValueScreenVisible(bool isVisible);
	void setDecisionScreenVisible(bool isVisible);
	void setPlayer1ToWithdraw(std::shared_ptr<Player> player_ptr);
	void setPlayer2ToWithdraw(std::shared_ptr<Player> player_ptr);
	std::shared_ptr<Player> getPlayer1ToWithdraw();
	std::shared_ptr<Player> getPlayer2ToWithdraw();
	std::shared_ptr<Button> getResignValueButton();
	std::shared_ptr<Button> getSubmitValueButton();

	std::shared_ptr<Button> getPlayer1minus1();
	std::shared_ptr<Button> getPlayer1minus10();
	std::shared_ptr<Button> getPlayer1minus100();
	std::shared_ptr<Button> getPlayer1plus1();
	std::shared_ptr<Button> getPlayer1plus10();
	std::shared_ptr<Button> getPlayer1plus100();
	std::shared_ptr<Button> getPlayer2minus1();
	std::shared_ptr<Button> getPlayer2minus10();
	std::shared_ptr<Button> getPlayer2minus100();
	std::shared_ptr<Button> getPlayer2plus1();
	std::shared_ptr<Button> getPlayer2plus10();
	std::shared_ptr<Button> getPlayer2plus100();
	std::shared_ptr<Button> getPlayer1NextButton();
	std::shared_ptr<Button> getPlayer1PreviousButton();
	std::shared_ptr<Button> getPlayer2NextButton();
	std::shared_ptr<Button> getPlayer2PreviousButton();
	std::shared_ptr<Button> getPlayer1IndexNextButton();
	std::shared_ptr<Button> getPlayer1IndexPreviousButton();
	std::shared_ptr<Button> getPlayer2IndexNextButton();
	std::shared_ptr<Button> getPlayer2IndexPreviousButton();
	std::shared_ptr<Button> getPlayer1AddButton();
	std::shared_ptr<Button> getPlayer1RemoveButton();
	std::shared_ptr<Button> getPlayer2AddButton();
	std::shared_ptr<Button> getPlayer2RemoveButton();

	// submit
	std::shared_ptr<Button> getResignDecisionButton();
	std::shared_ptr<Button> getAcceptDecisionButton();

	void moneyTransferIndex(unsigned int playerNum, int money);
	void moneyTextUpdate();
	void makeWithdraw();

	void showProperty(int column);
	sf::Sprite& getSpritePropertyPlayer1();
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer1();
	sf::Sprite& getSpritePropertyPlayer1Index();
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer1Index();
	sf::Sprite& getSpritePropertyPlayer2();
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer2();
	sf::Sprite& getSpritePropertyPlayer2Index();
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer2Index();
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer1();
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer1Index();
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer2();
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer2Index();
	void addPropertyPlayerShowed(int i, unsigned int col);
	void propertyPlayerMoveIndex(int dir, unsigned int plrNum);
	bool isNonZeroValue();
};

#endif