/**
 * @file GameEngine.h
 *
 * @brief Header file for GameEngine class,
 * used to handle lowes level program operations as
 * input interactions (mouse, keyboard) or display window
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include <typeinfo>
#include <variant>
#include <vector>
#include "ActiveScreen.h"
#include "ContextWindow.h"
#include "GameScreen.h"
#include "Player.h"
#include "main.h"

/**
 * @class GameEngine
 *
 * @brief Handles low-level program operations, including input interactions and window display.
 *
 * The GameEngine class manages fundamental program operations, such as handling input interactions
 * (mouse, keyboard) and displaying the window. It incorporates an ActiveScreen to manage various game screens.
 * Also connects main monopolyGame engine with main loop in main.c to exchange data about results.
 */
class GameEngine {
   private:
	ContextWindow* context_window_;						/**< Pointer to the context window used for rendering graphics.*/

	std::unique_ptr<ActiveScreen> active_screen_;		/**< Unique pointer to the active screen managing different purpose screens.*/

	double frame_rate_hz_;								/**< Frame rate in Hertz (frames per second).*/
	sf::Time frame_rate_delay_ms_;							/**< Time delay for achieving the specified frame rate.*/
	unsigned int window_width_;							/**< Width of the game window in pixels.*/
	unsigned int window_height_;							/**< Height of the game window in pixels.*/

	std::vector<std::shared_ptr<Player>> players_;		/**< Vector of shared pointers to Player objects representing game players.*/

   public:
  	/**
     * @brief Constructor for the GameEngine class.
     *
     * @param frame_rate_hz Desired frame rate in Hertz (frames per second).
     * @param window_width Width of the game window in pixels.
     * @param window_height Height of the game window in pixels.
     */
	GameEngine(double frame_rate_hz, uint window_width, uint window_height);

	/**
     * @brief Additional constructor for the GameEngine class. Used only to train AI players
     *
     * @param frame_rate_hz Desired frame rate in Hertz (frames per second).
     */
	GameEngine(double frame_rate_hz);

	 /**
     * @brief Clears content on the displayed window.
     */
	void clear();

	/**
     * @brief Displays the content of the context window.
     */
	void display();

	/**
     * @brief Polls for events such as mouse and keyboard interactions.
     *
     * @param event Reference to an sf::Event object to store the polled event.
     */
	void pollForEvents(sf::Event& event);

	/**
     * @brief Worker function for processing player-related tasks.
     *
     * @param players_vec Vector of shared pointers to Player objects representing game players.
     * @return Vector of shared pointers to Player objects after processing.
     */
	std::vector<std::shared_ptr<Player>> worker(std::vector<std::shared_ptr<Player>>& players_vec);

	/**
     * @brief Gets the width of the game window.
     *
     * @return Width of the game window in pixels.
     */
	unsigned int getwindow_width() const;

	/**
     * @brief Gets the height of the game window.
     *
     * @return Height of the game window in pixels.
     */
	unsigned int getwindow_height() const;

	/**
     * @brief Gets the pointer to the context window.
     *
     * @return Pointer to the context window used for rendering graphics.
     */
	ContextWindow* getContextWindow();
};

#endif