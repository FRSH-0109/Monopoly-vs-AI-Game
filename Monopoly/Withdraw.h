/**
 * @file Withdraw.h
 *
 * @brief Header file for trade/withdraw mechanism
 *        in the monopoly game between players
 *
 * This file contains the declaration of the Withdraw class, which represents
 * the trade and withdraw mechanism in a monopoly game between players.
 * Player 1 - active in withdraw (offering one)
 * Player 2 - passive in withdraw (decision maker)
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
	bool ALLOW_ONE_SIDE_BENEFIT_WITHDRAW = 1; /** If only one side can benefit from second side.*/
	float PROPERTY_SHOW_SCALE = 4;			  /** Scale factor for property sprite display.*/
	std::shared_ptr<Board> gameboard_;		  /** Pointer to monopoly gameboard object ,source for data about fields.*/
	const unsigned int FONT_SIZE = 30;		  /** Font size for rendering text.*/
	sf::Font font_;							  /** Font used for rendering text.*/
	std::vector<std::shared_ptr<Button>> buttons_; /** Vector of buttons used in the withdrawal process.*/
	std::vector<std::shared_ptr<sf::Text>> texts_; /** Vector of text objects used in the withdrawal process.*/

	// Choose Player Screen members
	std::shared_ptr<sf::Text>
		choose_player_text_; /** Vector of text objects used in the withdrawal choose player process.*/
	sf::Vector2f CHOOSE_PLAYER_TEXT_POSITION = sf::Vector2f(980,
		200); /** Origin position for first text object used in the withdrawal choose player process.*/
	std::shared_ptr<Button> choose_player_1_button_; /** Pointer to button object for choosing withdrawal player.*/
	std::shared_ptr<Button> choose_player_2_button_; /** Pointer to button object for choosing withdrawal player.*/
	std::shared_ptr<Button> choose_player_3_button_; /** Pointer to button object for choosing withdrawal player.*/
	std::shared_ptr<Button> choose_player_4_button_; /** Pointer to button object for choosing withdrawal player.*/
	sf::Vector2f RESIGN_BUTTON_POSITION =
		sf::Vector2f(980, 700); /** Origin position for resign button used in the withdrawal choose player process.*/
	std::shared_ptr<Button>
		resign_button_; /** Pointer to button object for resigning from choosing withdrawal player.*/

	// Add Value Screen members
	sf::Vector2f RESIGN_VALUE_BUTTON_POSITION =
		sf::Vector2f(820, 100); /** Origin position for resign button used in the withdrawal main process.*/
	std::shared_ptr<Button> resign_value_button_; /** Pointer to button object for resigning from withdrawal process.*/
	std::shared_ptr<Button> submit_value_button_; /** Pointer to button object for submitting withdrawal process.*/

	sf::Vector2f COLUMN_1_POSITION =
		sf::Vector2f(200, 200);					  /** Origin position for first column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_1_text_;	  /** Pointer to text object for first withdrawal player name.*/
	std::shared_ptr<sf::Text> player_1_money_;	  /** Pointer to text object for first withdrawal player money.*/
	unsigned int player_1_money_buffer_;		  /**< Buffer for Player 1's money. */
	unsigned int player_1_money_index_buffer_;	  /**< Buffer for Player 1's offered money. */
	std::vector<unsigned int> player1Properties_; /** Vector of properties id's of player1.*/
	std::shared_ptr<Button>
		plus_1_player_1_button_; /** Pointer to button for add 1 money to offer value from player1.*/
	std::shared_ptr<Button>
		plus_10_player_1_button_; /** Pointer to button for add 10 money to offer value from player1.*/
	std::shared_ptr<Button>
		plus_100_player_1_button_; /** Pointer to button for add 100 money to offer value from player1.*/
	std::shared_ptr<Button>
		minus_1_player_1_button_; /** Pointer to button for substract 1 money from offer value from player1.*/
	std::shared_ptr<Button>
		minus_10_player_1_button_; /** Pointer to button for substract 10 money from offer value from player1.*/
	std::shared_ptr<Button>
		minus_100_player_1_button_; /** Pointer to button for substract 100 money from offer value from player1.*/
	sf::Vector2f PROPERTY_PLAYER1_POSITION =
		sf::Vector2f(150, 450);								 /** Origin position for player 1 properties sprite.*/
	sf::Sprite property_player_1_sprite_;					 /** Player 1 properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_1_texture_; /** Player 1 properties texture.*/
	std::vector<std::shared_ptr<sf::Text>>
		property_player_1_texts_; /** Vector of text objects used in player 1 property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER1_BUTTON_POSITION =
		sf::Vector2f(350, 900); /** Origin position for player 1 properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER1_BUTTON_POSITION =
		sf::Vector2f(200, 900); /** Origin position for player 1 properties previous button.*/
	std::shared_ptr<Button> next_property_player_1_button_; /** Pointer to button for player 1 next property showing.*/
	std::shared_ptr<Button>
		previous_propertyplayer_1_button_;			/** Pointer to button for player 1 previous property showing.*/
	unsigned int current_property_player_1_showed_; /** Field position of currently showed player 1 property*/

	sf::Vector2f COLUMN_2_POSITION =
		sf::Vector2f(600, 200);						 /** Origin position for second column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_1_index_text_;	 /** Pointer to text object for player 1 offer name.*/
	std::shared_ptr<sf::Text> player_1_index_money_; /** Pointer to text object for player 1 offer money.*/
	std::vector<unsigned int> player_1_index_properties_; /** Vector of properties id's of player 1 offer.*/
	sf::Vector2f PROPERTY_PLAYER1_INDEX_POSITION =
		sf::Vector2f(550, 450);					/** Origin position for player 1 offered properties sprite.*/
	sf::Sprite property_player_1_index_sprite_; /** Player 1 offered properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_1_index_texture_; /** Player 1 offered propertie texture.*/
	std::vector<std::shared_ptr<sf::Text>>
		property_player_1_index_texts_; /** Vector of text objects used in player 1 offered property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION =
		sf::Vector2f(750, 900); /** Origin position for player 1 offered properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION =
		sf::Vector2f(600, 900); /** Origin position for player 1 previous properties next button.*/
	std::shared_ptr<Button>
		next_property_player_1_index_button_; /** Pointer to button for player 1 next offered property showing.*/
	std::shared_ptr<Button> previous_property_player_1_index_button_; /** Pointer to button for player 1 previous
																		 offered property showing.*/
	unsigned int
		current_property_player_1_index_showed_; /** Field position of currently showed player 1 offered property*/

	sf::Vector2f COLUMN_3_POSITION =
		sf::Vector2f(1000, 200);					 /** Origin position for third column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_2_index_text_;	 /** Pointer to text object for player 2 offer name.*/
	std::shared_ptr<sf::Text> player_2_index_money_; /** Pointer to text object for player 2 offer money.*/
	std::vector<unsigned int> player_2_index_properties_; /** Vector of properties id's of player 2 offer.*/
	sf::Vector2f PROPERTY_PLAYER2_INDEX_POSITION =
		sf::Vector2f(950, 450);					/** Origin position for player 2 offered properties sprite.*/
	sf::Sprite property_player_2_index_sprite_; /** Player 2 offered properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_2_index_texture_; /** Player 2 offered propertie texture.*/
	std::vector<std::shared_ptr<sf::Text>>
		property_player_2_index_texts_; /** Vector of text objects used in player 2 offered property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION =
		sf::Vector2f(1150, 900); /** Origin position for player 2 offered properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION =
		sf::Vector2f(1000, 900); /** Origin position for player 2 previous properties next button.*/
	std::shared_ptr<Button>
		next_property_player_2_index_button_; /** Pointer to button for player 2 next offered property showing.*/
	std::shared_ptr<Button>
		previousPropertyPlayer2IndexButton_; /** Pointer to button for player 2 previous offered property showing.*/
	unsigned int
		current_property_player_2_index_showed_; /** Field position of currently showed player 2 offered property*/

	sf::Vector2f COLUMN_4_POSITION =
		sf::Vector2f(1400, 200);					/** Origin position for fourth column of withdrawal main process.*/
	std::shared_ptr<sf::Text> player_2_text_;		/** Pointer to text object for second withdrawal player name.*/
	std::shared_ptr<sf::Text> player_2_money_;		/** Pointer to text object for second withdrawal player money.*/
	unsigned int player_2_money_buffer_;			/**< Buffer for Player 2's money. */
	unsigned int player_2_money_index_buffer_;		/**< Buffer for Player 2's offered money. */
	std::vector<unsigned int> player_2_properties_; /** Vector of properties id's of player 2.*/
	std::shared_ptr<Button>
		plus_1_player_2_button_; /** Pointer to button for add 1 money to offer value from player 2.*/
	std::shared_ptr<Button>
		plus_10_player_2_button_; /** Pointer to button for add 10 money to offer value from player 2.*/
	std::shared_ptr<Button>
		plus_100_player_2_button_; /** Pointer to button for add 100 money to offer value from player 2.*/
	std::shared_ptr<Button>
		minus_1_player_2_button_; /** Pointer to button for substract 1 money from offer value from player 2.*/
	std::shared_ptr<Button>
		minus_10_player_2_button_; /** Pointer to button for substract 10 money from offer value from player 2.*/
	std::shared_ptr<Button>
		minus_100_player_2_button_; /** Pointer to button for substract 100 money from offer value from player 2.*/
	sf::Vector2f PROPERTY_PLAYER2_POSITION =
		sf::Vector2f(1350, 450);							 /** Origin position for player 1 properties sprite.*/
	sf::Sprite property_player_2_sprite_;					 /** Player 2 properties sprite.*/
	std::shared_ptr<sf::Texture> property_player_2_texture_; /** Player 2 properties texture.*/
	std::vector<std::shared_ptr<sf::Text>>
		propertyPlayer2Texts_; /** Vector of text objects used in player 1 property description.*/
	sf::Vector2f NEXT_PROPERTY_PLAYER2_BUTTON_POSITION =
		sf::Vector2f(1550, 900); /** Origin position for player 2 properties next button.*/
	sf::Vector2f PREVIOUS_PROPERTY_PLAYER2_BUTTON_POSITION =
		sf::Vector2f(1400, 900); /** Origin position for player 2 properties previous button.*/
	std::shared_ptr<Button> next_property_player_2_button_; /** Pointer to button for player 2 next property showing.*/
	std::shared_ptr<Button>
		previous_propertyplayer_2_button_;			/** Pointer to button for player 2 previous property showing.*/
	unsigned int current_property_player_2_showed_; /** Field position of currently showed player 2 property*/

	sf::Vector2f ADD_PROPERTY_PLAYER1_BUTTON_POSITION =
		sf::Vector2f(475, 550); /** Origin position for player 1 add to offered properties button.*/
	sf::Vector2f REMOVE_PROPERTY_PLAYER1_BUTTON_POSITION =
		sf::Vector2f(475, 650); /** Origin position for player 1 remove from offered properties button.*/
	std::shared_ptr<Button>
		add_property_player_1_button_; /** Pointer to button for player 1 add to offered properties.*/
	std::shared_ptr<Button>
		remove_property_player_1_button_; /** Pointer to button for player 1 remove from offered properties.*/
	sf::Vector2f ADD_PROPERTY_PLAYER2_BUTTON_POSITION =
		sf::Vector2f(1275, 550); /** Origin position for player 2 add to offered properties button.*/
	sf::Vector2f REMOVE_PROPERTY_PLAYER2_BUTTON_POSITION =
		sf::Vector2f(1275, 650); /** Origin position for player 2 remove from offered properties button.*/
	std::shared_ptr<Button>
		add_property_player_2_button_; /** Pointer to button for player 2 add to offered properties.*/
	std::shared_ptr<Button>
		remove_property_player_2_button_; /** Pointer to button for player 2 remove from offered properties.*/

	// Submit Screen members
	sf::Vector2f DECISION_TEXT_POSITION = sf::Vector2f(980, 200); /** Origin position for player 2 decision stuff.*/
	std::shared_ptr<sf::Text> player_info_text_;				  /** Pointer to text object for player 2 name.*/
	std::shared_ptr<sf::Text> player_info_money_; /** Pointer to text object for player 2 money balance from withdraw.*/
	std::shared_ptr<sf::Text>
		player_info_get_properties_; /** Pointer to text object for player 2 wihdraw properties gain.*/
	std::shared_ptr<sf::Text>
		player_info_lose_properties_; /** Pointer to text object for player 2 wihdraw properties loose.*/
	sf::Vector2f RESIGN_DECISION_BUTTON_POSITION =
		sf::Vector2f(820, 400); /** Origin position for player 2 accept withdraw button.*/
	sf::Vector2f ACCEPT_DECISION_BUTTON_POSITION =
		sf::Vector2f(1140, 400);					 /** Origin position for player 2 resign withdraw button.*/
	std::shared_ptr<Button> resign_decision_button_; /** Pointer to button for player 2 resign from withdraw decision.*/
	std::shared_ptr<Button> accpet_decision_button_; /** Pointer to button for player 2 accept withdraw decision.*/

	TurnState turn_state_prev_;						/** Buffer for monopoly engine turn when withdraw was started.*/
	std::shared_ptr<Player> player_1_to_with_draw_; /** Pointer for player 1 of withdraw (active)*/
	std::shared_ptr<Player> player_2_to_with_draw_; /** Pointer for player 2 of withdraw (passive)*/

	/**
	 * Add a button to vector of all buttons for withdraw handling
	 *
	 * @param text sf::String object for button text
	 * @param width width of Button object sprite
	 * @param height height of Button object sprite
	 * @return Pointer to created Button object instance
	 */
	std::shared_ptr<Button> createDefaultButton(sf::String text, unsigned int width, unsigned int height);

	/**
	 * Add a button to vector of all buttons for withdraw handling
	 *
	 * @param button_tmp Pointer to Button object
	 */
	void addButton(std::shared_ptr<Button> button_tmp);

	/**
	 * Add a text to vector of all texts for withdraw handling
	 *
	 * @param text_tmp Pointer to sf::Text object
	 */
	void addText(std::shared_ptr<sf::Text> text_tmp);

   public:
	/**
	 * @brief Constructor for the Withdraw class.
	 */
	Withdraw();

	/**
	 * @brief Getter for the properties of Player 1 offer.
	 *
	 * @return A vector of unsigned integers representing the indexes of Player 1 offer properties.
	 */
	std::vector<unsigned int> getPlayer1IndexProperties();

	/**
	 * @brief Getter for the properties of Player 2 offer.
	 *
	 * @return A vector of unsigned integers representing the indexes of Player 2 offer properties.
	 */
	std::vector<unsigned int> getPlayer2IndexProperties();

	/**
	 * @brief Setter for the properties of Player 1 offer.
	 *
	 * @param new_index_properties A vector of unsigned integers representing the new index for Player 1 offer
	 * properties.
	 */
	void setPlayer1IndexProperties(std::vector<unsigned int> new_index_properties);

	/**
	 * @brief Setter for the properties of Player 2 offer.
	 *
	 * @param new_index_properties A vector of unsigned integers representing the new index for Player 2 offer
	 * properties.
	 */
	void setPlayer2IndexProperties(std::vector<unsigned int> new_index_properties);

	/**
	 * @brief Setter for the gameboard.
	 *
	 * @param board_ptr A shared pointer to the Board object.
	 */
	void setBoard(std::shared_ptr<Board> board_ptr);

	/**
	 * @brief Setter for the turn state of monopolygame when withdraw was started.
	 *
	 * @param state A TurnState enumeration representing the turn state.
	 */
	void setTurnState(TurnState state);

	/**
	 * @brief Getter for the turn state.
	 *
	 * @return The current TurnState enumeration representing the turn state
	 * of monopolygame when withdraw was started.
	 */
	TurnState getTurnState();

	/**
	 * @brief Getter for the font used in rendering text.
	 *
	 * @return A reference to the SFML Font object.
	 */
	sf::Font& getFont();

	/**
	 * @brief Getter for the font size used in rendering text.
	 *
	 * @return An unsigned integer representing the font size.
	 */
	unsigned int getFontSize() const;

	/**
	 * @brief Setter for the font used in rendering text.
	 *
	 * @param font A reference to the SFML Font object.
	 */
	void setFont(sf::Font font);

	/**
	 * @brief Getter for the vector of buttons used in the withdrawal process.
	 *
	 * @return A vector of shared pointers to Button objects.
	 */
	std::vector<std::shared_ptr<Button>>& getButtons();

	/**
	 * @brief Getter for the vector of text objects used in the withdrawal process.
	 *
	 * @return A vector of shared pointers to SFML Text objects.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getTexts();

	/**
	 * @brief Getter for button for choosing Player 1 as passive in withdraw.
	 *
	 * @return A shared pointer to choose Player 1 Button object.
	 */
	std::shared_ptr<Button> getPlayer1Button();

	/**
	 * @brief Getter for button for choosing Player 2 as passive in withdraw.
	 *
	 * @return A shared pointer to choose Player 2 Button object.
	 */
	std::shared_ptr<Button> getPlayer2Button();

	/**
	 * @brief Getter for button for choosing Player 3 as passive in withdraw.
	 *
	 * @return A shared pointer to choose Player 3 Button object.
	 */
	std::shared_ptr<Button> getPlayer3Button();

	/**
	 * @brief Getter for button for choosing Player 4 as passive in withdraw.
	 *
	 * @return A shared pointer to choose Player 4 Button object.
	 */
	std::shared_ptr<Button> getPlayer4Button();

	/**
	 * @brief Getter for the resign button.
	 *
	 * @return A shared pointer to the resign Button object.
	 */
	std::shared_ptr<Button> getResignButton();

	/**
	 * @brief Creates the screen for choosing players during withdrawal.
	 */
	void createChoosePlayerScreen();

	/**
	 * @brief Creates the screen for specifying values during withdrawal.
	 */
	void createValuePlayerScreen();

	/**
	 * @brief Creates the screen for making decisions during withdrawal.
	 */
	void createDecisionPlayerScreen();

	/**
	 * @brief Sets the visibility of the choose player screen.
	 *
	 * @param is_visible A boolean indicating whether the screen should be visible.
	 */
	void setChooseScreenVisible(bool is_visible);

	/**
	 * @brief Sets the visibility of the value player screen.
	 *
	 * @param is_visible A boolean indicating whether the screen should be visible.
	 */
	void setValueScreenVisible(bool is_visible);

	/**
	 * @brief Sets the visibility of the decision player screen.
	 *
	 * @param is_visible A boolean indicating whether the screen should be visible.
	 */
	void setDecisionScreenVisible(bool is_visible);

	/**
	 * @brief Sets Active Player for withdrawal.
	 *
	 * @param player_ptr A shared pointer to the Player object.
	 */
	void setPlayer1ToWithdraw(std::shared_ptr<Player> player_ptr);

	/**
	 * @brief Sets Passive Player for withdrawal.
	 *
	 * @param player_ptr A shared pointer to the Player object.
	 */
	void setPlayer2ToWithdraw(std::shared_ptr<Player> player_ptr);

	/**
	 * @brief Getter for Active Player object.
	 *
	 * @return A shared pointer to the Player object.
	 */
	std::shared_ptr<Player> getPlayer1ToWithdraw();

	/**
	 * @brief Getter for Passive Player object.
	 *
	 * @return A shared pointer to the Player object.
	 */
	std::shared_ptr<Player> getPlayer2ToWithdraw();

	/**
	 * @brief Getter for the resign button from value screen.
	 *
	 * @return A shared pointer to the resign value Button object.
	 */
	std::shared_ptr<Button> getResignValueButton();

	/**
	 * @brief Getter for the submit value button from value screen.
	 *
	 * @return A shared pointer to the submit value Button object.
	 */
	std::shared_ptr<Button> getSubmitValueButton();

	/**
	 * @brief Getter for Active Player minus 1 money button.
	 *
	 * @return A shared pointer to the minus 1 Button object for Active Player.
	 */
	std::shared_ptr<Button> getPlayer1minus1();

	/**
	 * @brief Getter for Active Player minus 10 money button.
	 *
	 * @return A shared pointer to the minus 10 Button object for Active Player.
	 */
	std::shared_ptr<Button> getPlayer1minus10();

	/**
	 * @brief Getter for Active Player minus 100 money button.
	 *
	 * @return A shared pointer to the minus 100 Button object for Active Player.
	 */
	std::shared_ptr<Button> getPlayer1minus100();

	/**
	 * @brief Getter for Active Player plus 1 money button.
	 *
	 * @return A shared pointer to the minus 1 Button object for Active Player.
	 */
	std::shared_ptr<Button> getPlayer1plus1();

	/**
	 * @brief Getter for Active Player plus 10 money button.
	 *
	 * @return A shared pointer to the minus 10 Button object for Active Player.
	 */
	std::shared_ptr<Button> getPlayer1plus10();

	/**
	 * @brief Getter for Active Player plus 100 money button.
	 *
	 * @return A shared pointer to the minus 100 Button object for Active Player.
	 */
	std::shared_ptr<Button> getPlayer1plus100();

	/**
	 * @brief Getter for Passive Player minus 1 money button.
	 *
	 * @return A shared pointer to the minus 1 Button object for Passive Player.
	 */
	std::shared_ptr<Button> getPlayer2minus1();

	/**
	 * @brief Getter for Passive Player minus 10 money button.
	 *
	 * @return A shared pointer to the minus 10 Button object for Passive Player.
	 */
	std::shared_ptr<Button> getPlayer2minus10();

	/**
	 * @brief Getter for Passive Player minus 100 money button.
	 *
	 * @return A shared pointer to the minus 100 Button object for Passive Player.
	 */
	std::shared_ptr<Button> getPlayer2minus100();

	/**
	 * @brief Getter for Passive Player plus 1 money button.
	 *
	 * @return A shared pointer to the plus 1 Button object for Passive Player.
	 */
	std::shared_ptr<Button> getPlayer2plus1();

	/**
	 * @brief Getter for Passive Player plus 10 money button.
	 *
	 * @return A shared pointer to the plus 10 Button object for Passive Player.
	 */
	std::shared_ptr<Button> getPlayer2plus10();

	/**
	 * @brief Getter for Passive Player plus 100 money button.
	 *
	 * @return A shared pointer to the plus 100 Button object for Passive Player.
	 */
	std::shared_ptr<Button> getPlayer2plus100();

	/**
	 * @brief Getter for Player 1 next property button.
	 *
	 * @return A shared pointer to the next property Button object for Player 1.
	 */
	std::shared_ptr<Button> getPlayer1NextButton();

	/**
	 * @brief Getter for Player 1 previous property button.
	 *
	 * @return A shared pointer to the previous property Button object for Player 1.
	 */
	std::shared_ptr<Button> getPlayer1PreviousButton();

	/**
	 * @brief Getter for Player 2 next property button.
	 *
	 * @return A shared pointer to the next property Button object for Player 2.
	 */
	std::shared_ptr<Button> getPlayer2NextButton();

	/**
	 * @brief Getter for Player 2 previous property button.
	 *
	 * @return A shared pointer to the previous property Button object for Player 2 .
	 */
	std::shared_ptr<Button> getPlayer2PreviousButton();

	/**
	 * @brief Getter for Player 1 offer next property button.
	 *
	 * @return A shared pointer to the next property Button object for Player 1 offer.
	 */
	std::shared_ptr<Button> getPlayer1IndexNextButton();

	/**
	 * @brief Getter for Player 1 index previous property button.
	 *
	 * @return A shared pointer to the previous property Button object for Player 1 offer.
	 */
	std::shared_ptr<Button> getPlayer1IndexPreviousButton();

	/**
	 * @brief Getter for Player 2 index next property button.
	 *
	 * @return A shared pointer to the next property Button object for Player 2 offer.
	 */
	std::shared_ptr<Button> getPlayer2IndexNextButton();

	/**
	 * @brief Getter for Player 2 index previous property button.
	 *
	 * @return A shared pointer to the previous property Button object for Player 2 offer.
	 */
	std::shared_ptr<Button> getPlayer2IndexPreviousButton();

	/**
	 * @brief Getter for Player 1 add to offer property button.
	 *
	 * @return A shared pointer to the add property Button object for Player 1 offer.
	 */
	std::shared_ptr<Button> getPlayer1AddButton();

	/**
	 * @brief Getter for Player 1 remove from offer property button.
	 *
	 * @return A shared pointer to the remove property Button object from Player 1 offer.
	 */
	std::shared_ptr<Button> getPlayer1RemoveButton();

	/**
	 * @brief Getter for Player 2 add to offer property button.
	 *
	 * @return A shared pointer to the add property Button object for Player 2 offer.
	 */
	std::shared_ptr<Button> getPlayer2AddButton();

	/**
	 * @brief Getter for Player 2 remove from offer property button.
	 *
	 * @return A shared pointer to the remove property Button object from Player 2 offer.
	 */
	std::shared_ptr<Button> getPlayer2RemoveButton();

	/**
	 * @brief Getter for the resign decision button.
	 *
	 * @return A shared pointer to the resign decision Button object.
	 */
	std::shared_ptr<Button> getResignDecisionButton();

	/**
	 * @brief Getter for the accept decision button.
	 *
	 * @return A shared pointer to the accept decision Button object.
	 */
	std::shared_ptr<Button> getAcceptDecisionButton();

	/**
	 * @brief Initiates the withdrawal process (exchange of money and properties).
	 */
	void makeWithdraw();

	/**
	 * @brief Transfers money during the withdrawal process.
	 *
	 * @param player_num An unsigned integer representing the player number (1-2).
	 * @param money An integer representing the amount of money to transfer (can be negative).
	 */
	void moneyTransferIndex(unsigned int player_num, int money);

	/**
	 * @brief Updates the displayed money text during the withdrawal process.
	 */
	void moneyTextUpdate();

	/**
	 * @brief Shows the property details for the specified column during the withdrawal process.
	 *
	 * @param column An integer representing the column index (1-4).
	 */
	void showProperty(int column);

	/**
	 * @brief Getter for Player 1's property sprite.
	 *
	 * @return A reference to the SFML Sprite object representing Player 1's properties.
	 */
	sf::Sprite& getSpritePropertyPlayer1();

	/**
	 * @brief Getter for the text objects representing Player 1's property details.
	 *
	 * @return A vector of shared pointers to SFML Text objects.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer1();

	/**
	 * @brief Getter for Player 1's offered property sprite.
	 *
	 * @return A reference to the SFML Sprite object representing Player 1's offered properties.
	 */
	sf::Sprite& getSpritePropertyPlayer1Index();

	/**
	 * @brief Getter for the text objects representing Player 1's offered property details.
	 *
	 * @return A vector of shared pointers to SFML Text objects.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer1Index();

	/**
	 * @brief Getter for Player 2's property sprite.
	 *
	 * @return A reference to the SFML Sprite object representing Player 2's properties.
	 */
	sf::Sprite& getSpritePropertyPlayer2();

	/**
	 * @brief Getter for the text objects representing Player 2's property details.
	 *
	 * @return A vector of shared pointers to SFML Text objects.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer2();

	/**
	 * @brief Getter for Player 2's offered property sprite.
	 *
	 * @return A reference to the SFML Sprite object representing Player 2's offered properties.
	 */
	sf::Sprite& getSpritePropertyPlayer2Index();

	/**
	 * @brief Getter for the text objects representing Player 2's offered property details.
	 *
	 * @return A vector of shared pointers to SFML Text objects.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getTextsPropertyPlayer2Index();

	/**
	 * @brief Getter for Player 1's property texture.
	 *
	 * @return SFML Texrute object representing Player 1's properties.
	 */
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer1();

	/**
	 * @brief Getter for Player 1's offer property texture.
	 *
	 * @return SFML Texrute object representing Player 1's properties offer.
	 */
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer1Index();

	/**
	 * @brief Getter for Player 2's property texture.
	 *
	 * @return SFML Texrute object representing Player 2's properties.
	 */
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer2();

	/**
	 * @brief Getter for Player 2's offer property texture.
	 *
	 * @return SFML Texrute object representing Player 2's properties offer.
	 */
	std::shared_ptr<sf::Texture> getTexturePropertyPlayer2Index();

	/**
	 * @brief Changes showed property in certain column
	 *
	 *  @param i if i > 0 moves righ in properties vector, if i < 0 moves left
	 *  @param col (1-4) defines properties column
	 */
	void addPropertyPlayerShowed(int i, unsigned int col);

	/**
	 * @brief Moves properties between players ownership and withdraw offer
	 *
	 *  @param dir if > 0 moves to offer vector, if < 0 moves from offer vectpr
	 *  @param plr_num if 1 - active player, if 2 - passive player, in withdraw
	 */
	void propertyPlayerMoveIndex(int dir, unsigned int plr_num);

	/**
	 * @brief Check whenever withdraw is legal to be done
	 *
	 * @return true if withdraw is legal, otherwise false
	 */
	bool isNonZeroValue();
};

#endif
