/**
 * @file ContextWindow.h
 *
 * @brief Source file for context Window class
 * It is Singleton class type used mainly for handling SFML window operations
 * between other classes
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef CONTEXT_WINDOW_H
#define CONTEXT_WINDOW_H

#include <SFML/Graphics.hpp>

/**
 * @class ContextWindow
 *
 * @brief Represents a Singleton class for handling SFML window operations.
 *
 * The ContextWindow class is a Singleton class used mainly for handling SFML window operations
 * between other classes.
 */
class ContextWindow {
   private:
	ContextWindow();								/**< Private constructor to enforce Singleton pattern. */
	static ContextWindow* context_window_;			/**< Static pointer to the ContextWindow instance. */

   public:
   	/**
     * @brief Deleted constructor for proper
	 * Singleton class implementation
     */
	ContextWindow(ContextWindow& other) = delete;

	/**
     * @brief Deleted = operator for proper
	 * Singleton class implementation
     */
	void operator=(const ContextWindow&) = delete;

	/**
     * @brief Gets the pointer to the ContextWindow instance.
     *
     * @return Pointer to the ContextWindow instance.
     */
	static ContextWindow* GetInstance();

	sf::RenderWindow window_;						/**< SFML RenderWindow object for rendering graphics. */
	sf::View view_;									/**< SFML View object for defining a camera in the 2D scene. */

	/**
     * @brief Displays the contents of the window.
     */
	void display();

	/**
     * @brief Clears the contents of the window.
     */
	void clear();

	/**
     * @brief Checks if the window is open.
     *
     * @return True if the window is open, false otherwise.
     */
	bool isOpen();

	/**
     * @brief Gets the SFML RenderWindow object.
     *
     * @return Reference to the SFML RenderWindow object.
     */
	sf::RenderWindow& getWindow();

	/**
     * @brief Gets the SFML View object.
     *
     * @return Reference to the SFML View object.
     */
	sf::View& getView();
};

#endif