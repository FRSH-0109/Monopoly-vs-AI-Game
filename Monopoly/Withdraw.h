/**
 * @file Withdraw.h
 *
 * @brief Header file for trade/withdraw mechanism
 *        in the monopoly game between players
 *
 * This file contains the declaration of the Withdraw class, which represents
 * the trade and withdraw mechanism in a monopoly game between players.
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

/**
 * @class Withdraw
 *
 * @brief Represents the trade and withdraw mechanism in a monopoly game.
 *
 * The Withdraw class handles the trade and withdraw functionality between players
 * in a monopoly game. It includes features such as choosing players, specifying values,
 * and making decisions during the withdrawal process.
 */

class Withdraw {
   private:
 	// Class constants
	bool ALLOW_ONE_SIDE_BENEFIT_WITHDRAW = 1;		/** If only one side can benefit from second side.*/
	float PROPERTY_SHOW_SCALE = 4;					/** Scale factor for property sprite display.*/
	std::shared_ptr<Board> gameboard_;				/** Pointer to monopoly gameboard object ,source for data about fields.*/
	const unsigned int FONT_SIZE = 30;				/** Font size for rendering text.*/
	sf::Font font_;									/** Font used for rendering text.*/
	std::vector<std::shared_ptr<Button>> buttons_;  /** Vector of buttons used in the withdrawal process.*/
  	std::vector<std::shared_ptr<sf::Text>> texts_;  /** Vector of text objects used in the withdrawal process.*/

	// Choose Player Screen members
	std::shared_ptr<sf::Text> choose_player_text_;	/** Vector of text objects used in the withdrawal choose player process.*/
	sf::Vector2f CHOOSE_PLAYER_TEXT_POSITION = sf::Vector2f(980, 200);	/** Origin position for first text object used in the withdrawal choose player process.*/
	std::shared_ptr<Button> choose_player_1_button_;	/** Pointer to button object for choosing withdrawal player.*/
	std::shared_ptr<Button> choose_player_2_button_;	/** Pointer to button object for choosing withdrawal player.*/
	std::shared_ptr<Button> choose_player_3_button_;	/** Pointer to button object for choosing withdrawal player.*/
	std::shared_ptr<Button> choose_player_4_button_;	/** Pointer to button object for choosing withdrawal player.*/
	sf::Vector2f RESIGN_BUTTON_POSITION = sf::Vector2f(980, 700);	/** Origin position for resign button used in the withdrawal choose player process.*/
	std::shared_ptr<Button> resign_button_;			/** Pointer to button object for resigning from choosing withdrawal player.*/

	// Add Value Screen members
	sf::Vector2f RESIGN_VALUE_BUTTON_POSITION = sf::Vector2f(820, 100);	/** Origin position for resign button used in the withdrawal main process.*/
	std::shared_ptr<Button> resign_value_button_;		/** Pointer to button object for resigning from withdrawal process.*/
	std::shared_ptr<Button> submit_value_button_;		/** Pointer to button object for submitting withdrawal process.*/

	sf::Vector2f COLUMN_1_POSITION = sf::Vector2f(200, 200);	/** Origin position for first column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_1_text_;			/** Pointer to text object for first withdrawal player name.*/
	std::shared_ptr<sf::Text> player_1_money_;		/** Pointer to text object for first withdrawal player money.*/
	unsigned int player1MoneyBuffer_;				/**< Buffer for Player 1's money. */
	unsigned int player1MoneyIndexBuffer_;			/**< Buffer for Player 1's offered money. */
	std::vector<unsigned int> player1Properties_;	/** Vector of properties id's of player1.*/
	std::shared_ptr<Button> plus_1_player_1_button_;	/** Pointer to button for add 1 money to offer value from player1.*/
	std::shared_ptr<Button> plus_10_player_1_button_;	/** Pointer to button for add 10 money to offer value from player1.*/
	std::shared_ptr<Button> plus_100_player_1_button_;	/** Pointer to button for add 100 money to offer value from player1.*/
	std::shared_ptr<Button> minus_1_player_1_button_;	/** Pointer to button for substract 1 money from offer value from player1.*/
	std::shared_ptr<Button> minus_10_player_1_button_;	/** Pointer to button for substract 10 money from offer value from player1.*/
	std::shared_ptr<Button> minus_100_player_1_button_;	/** Pointer to button for substract 100 money from offer value from player1.*/
	sf::Vector2f PROPERTY_PLAYER1_POSITION = sf::Vector2f(150, 450);	/** Origin position for player 1 properties sprite.*/
	sf::Sprite property_player_1_sprite_;				/** Player 1 properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_1_texture_;	/** Player 1 properties texture.*/
	std::vector<std::shared_ptr<sf::Text>> property_player_1_texts_;	/** Vector of text objects used in player 1 property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(350, 900);	/** Origin position for player 1 properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(200, 900);	/** Origin position for player 1 properties previous button.*/
	std::shared_ptr<Button> next_property_player_1_button_;		/** Pointer to button for player 1 next property showing.*/
	std::shared_ptr<Button> previous_propertyplayer_1_button_;	/** Pointer to button for player 1 previous property showing.*/
	unsigned int current_property_player_1_showed_;				/** Field position of currently showed player 1 property*/

	sf::Vector2f COLUMN_2_POSITION = sf::Vector2f(600, 200);	/** Origin position for second column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_1_index_text_;				/** Pointer to text object for player 1 offer name.*/
	std::shared_ptr<sf::Text> player_1_index_money_;				/** Pointer to text object for player 1 offer money.*/
	std::vector<unsigned int> player_1_index_properties_;			/** Vector of properties id's of player 1 offer.*/
	sf::Vector2f PROPERTY_PLAYER1_INDEX_POSITION = sf::Vector2f(550, 450);	/** Origin position for player 1 offered properties sprite.*/
	sf::Sprite property_player_1_index_sprite_;						/** Player 1 offered properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_1_index_texture_;	/** Player 1 offered propertie texture.*/
	std::vector<std::shared_ptr<sf::Text>> property_player_1_index_texts_;	/** Vector of text objects used in player 1 offered property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION = sf::Vector2f(750, 900);	/** Origin position for player 1 offered properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION = sf::Vector2f(600, 900);	/** Origin position for player 1 previous properties next button.*/
	std::shared_ptr<Button> next_property_player_1_index_button_;	/** Pointer to button for player 1 next offered property showing.*/
	std::shared_ptr<Button> previous_property_player_1_index_button_;	/** Pointer to button for player 1 previous offered property showing.*/
	unsigned int current_property_player_1_index_showed_;			/** Field position of currently showed player 1 offered property*/

	sf::Vector2f COLUMN_3_POSITION = sf::Vector2f(1000, 200);	/** Origin position for third column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_2_index_text_;				/** Pointer to text object for player 2 offer name.*/
	std::shared_ptr<sf::Text> player_2_index_money_;				/** Pointer to text object for player 2 offer money.*/
	std::vector<unsigned int> player_2_index_properties_;			/** Vector of properties id's of player 2 offer.*/
	sf::Vector2f PROPERTY_PLAYER2_INDEX_POSITION = sf::Vector2f(950, 450);	/** Origin position for player 2 offered properties sprite.*/
	sf::Sprite property_player_2_index_sprite_;						/** Player 2 offered properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_2_index_texture_;	/** Player 2 offered propertie texture.*/
	std::vector<std::shared_ptr<sf::Text>> property_player_2_index_texts_;	/** Vector of text objects used in player 2 offered property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION = sf::Vector2f(1150, 900);		/** Origin position for player 2 offered properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION = sf::Vector2f(1000, 900);	/** Origin position for player 2 previous properties next button.*/
	std::shared_ptr<Button> next_property_player_2_index_button_;	/** Pointer to button for player 2 next offered property showing.*/
	std::shared_ptr<Button> previousPropertyPlayer2IndexButton_;	/** Pointer to button for player 2 previous offered property showing.*/
	unsigned int current_property_player_2_index_showed_;			/** Field position of currently showed player 2 offered property*/

	sf::Vector2f COLUMN_4_POSITION = sf::Vector2f(1400, 200);	/** Origin position for fourth column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_2_text_;			/** Pointer to text object for second withdrawal player name.*/
	std::shared_ptr<sf::Text> player_2_money_;		/** Pointer to text object for second withdrawal player money.*/
	unsigned int player1MoneyBuffer_;				/**< Buffer for Player 2's money. */
	unsigned int player1MoneyIndexBuffer_;			/**< Buffer for Player 2's offered money. */
	std::vector<unsigned int> player_2_properties_;	/** Vector of properties id's of player 2.*/
	std::shared_ptr<Button> plus_1_player_2_button_;	/** Pointer to button for add 1 money to offer value from player 2.*/
	std::shared_ptr<Button> plus_10_player_2_button_;	/** Pointer to button for add 10 money to offer value from player 2.*/
	std::shared_ptr<Button> plus_100_player_2_button_;	/** Pointer to button for add 100 money to offer value from player 2.*/
	std::shared_ptr<Button> minus_1_player_2_button_;	/** Pointer to button for substract 1 money from offer value from player 2.*/
	std::shared_ptr<Button> minus_10_player_2_button_;	/** Pointer to button for substract 10 money from offer value from player 2.*/
	std::shared_ptr<Button> minus_100_player_2_button_;	/** Pointer to button for substract 100 money from offer value from player 2.*/
	sf::Vector2f PROPERTY_PLAYER2_POSITION = sf::Vector2f(1350, 450);	/** Origin position for player 1 properties sprite.*/
	sf::Sprite property_player_2_sprite_;				/** Player 2 properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_2_texture_;	/** Player 2 properties texture.*/
	std::vector<std::shared_ptr<sf::Text>> propertyPlayer2Texts_;	/** Vector of text objects used in player 1 property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1550, 900);	/** Origin position for player 2 properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1400, 900);	/** Origin position for player 2 properties previous button.*/
	std::shared_ptr<Button> next_property_player_2_button_;		/** Pointer to button for player 2 next property showing.*/
	std::shared_ptr<Button> previous_propertyplayer_2_button_;	/** Pointer to button for player 2 previous property showing.*/
	unsigned int current_property_player_2_showed_;				/** Field position of currently showed player 2 property*/

	sf::Vector2f ADD_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(475, 550);		/** Origin position for player 1 add to offered properties button.*/
	sf::Vector2f REMOVE_PROPERTY_PLAYER1_BUTTON_POSITION = sf::Vector2f(475, 650);	/** Origin position for player 1 remove from offered properties button.*/
	std::shared_ptr<Button> addPropertyPlayer1Button_;								/** Pointer to button for player 1 add to offered properties.*/
	std::shared_ptr<Button> removePropertyPlayer1Button_;							/** Pointer to button for player 1 remove from offered properties.*/
	sf::Vector2f ADD_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1275, 550);	/** Origin position for player 2 add to offered properties button.*/
	sf::Vector2f REMOVE_PROPERTY_PLAYER2_BUTTON_POSITION = sf::Vector2f(1275, 650);	/** Origin position for player 2 remove from offered properties button.*/
	std::shared_ptr<Button> addPropertyPlayer2Button_;								/** Pointer to button for player 2 add to offered properties.*/
	std::shared_ptr<Button> removePropertyPlayer2Button_;							/** Pointer to button for player 2 remove from offered properties.*/

	// Submit Screen members
	sf::Vector2f DECISION_TEXT_POSITION = sf::Vector2f(980, 200);	/** Origin position for player 2 decision stuff.*/
	std::shared_ptr<sf::Text> playerInfoText_;						/** Pointer to text object for player 2 name.*/
	std::shared_ptr<sf::Text> playerInfoMoney_;						/** Pointer to text object for player 2 money balance from withdraw.*/
	std::shared_ptr<sf::Text> playerInfoGetProperties_;				/** Pointer to text object for player 2 wihdraw properties gain.*/
	std::shared_ptr<sf::Text> playerInfoLoseProperties_;			/** Pointer to text object for player 2 wihdraw properties loose.*/
	sf::Vector2f RESIGN_DECISION_BUTTON_POSITION = sf::Vector2f(820, 400);	/** Origin position for player 2 accept withdraw button.*/
	sf::Vector2f ACCEPT_DECISION_BUTTON_POSITION = sf::Vector2f(1140, 400);	/** Origin position for player 2 resign withdraw button.*/
	std::shared_ptr<Button> resignDecisionButton_;					/** Pointer to button for player 2 resign from withdraw decision.*/
	std::shared_ptr<Button> acceptDecisionButton_;					/** Pointer to button for player 2 accept withdraw decision.*/

	TurnState turnStatePrev_;
	std::shared_ptr<Player> player1ToWithDraw_;
	std::shared_ptr<Player> player2ToWithDraw_;

	std::shared_ptr<Button> createDefaultButton(sf::String text, unsigned int width, unsigned int height);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);

   public:
	Withdraw();
	std::vector<unsigned int> getPlayer1IndexProperties();
	std::vector<unsigned int> getPlayer2IndexProperties();
	void setPlayer1IndexProperties(std::vector<unsigned int> new_index_properties);
	void setPlayer2IndexProperties(std::vector<unsigned int> new_index_properties);
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
