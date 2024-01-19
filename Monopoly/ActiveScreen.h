/**
 * @file ActiveScreen.h
 *
 * @brief Header file handling displayed screens of  project
 * Base claass is ActiveScreen, then derived class are used to
 * work with specific screen shown.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef ACTIVE_SCREEN_H
#define ACTIVE_SCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <list>
#include <memory>
#include <string>
#include "Button.h"
#include "ContextWindow.h"
#include "Player.h"
#include "main.h"

/**
 * @class ActiveScreen
 * @brief Represents the base class for handling displayed screens in the project.
 */
class ActiveScreen {
	ContextWindow* context_window_;                       /**< Pointer to the context window. */
    sf::Font font_;                                       /**< The SFML font used for text on the screen. */
    std::vector<std::shared_ptr<Button>> buttons_;        /**< Vector of buttons displayed on the screen. */
    std::vector<std::shared_ptr<sf::Text>> texts_;        /**< Vector of SFML Text objects displayed on the screen. */
    ActiveScreenType type_;                               /**< The type of the active screen. */

   public:
	/**
     * @brief Constructor for the ActiveScreen class.
     */
    ActiveScreen();

    /**
     * @brief Pure virtual function representing the worker function for the screen.
     * @return The ScreenEventType associated with the user interaction.
     */
    virtual ScreenEventType worker() = 0;

    /**
     * @brief Pure virtual function to draw the screen.
     */
    virtual void draw() = 0;

    /**
     * @brief Gets the SFML font used for text on the screen.
     * @return The SFML font.
     */
    sf::Font& getFont();

    /**
     * @brief Sets the SFML font used for text on the screen.
     * @param font The SFML font to set.
     */
    void setFont(sf::Font font);

    /**
     * @brief Adds a button to the screen.
     * @param button_tmp The button to add.
     */
    void addButton(std::shared_ptr<Button> button_tmp);

    /**
     * @brief Adds a text object to the screen.
     * @param text_tmp The SFML Text object to add.
     */
    void addText(std::shared_ptr<sf::Text> text_tmp);

    /**
     * @brief Gets the vector of buttons displayed on the screen.
     * @return Vector of shared pointers to Button objects.
     */
    std::vector<std::shared_ptr<Button>>& getButtons();

    /**
     * @brief Gets the vector of SFML Text objects displayed on the screen.
     * @return Vector of shared pointers to SFML Text objects.
     */
    std::vector<std::shared_ptr<sf::Text>>& getTexts();

    /**
     * @brief Gets the pointer to the ContextWindow.
     * @return Pointer to the ContextWindow.
     */
    ContextWindow* getContextWindow();

    /**
     * @brief Sets the pointer to the ContextWindow.
     * @param context_window Pointer to the ContextWindow to set.
     */
    void setContextWindow(ContextWindow*);

    /**
     * @brief Gets the type of the active screen.
     * @return The ActiveScreenType.
     */
    ActiveScreenType getScreenType();

    /**
     * @brief Sets the type of the active screen.
     * @param type The ActiveScreenType to set.
     */
    void setScreenType(ActiveScreenType type);

    /**
     * @brief Sets the colors of a button based on its state.
     * @param buttonPtr The shared pointer to the Button.
     */
    void buttonSetColors(std::shared_ptr<Button> buttonPtr);

    /**
     * @brief Virtual function to get players' settings.
     * @return Vector of shared pointers to playerSettings objects.
     */
    virtual std::vector<std::shared_ptr<playerSettings>> getPlayersSettings() const;

    /**
     * @brief Virtual function to get players' results.
     * @return Vector of shared pointers to Player objects.
     */
    virtual std::vector<std::shared_ptr<Player>> getPlayersResult();
};

/**
 * @class GameMenuScreen
 * @brief Represents the screen for the game menu.
 */
class GameMenuScreen : public ActiveScreen {
	std::vector<std::shared_ptr<playerSettings>> playerSettingsList_; 				/**< Vector of players' settings. */

	/**
     * @brief Creates player settings columns.
     * @param colNum The number of columns to create.
     * @param posStart The starting position of the columns.
     * @param yStep The step between items in the columns.
     */
	void createPlayerSettingsColumn(int colNum, sf::Vector2f posStart, int yStep);

   public:
	/**
     * @brief Constructor for the GameMenuScreen class.
     */
    GameMenuScreen();

    /**
     * @brief Function to create the game menu.
     */
    void gameMenuCreate();

    /**
     * @brief Worker function for the game menu screen.
     * @return The ScreenEventType associated with the user interaction.
     */
    ScreenEventType worker();

    /**
     * @brief Sets the player settings.
     * @param index The index of the player.
     * @param isNone Flag indicating if the player is set to None.
     * @param isHuman Flag indicating if the player is human.
     * @param level The AI level of the player.
     */
    void setPlayerSettings(unsigned int index, bool isNone, bool isHuman, int level);

    /**
     * @brief Handles button clicks in the game menu.
     * @param buttonPtr The shared pointer to the Button clicked.
     */
    void buttonClickHandle(std::shared_ptr<Button> buttonPtr);

    /**
     * @brief Sets other buttons inactive when a button is clicked.
     * @param buttonPtr The shared pointer to the Button clicked.
     */
    void setOtherButtonsInactive(std::shared_ptr<Button> buttonPtr);

    /**
     * @brief Gets the player number from the ScreenEventType.
     * @param event The ScreenEventType to analyze.
     * @return The player number.
     */
    int getPlayerNumFromEventType(ScreenEventType event);

    /**
     * @brief Sets the AI level column visibility.
     * @param playerNum The player number.
     * @param visible Flag indicating if the column should be visible.
     */
    void setAILevelColumnVisibility(int playerNum, bool visible);

    /**
     * @brief Checks if the ScreenEventType is related to AI level.
     * @param playerNum The player number.
     * @param event The ScreenEventType to analyze.
     * @return True if the event is related to AI level, false otherwise.
     */
    bool isEventTypeAILevel(int playerNum, ScreenEventType event);

    /**
     * @brief Checks if the ScreenEventType is related to setting AI.
     * @param playerNum The player number.
     * @param event The ScreenEventType to analyze.
     * @return True if the event is related to setting AI, false otherwise.
     */
    bool isEventTypeSetAI(int playerNum, ScreenEventType event);

    /**
     * @brief Sets the default AI level buttons' focus.
     * @param playerNum The player number.
     */
    void setDefaultAILevelButtonsFocus(int playerNum);

    /**
     * @brief Gets the players' settings.
     * @return Vector of shared pointers to playerSettings objects.
     */
    std::vector<std::shared_ptr<playerSettings>> getPlayersSettings() const;

    /**
     * @brief Draws the game menu screen.
     */
    void draw();
};

/**
 * @class MainMenuScreen
 * @brief Represents the screen for the main menu.
 */
class MainMenuScreen : public ActiveScreen {
   public:
	public:
    /**
     * @brief Constructor for the MainMenuScreen class.
     */
    MainMenuScreen();

    /**
     * @brief Function to create the main menu.
     */
    void mainMenuCreate();

    /**
     * @brief Worker function for the main menu screen.
     * @return The ScreenEventType associated with the user interaction.
     */
    ScreenEventType worker();

    /**
     * @brief Draws the main menu screen.
     */
    void draw();
};

#endif