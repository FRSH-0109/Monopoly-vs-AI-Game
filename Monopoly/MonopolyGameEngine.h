/**
 * @file MonopolyGameEngine.h
 *
 * @brief Header file of class used to handle whole
 * monopoly game process, turns, actions with players,
 * board etc.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef MONOPOLY_GAME_ENGINE_H
#define MONOPOLY_GAME_ENGINE_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <random>
#include <vector>
#include "ActiveScreen.h"
#include "Board.h"
#include "Chance.h"
#include "NotificationWall.h"
#include "Withdraw.h"
#include "main.h"

/**
 * @brief Enum representing the state of an auction.
 */
enum AuctionState { NO_AUCTION, INITIALIZATION, PASS_BIDDING_TURN, BIDDING, ENDING };

/**
 * @class MonopolyGameEngine
 *
 * @brief Class representing the main game engine for the Monopoly game.
 *
 * The MonopolyGameEngine class handles the overall game flow, including player turns, actions, and interactions with the game board. It also manages the graphical user interface (GUI) components.
 */
class MonopolyGameEngine {
	TurnState turn_state_; 								/**< Current turn state of the game. */
	const unsigned int PLAYERS_MAX_ = 4;				/**< Maximum number of players in the game. */
	const unsigned int PLAYERS_MIN_ = 2; 				/**< Minimum number of players in the game. */
	unsigned int players_starting_amount_ = 0;			/**< Starting amount of money for players. */
	std::array<unsigned int, 4> players_starting_Ids_;	/**< Starting IDs for players. */
	unsigned int house_count_ = 32; 					/**< Total count of houses in the game. */
	unsigned int hotel_count_ = 12; 					/**< Total count of hotels in the game. */
	unsigned int player_index_turn_; 						/**< Index of the current player's turn. */
	bool is_ai_game_only_; 								/**< Flag indicating whether the game consists only of AI players. */
	GameScreenType screen_type_ = BOARDGAME; 			/**< Type of the current game screen. */
	const unsigned int PLAYER_MONEY_DEFAULT_ = 1500; 	/**< Default starting money for players. */
	const unsigned int START_PASSING_MONEY_ = 200; 		/**< Money passed to players when passing the start position. */
	const unsigned int GAME_TURNS_MAX = 50; 			/**< Maximum number of turns in the game. */
	unsigned int game_turns_globally_done_;				/**< Number of turns globally performed in the game. */
	std::array<bool, 4> game_turn_by_player_done_; 			/**< Number of turns performed by each player. */
	AuctionState auction_state_ = NO_AUCTION; 			/**< Current state of the auction. */

	const std::string CHANCE_FILE_PATH_ = "Monopoly/game_config_json/chance.json";	 /**< File path for chance cards configuration. */
	const std::string GAMEBOARD_FILE_PATH_ = "Monopoly/game_config_json/board.json"; /**< File path for game board configuration. */
	std::shared_ptr<Board> gameboard_;												 /**< Pointer to the game board. */
	std::vector<std::shared_ptr<Player>> players_; 									 /**< Vector of player objects. */
	std::vector<std::shared_ptr<Player>> players_bankrupted_; 						 /**< Vector of bankrupted players. */

	// GUI
	const unsigned int FONT_SIZE = 30; 										/**< Font size for text in GUI. */
	sf::Font font_; 														/**< Font used in the GUI. */
	std::vector<std::shared_ptr<Button>> buttons_; 							/**< Vector of buttons in the GUI. */
	std::vector<std::shared_ptr<sf::Text>> texts_;							/**< Vector of text elements in the GUI. */
	std::vector<std::shared_ptr<Button>> auction_buttons_;					/**< Vector of auction buttons in the GUI. */
	std::vector<std::shared_ptr<sf::Text>> auction_texts_; 					/**< Vector of auction text elements in the GUI. */
	sf::Vector2f ROLL_DICE_BUTTON_POSITION = sf::Vector2f(965, 120);		/**< Position of the roll dice button. */
	std::shared_ptr<Button> roll_dice_button_; 								/**< Pointer to the roll dice button. */
	sf::Vector2f TURN_INFO_TEXT_POSITION = sf::Vector2f(900, 50); 			/**< Position of the turn info text. */
	std::shared_ptr<sf::Text> turn_info_text_; 								/**< Pointer to the turn info text. */
	sf::Vector2f ROLLED_VALUE_TEXT_POSITION = sf::Vector2f(900, 100); 		/**< Position of the rolled value text. */
	std::shared_ptr<sf::Text> rolled_value_text_; 							/**< Pointer to the rolled value text. */
	sf::Vector2f PLAYERS_INFO_TEXT_POSITION = sf::Vector2f(850, 800); 		/**< Position of the players info text. */
	std::array<std::vector<std::shared_ptr<sf::Text>>, 4> player_info_text_; /**< Array of vectors containing player info text elements. */

	sf::Vector2f BUY_BUTTON_POSITION = sf::Vector2f(965, 220);				/**< Position of the buy button. */
	std::shared_ptr<Button> buy_field_button_; 								/**< Pointer to the buy field button. */
	std::shared_ptr<Button> resign_buy_field_button_; 							/**< Pointer to the resign buy field button. */

	sf::Vector2f PROPERTY_DATA_POSITION = sf::Vector2f(910, 260); 			/**< Position of the property data. */
	sf::Vector2f PROPERTY_DATA_AUCTION_POSITION = sf::Vector2f(200, 300);	/**< Position of the property data during an auction. */
	float PROPERTY_DATA_SCALE = 4; 											/**< Scale factor for property data. */
	sf::Sprite property_data_sprite_; 										/**< Sprite for property data. */
	sf::Texture property_data_texture_; 										/**< Texture for property data. */
	std::vector<std::shared_ptr<sf::Text>> property_data_texts_; 				/**< Vector of property data text elements. */

	sf::Vector2f ALL_PROPERTY_DATA_POSITION = sf::Vector2f(1400, 260); 		/**< Position of all property data. */
	sf::Sprite all_property_data_sprite_;										/**< Sprite for all property data. */
	sf::Texture all_property_data_texture_; 									/**< Texture for all property data. */
	std::vector<std::shared_ptr<sf::Text>> all_property_data_texts_;			/**< Vector of all property data text elements. */
	sf::Vector2f NEXT_PROPERTY_BUTTON_POSITION = sf::Vector2f(1595, 700); 	/**< Position of the next property button. */
	sf::Vector2f PREVIOUS_PROPERTY_BUTTON_POSITION = sf::Vector2f(1445, 700); /**< Position of the previous property button. */
	std::shared_ptr<Button> next_property_button_; 							/**< Pointer to the next property button. */
	std::shared_ptr<Button> previous_property_button_; 						/**< Pointer to the previous property button. */
	unsigned int current_property_showed_ = 1; 								/**< Index of the currently displayed property. */

	std::shared_ptr<sf::Text> available_houses_text_; 						/**< Pointer to the text displaying available houses. */
	std::shared_ptr<sf::Text> availableH_hotels_text_; 						/**< Pointer to the text displaying available hotels. */
	sf::Vector2f AVAILABLE_HOUSE_TEXT_POSITION = sf::Vector2f(1675, 110); 	/**< Position of the available houses text. */

	std::shared_ptr<sf::Text> bidded_property_text_; 							/**< Pointer to the text displaying the bidded property. */
	sf::Vector2f BIDDED_PROPERTY_TEXT_POSITION = sf::Vector2f(200, 250); 	/**< Position of the bidded property text. */

	std::shared_ptr<sf::Text> bidder_info_text_; 								/**< Pointer to the text displaying bidder information. */
	sf::Vector2f BIDDER_INFO_TEXT_POSITION = sf::Vector2f(1360, 105); 		/**< Position of the bidder info text. */

	std::shared_ptr<sf::Text> highest_bid_info_text_; 							/**< Pointer to the text displaying the highest bid information. */
	sf::Vector2f HIGHEST_BID_TEXT_POSITION = sf::Vector2f(820, 100);		/**< Position of the highest bid text. */

	std::shared_ptr<sf::Text> leading_bidder_info_text_; 						/**< Pointer to the text displaying the leading bidder information. */
	sf::Vector2f LEADING_BIDDER_TEXT_POSITION = sf::Vector2f(820, 145); 	/**< Position of the leading bidder text. */

	std::shared_ptr<sf::Text> current_offer_info_text_; 						/**< Pointer to the text displaying the current offer information. */
	sf::Vector2f CURRENT_OFFER_TEXT_POSITION = sf::Vector2f(820, 680); 		/**< Position of the current offer text. */

	std::shared_ptr<sf::Text> house_text_; 									/**< Pointer to the text displaying the number of houses. */
	std::shared_ptr<sf::Text> hotel_text_; 									/**< Pointer to the text displaying the number of hotels. */
	sf::Vector2f HOUSE_TEXT_POSITION = sf::Vector2f(1445, 80); 				/**< Position of the house text. */
	sf::Vector2f HOTEL_TEXT_POSITION = sf::Vector2f(1595, 80); 				/**< Position of the hotel text. */
	std::shared_ptr<Button> buy_house_button_; 								/**< Pointer to the buy house button. */
	std::shared_ptr<Button> sell_house_button_; 								/**< Pointer to the sell house button. */
	std::shared_ptr<Button> buy_hotel_button_; 								/**< Pointer to the buy hotel button. */
	std::shared_ptr<Button> sell_hotel_button_; 								/**< Pointer to the sell hotel button. */
	sf::Vector2f BUY_HOUSE_BUTTON_POSITION = sf::Vector2f(1445, 120); 		/**< Position of the buy house button. */
	sf::Vector2f SELL_HOUSE_BUTTON_POSITION = sf::Vector2f(1445, 180); 		/**< Position of the sell house button. */
	sf::Vector2f BUY_HOTEL_BUTTON_POSITION = sf::Vector2f(1595, 120); 		/**< Position of the buy hotel button. */
	sf::Vector2f SELL_HOTEL_BUTTON_POSITION = sf::Vector2f(1595, 180); 		/**< Position of the sell hotel button. */

	std::shared_ptr<Button> bankrupt_button_; 								/**< Pointer to the bankrupt button. */
	std::shared_ptr<Button> next_turn_button_; 								/**< Pointer to the next turn button. */
	sf::Vector2f BANKRUPT_BUTTON_POSITION = sf::Vector2f(1270, 50); 		/**< Position of the bankrupt button. */
	sf::Vector2f NEXT_TURN_BUTTON_POSITION = sf::Vector2f(1270, 180); 		/**< Position of the next turn button. */

	// houses and hotels
	sf::Vector2f house_size_ = sf::Vector2f(22, 13); 						/**< Size of a house. */
	sf::Texture house_texture_;						 						/**< Texture for a house. */
	sf::Texture hotel_texture_; 												/**< Texture for a hotel. */

	// pay jail button
	std::shared_ptr<Button> jail_pay_button_; 								/**< Pointer to the pay jail button. */
	sf::Vector2f JAIL_PAY_BUTTON_POSITION = sf::Vector2f(1000, 250);		/**< Position of the pay jail button. */
	const unsigned int JAIL_PAY_MONEY = 50; 								/**< Amount of money required to pay jail. */

	// Auction change offer buttons
	std::shared_ptr<sf::Text> Text1_; 										/**< Pointer to the text displaying the value 1 in auction offer. */
	std::shared_ptr<sf::Text> Text10_; 										/**< Pointer to the text displaying the value 10 in auction offer. */
	std::shared_ptr<sf::Text> Text100_; 									/**< Pointer to the text displaying the value 100 in auction offer. */
	sf::Vector2f TEXT_1_POSITION = sf::Vector2f(975, 880);					/**< Position of the text displaying the value 1. */
	sf::Vector2f TEXT_10_POSITION = sf::Vector2f(975, 820); 				/**< Position of the text displaying the value 10. */
	sf::Vector2f TEXT_100_POSITION = sf::Vector2f(975, 760); 				/**< Position of the text displaying the value 100. */

	std::shared_ptr<Button> add_1_to_offer_button_; 							/**< Pointer to the add 1 to offer button in auction. */
	std::shared_ptr<Button> add_10_to_offer_button_; 							/**< Pointer to the add 10 to offer button in auction. */
	std::shared_ptr<Button> add_100_to_offer_button_; 							/**< Pointer to the add 100 to offer button in auction. */
	std::shared_ptr<Button> substract_1_from_offer_button_; 					/**< Pointer to the subtract 1 from offer button in auction. */
	std::shared_ptr<Button> substract_10_from_offer_button_; 					/**< Pointer to the subtract 10 from offer button in auction. */
	std::shared_ptr<Button> substract_100_from_offer_button_; 					/**< Pointer to the subtract 100 from offer button in auction. */
	sf::Vector2f ADD_1_BUTTON_POSITION = sf::Vector2f(900, 880); 			/**< Position of the add 1 button. */
	sf::Vector2f ADD_10_BUTTON_POSITION = sf::Vector2f(900, 820); 			/**< Position of the add 10 button. */
	sf::Vector2f ADD_100_BUTTON_POSITION = sf::Vector2f(900, 760); 			/**< Position of the add 100 button. */
	sf::Vector2f SUBSTRACT_1_BUTTON_POSITION = sf::Vector2f(1050, 880); 	/**< Position of the subtract 1 button. */
	sf::Vector2f SUBSTRACT_10_BUTTON_POSITION = sf::Vector2f(1050, 820); 	/**< Position of the subtract 10 button. */
	sf::Vector2f SUBSTRACT_100_BUTTON_POSITION = sf::Vector2f(1050, 760); 	/**< Position of the subtract 100 button. */

	std::shared_ptr<Button> auction_bid_button_; 								/**< Pointer to the auction bid button. */
	sf::Vector2f AUCTION_BID_BUTTON_POSITION = sf::Vector2f(1200, 880); 	/**< Position of the auction bid button. */

	// Auction resign button
	std::shared_ptr<Button> auction_resign_button_; 							/**< Pointer to the auction resign button. */
	sf::Vector2f AUCTION_RESIGN_BUTTON_POSITION = sf::Vector2f(1330, 880);  /**< Position of the auction resign button. */

	// withdraw
	Withdraw withdraw_; 													/**< Object representing player withdrawal. */
	std::shared_ptr<Button> withdraw_button_; 								/**< Pointer to the withdraw button. */
	sf::Vector2f WITHDRAW_BUTTON_POSITION = sf::Vector2f(400, 900); 		/**< Position of the withdraw button. */

	// Mortgaging mechanic buttons
	std::shared_ptr<Button> mortgage_button_; 								/**< Pointer to the mortgage button. */
	std::shared_ptr<Button> unmortgage_button_; 								/**< Pointer to the unmortgage button. */
	sf::Vector2f MORTGAGE_BUTTON_POSITION = sf::Vector2f(1445, 760); 		/**< Position of the mortgage button. */
	sf::Vector2f UNMORTGAGE_BUTTON_POSITION = sf::Vector2f(1595, 760); 		/**< Position of the unmortgage button. */

	// chance cards
	std::vector<ChanceCard> chance_cards_;									/**< Vector of chance cards. */
	unsigned int chance_card_current_ = 0; 									/**< Index of the current chance card. */

	NotificationWall notifications_wall_; 									/**< Notification wall for displaying messages. */

	// result screen
	float RESULT_DATA_Y = 200; 												/**< Y-coordinate for positioning result screen data. */
	std::vector<std::shared_ptr<sf::Text>> result_players_places_; 			/**< Vector of result screen player places. */
	std::shared_ptr<Button> return_to_main_menu_button_; 						/**< Pointer to the return to main menu button. */

	// File logger
	std::ofstream file_logger;
	const std::string FILE_LOGGER_PATH = "game_logs.txt";

	/**
	 * @brief Gets the current player index of the turn.
	 *
	 * @return The current player index.
	 */
	unsigned int getplayer_index_turn() const;

	/**
	 * @brief Increases the player index for the next turn.
	 */
	void incplayer_index_turn();

	/**
	 * @brief Sets the turn state to the specified state.
	 *
	 * @param new_state The new turn state.
	 */
	void setTurnState(TurnState new_state);

	/**
	 * @brief Handles various button-related actions.
	 */
	void buttonsWorker();

	/**
	 * @brief Gets the updated position for the player sprite.
	 *
	 * @return The updated position.
	 */
	sf::Vector2f getUpdatePlayerSpritePosition();

	/**
	 * @brief Clears property data based on whether it is shown for purchase or not.
	 *
	 * @param is_property_shown_to_buy Flag indicating whether the property is shown for purchase.
	 */
	void clearPropertyData(bool is_property_shown_to_buy);

	/**
	 * @brief Gets the field price based on its position.
	 *
	 * @param pos The position of the field.
	 * @return The price of the field.
	 */
	unsigned int getFieldPriceByPosition(unsigned int pos);

	/**
	 * @brief Adds the owner to a property field.
	 *
	 * @param player The player becoming the owner.
	 * @param pos The position of the property field.
	 */
	void addOwnerToPropertyField(std::shared_ptr<Player> player, unsigned int pos);

	/**
	 * @brief Checks if a button is clicked.
	 *
	 * @param button_ptr The pointer to the button.
	 * @return True if the button is clicked, false otherwise.
	 */
	bool isButtonClicked(std::shared_ptr<Button> button_ptr);

	/**
	 * @brief Rolls the dice and returns the result.
	 *
	 * @return The rolled dice value.
	 */
	unsigned int rollDice() const;

	/**
	 * @brief Moves the player based on the turn index and position increment.
	 *
	 * @param turn_index The index of the player's turn.
	 * @param position_increment The increment in the player's position.
	 */
	void movePlayer(unsigned int turn_index, unsigned int position_increment);

	/**
	 * @brief Sends the player to jail based on the turn index.
	 *
	 * @param turn_index The index of the player's turn.
	 */
	void sendToJail(unsigned int turn_index);

	/**
	 * @brief Adds a notification based on the index and text.
	 *
	 * @param index The index of the notification.
	 * @param text The text of the notification.
	 */
	void notificationAdd(unsigned int index, std::string text);

	/**
	 * @brief Handles passing the Start position, updating old and new positions.
	 *
	 * @param old_pos The old position of the player.
	 * @param new_pos The new position of the player.
	 */
	void handlePassingStart(unsigned int old_pos, unsigned int new_pos);

	/**
	 * @brief Makes the player bankrupt based on the player's index.
	 *
	 * @param player_index_turn The index of the player.
	 */
	void makePlayerBankrupt(unsigned int player_index_turn);

	/**
	 * @brief Shows all properties for purchase.
	 */
	void showAllPropertiesWorker();

	/**
	 * @brief Switches the game window to auction mode based on the specified flag.
	 *
	 * @param is_auction Flag indicating whether to switch to auction mode.
	 */
	void boardToAuctionSwitchHandler(bool is_auction);

	/**
	 * @brief Handles withdrawal actions.
	 */
	void withdrawWorker();

	/**
	 * @brief Handles AI withdrawal actions.
	 */
	void aiWithdrawWorker();

	/**
	 * @brief Gets the chance card reference.
	 *
	 * @return The chance card reference.
	 */
	ChanceCard& getChanceCard();

	/**
	 * @brief Updates the chance card.
	 */
	void updateChanceCard();

	/**
	 * @brief Checks if the game has finished and determined a winner.
	 *
	 * @return True if there is a winner, false otherwise.
	 */
	bool gameFinishedCheckWinner();

	/**
	 * @brief Checks if the game has finished in a draw.
	 *
	 * @return True if the game is a draw, false otherwise.
	 */
	bool gameFinishedCheckDraw();

	/**
	 * @brief Handles the counter for game turns.
	 */
	void gameTurnsCounterHandle();

	/**
	 * @brief Removes the player from the game based on the player's index and draw status.
	 *
	 * @param player_index_turn The index of the player.
	 * @param is_draw Flag indicating whether the game ended in a draw.
	 */
	void removePlayerFromGame(unsigned int player_index_turn, bool is_draw);

	/**
	 * @brief Creates a default button with the specified text, width, and height.
	 *
	 * @param text The text of the button.
	 * @param width The width of the button.
	 * @param height The height of the button.
	 * @return A shared pointer to the created button.
	 */
	std::shared_ptr<Button> createDefaultButton(sf::String text, unsigned int width, unsigned int height);
	
	/**
	 * @brief Opens the file logger.
	 */
	void fileLoggerOpen();

	/**
	 * @brief Writes text to the file logger.
	 *
	 * @param text The text to write.
	 */
	void fileLoggerWrite(std::string text);

	/**
	 * @brief Closes the file logger.
	 */
	void fileLoggerClose();

	public:
	/**
	 * @brief Constructor for the `monopolyGameEngine` class.
	 */
	MonopolyGameEngine();

	/**
	 * @brief Sets the screen type to the specified type.
	 *
	 * @param new_screen_type The new screen type.
	 */
	void setScreenType(GameScreenType new_screen_type);

	/**
	 * @brief Gets the current screen type.
	 *
	 * @return The current screen type.
	 */
	GameScreenType getScreenType() const;

	// GUI methods
	/**
	 * @brief Creates the button for rolling the dice.
	 */
	void createButtonRollDice();

	/**
	 * @brief Creates text for displaying turn information.
	 */
	void createTextTurnInfo();

	/**
	 * @brief Creates text for displaying the rolled dice value.
	 */
	void createTextrolled_value();

	/**
	 * @brief Creates text for displaying players' information.
	 */
	void createTextPlayersInfo();

	/**
	 * @brief Updates the text displaying players' information.
	 */
	void updateTextPlayersInfo();

	/**
	 * @brief Creates text for displaying information about the bidded property.
	 */
	void createTextBiddedProperty();

	/**
	 * @brief Creates text for displaying information about the bidder.
	 */
	void createTextBidderInfo();

	/**
	 * @brief Creates text for displaying the highest bid in an auction.
	 */
	void createTextHighestBidInfo();

	/**
	 * @brief Creates text for displaying information about the leading bidder in an auction.
	 */
	void createTextLeadingBidderInfo();

	/**
	 * @brief Creates text for displaying the current offer in an auction.
	 */
	void createCurrentOfferBidderInfo();

	/**
	 * @brief Creates a button for buying or resigning from a property.
	 */
	void createButtonBuyResign();

	/**
	 * @brief Creates a button for moving to the next property.
	 */
	void createButtonNextProperty();

	/**
	 * @brief Creates a button for moving to the previous property.
	 */
	void createButtonPerviousProperty();

	/**
	 * @brief Creates buttons for buying, selling houses, and hotels.
	 */
	void createButtonsBuySellHouseHotel();

	/**
	 * @brief Creates buttons for handling bankruptcy.
	 */
	void createButtonsBankrupt();

	/**
	 * @brief Creates buttons for moving to the next turn.
	 */
	void createButtonsNextTurn();

	/**
	 * @brief Creates buttons for handling actions related to the jail (paying to get out).
	 */
	void createButtonsJailPay();

	/**
	 * @brief Creates buttons for handling auction offers.
	 */
	void createAuctionOfferButtons();

	/**
	 * @brief Creates a button for participating in an auction by bidding.
	 */
	void createAuctionBidButton();

	/**
	 * @brief Creates a button for resigning from an auction.
	 */
	void createAuctionResignButton();

	/**
	 * @brief Creates a button for withdrawing from the game.
	 */
	void createButtonWithdraw();

	/**
	 * @brief Creates buttons for mortgaging properties.
	 */
	void createMortagingButton();

	/**
	 * @brief Creates various elements for the result screen.
	 */
	void createResultScreenStuff();

	/**
	 * @brief Updates elements on the result screen.
	 */
	void updateResultScreenStuff();

	/**
	 * @brief Shows property data on the GUI based on the position and whether it is shown to buy.
	 * @param pos The position of the property.
	 * @param is_property_shown_to_buy Indicates whether the property information is shown for buying.
	 */
	void showPropertyData(unsigned int pos, bool is_property_shown_to_buy);

	/**
	 * @brief Displays text related to turn information.
	 */
	void turnInfoTextShow();

	/**
	 * @brief Returns the font used in the GUI.
	 * @return Reference to the font.
	 */
	sf::Font& getFont();

	/**
	 * @brief Returns the font size used in the GUI.
	 * @return Font size.
	 */
	unsigned int getFontSize() const;

	/**
	 * @brief Sets the font to be used in the GUI.
	 * @param font Font to be set.
	 */
	void setFont(sf::Font font);

	/**
	 * @brief Adds a button to the GUI.
	 * @param button_tmp Pointer to the button to be added.
	 */
	void addButton(std::shared_ptr<Button> button_tmp);

	/**
	 * @brief Adds text to the GUI.
	 * @param text_tmp Pointer to the text to be added.
	 */
	void addText(std::shared_ptr<sf::Text> text_tmp);

	/**
	 * @brief Adds an auction button to the GUI.
	 * @param button_tmp Pointer to the auction button to be added.
	 */
	void addAuctionButton(std::shared_ptr<Button> button_tmp);

	/**
	 * @brief Adds auction text to the GUI.
	 * @param text_tmp Pointer to the auction text to be added.
	 */
	void addAuctionText(std::shared_ptr<sf::Text> text_tmp);

	/**
	 * @brief Returns a vector of pointers to buttons in the GUI.
	 * @return Vector of pointers to buttons.
	 */
	std::vector<std::shared_ptr<Button>>& getButtons();

	/**
	 * @brief Returns a vector of pointers to text elements in the GUI.
	 * @return Vector of pointers to text elements.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getTexts();

	/**
	 * @brief Returns a vector of pointers to auction buttons in the GUI.
	 * @return Vector of pointers to auction buttons.
	 */
	std::vector<std::shared_ptr<Button>>& getAuctionButtons();

	/**
	 * @brief Returns a vector of pointers to auction text elements in the GUI.
	 * @return Vector of pointers to auction text elements.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getAuctionTexts();

	/**
	 * @brief Returns a vector of pointers to text elements used in the result screen.
	 * @return Vector of pointers to result screen text elements.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getResultTexts();

	/**
	 * @brief Returns the sprite representing property data in the GUI.
	 * @return Reference to the property data sprite.
	 */
	sf::Sprite& getPropertyDataSprite();

	/**
	 * @brief Returns a vector of pointers to text elements representing property data in the GUI.
	 * @return Vector of pointers to property data text elements.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getPropertyDataTexts();

	/**
	 * @brief Returns the sprite representing all property data in the GUI.
	 * @return Reference to the all property data sprite.
	 */
	sf::Sprite& getAllPropertyDataSprite();

	/**
	 * @brief Returns a vector of pointers to text elements representing all property data in the GUI.
	 * @return Vector of pointers to all property data text elements.
	 */
	std::vector<std::shared_ptr<sf::Text>>& getAllPropertyDataTexts();

	/**
	 * @brief Returns the notification wall used in the GUI.
	 * @return Reference to the notification wall.
	 */
	NotificationWall& getNotificationsWall();

	/**
	 * @brief Gets the text suitable for drawing a property name.
	 * @param text Original text to be drawn.
	 * @param sprite Reference to the sprite associated with the property.
	 * @param rotation Rotation angle of the text.
	 * @return Modified text for drawing.
	 */
	sf::Text getPropertyNameToDraw(sf::Text text, sf::Sprite& sprite, float rotation);

	/**
	 * @brief Returns the texture of the house used in the GUI.
	 * @return Reference to the house texture.
	 */
	sf::Texture& getHouseTexture();

	/**
	 * @brief Returns the texture of the hotel used in the GUI.
	 * @return Reference to the hotel texture.
	 */
	sf::Texture& getHotelTexture();

	/**
	 * @brief Returns the size of a house in the GUI.
	 * @return Reference to the house size.
	 */
	sf::Vector2f& getHouseSize();

	// Game staff methods
	/**
	 * @brief Creates player objects based on input vector.
	 * @param players_from_game_engine Vector of pointers to players.
	 */
	void createPlayers(std::vector<std::shared_ptr<Player>>& players_from_game_engine);

	/**
	 * @brief Clears player objects from the game.
	 */
	void clearPlayers();

	/**
	 * @brief Creates the game board.
	 */
	void createBoard();

	/**
	 * @brief Clears the game board.
	 */
	void clearBoard();

	/**
	 * @brief Returns a pointer to the game board.
	 * @return Pointer to the game board.
	 */
	std::shared_ptr<Board> getBoard();

	/**
	 * @brief Returns a vector of pointers to players in the game.
	 * @return Vector of pointers to players.
	 */
	std::vector<std::shared_ptr<Player>>& getPlayers();

	/**
	 * @brief Returns a vector of pointers to players based on game result.
	 * @return Vector of pointers to players.
	 */
	std::vector<std::shared_ptr<Player>> getPlayersResult();

	/**
	 * @brief Sets the index of the player whose turn it is.
	 * @param indx Index of the player.
	 */
	void setplayer_index_turn(unsigned int indx);

	/**
	 * @brief Gets the current turn state.
	 * @return Current turn state.
	 */
	TurnState getTurnState() const;

	/**
	 * @brief Sets the state of the auction.
	 * @param new_state New state of the auction.
	 */
	void setAuctionState(AuctionState new_state);

	/**
	 * @brief Gets the current state of the auction.
	 * @return Current state of the auction.
	 */
	AuctionState getAuctionState();

	/**
	 * @brief Gets the count of houses available for purchase.
	 * @return Count of houses.
	 */
	unsigned int getHouseCount();

	/**
	 * @brief Gets the count of hotels available for purchase.
	 * @return Count of hotels.
	 */
	unsigned int getHotelCount();

	/**
	 * @brief Sets the count of houses available for purchase.
	 * @param new_count New count of houses.
	 */
	void setHouseCount(unsigned int new_count);

	/**
	 * @brief Sets the count of hotels available for purchase.
	 * @param new_count New count of hotels.
	 */
	void setHotelCount(unsigned int new_count);

	/**
	 * @brief Adds houses to the available count.
	 * @param added_amount Amount to be added.
	 */
	void addHouses(unsigned int added_amount);

	/**
	 * @brief Subtracts houses from the available count.
	 * @param substracted_amount Amount to be subtracted.
	 */
	void substractHouses(unsigned int substracted_amount);

	/**
	 * @brief Adds hotels to the available count.
	 * @param added_amount Amount to be added.
	 */
	void addHotels(unsigned int added_amount);

	/**
	 * @brief Subtracts hotels from the available count.
	 * @param substracted_amount Amount to be subtracted.
	 */
	void substractHotels(unsigned int substracted_amount);

	/**
	 * @brief Performs the auction.
	 */
	void performAuction();

	/**
	 * @brief Calculates the number of group fields owned by a player.
	 * @param player_fields Vector of field positions owned by the player.
	 * @param field Property field to check for group completion.
	 * @return Number of group fields owned.
	 */
	unsigned int calculateGroupFieldsOwned(std::vector<unsigned int> player_fields, PropertyField& field) const;

	/**
	 * @brief Checks if a group of properties is completed by a player.
	 * @param player_fields Vector of field positions owned by the player.
	 * @param field Property field to check for group completion.
	 * @return True if the group is completed, false otherwise.
	 */
	bool groupCompleted(std::vector<unsigned int> player_fields, PropertyField& field) const;

	/**
	 * @brief Checks if building houses on a street is legal for a player.
	 * @param builder Player attempting to build houses.
	 * @param field Street field to build houses on.
	 * @return True if building is legal, false otherwise.
	 */
	bool isBuildingLegal(std::shared_ptr<Player> builder, StreetField field);

	/**
	 * @brief Checks if destroying houses on a street is legal for a player.
	 * @param builder Player attempting to destroy houses.
	 * @param field Street field to destroy houses on.
	 * @return True if destroying is legal, false otherwise.
	 */
	bool isDestroyingLegal(std::shared_ptr<Player> builder, StreetField field);

	/**
	 * @brief Checks if building a hotel on a street is legal for a player.
	 * @param builder Player attempting to build a hotel.
	 * @param field Street field to build a hotel on.
	 * @return True if building a hotel is legal, false otherwise.
	 */
	bool isHotelBuildingLegal(std::shared_ptr<Player> builder, StreetField& field);

	/**
	 * @brief Checks if destroying a hotel on a street is legal for a player.
	 * @param builder Player attempting to destroy a hotel.
	 * @param field Street field to destroy a hotel on.
	 * @return True if destroying a hotel is legal, false otherwise.
	 */
	bool isHotelDestroyingLegal(std::shared_ptr<Player> builder, StreetField& field);

	/**
	 * @brief Checks if a color group is empty (no properties owned) for a player.
	 * @param mortgaging Player attempting to mortgage properties.
	 * @param field Street field to check for a color group.
	 * @return True if the color group is empty, false otherwise.
	 */
	bool colorGroupEmpty(std::shared_ptr<Player> mortgaging, StreetField& field);

	/**
	 * @brief Gets the sprite for a house on a street.
	 * @param field Street field to get the house sprite for.
	 * @param houses_number Number of houses on the street.
	 * @return Sprite of the house.
	 */
	sf::Sprite getHouseSprite(StreetField& field, unsigned int houses_number);

	/**
	 * @brief Gets the sprite for a hotel on a street.
	 * @param field Street field to get the hotel sprite for.
	 * @return Sprite of the hotel.
	 */
	sf::Sprite getHotelSprite(StreetField& field);

	/**
	 * @brief Creates text displaying the available count of houses and hotels.
	 */
	void createAvailableHousesHotelText();

	/**
	 * @brief Updates the text displaying the available count of houses and hotels.
	 */
	void updateAvailableHousesHotelText();

	/**
	 * @brief Calculates the rent to be paid based on the rolled dice value and property position.
	 * @param rolled_val Rolled dice value.
	 * @param pos Property position.
	 * @return Calculated rent amount.
	 */
	unsigned int calculateRent(unsigned int rolled_val, int pos);

	/**
	 * @brief Worker method for managing building and destroying houses/hotels.
	 */
	void buildingsManagingWorker();

	/**
	 * @brief AI worker method for managing building and destroying houses/hotels.
	 */
	void aiBuildingsMangingWorker();

	/**
	 * @brief Main Worker method for whole monopoly game engine.
	 */
	bool monopolyGameWorker();

	/**
	 * @brief Returns a reference to the Withdraw object.
	 * 
	 * @return Withdraw& Reference to the Withdraw object.
	 */
	Withdraw& getWithdraw();

	/**
	 * @brief Creates the chance cards for the game.
	 * 
	 * This method initializes and populates the collection of ChanceCard objects
	 * used in the game.
	 */
	void createChanceCards();

	/**
	 * @brief Shuffles the chance cards.
	 * 
	 * This method shuffles the collection of ChanceCard objects to randomize the
	 * order in which they will be drawn during the game.
	 */
	void shuffleChanceCards();
};

#endif