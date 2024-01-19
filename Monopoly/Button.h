/**
 * @file Button.h
 *
 * @brief Header file for handling button objects actions
 * used to communicate with user.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "main.h"

/**
 * @class Button
 * @brief Represents a button for handling user interactions.
 */

class Button {
   public:
    /**
     * @brief Constructor for the Button class.
     * @param type The type of screen event associated with the button.
     * @param btnText The text displayed on the button.
     * @param buttonSize The size of the button.
     * @param charSize The character size of the text.
     */
	Button(ScreenEventType type, sf::String btnText, sf::Vector2f buttonSize, int charSize);

	/**
     * @brief Draws the button on the specified window.
     * @param window The SFML window to draw the button on.
     */
	void draw(sf::RenderWindow& window);

    /**
     * @brief Checks if the mouse is over the button.
     * @param window The SFML window.
     * @return True if the mouse is over the button, false otherwise.
     */
	bool isMouseOver(sf::RenderWindow& window);

    /**
     * @brief Virtual function called when the mouse is over the button.
     */
	virtual void mouseIsOver();

    /**
     * @brief Virtual function called when the mouse is not over the button.
     */
	virtual void mouseIsNotOver();

    /**
     * @brief Gets the type of screen event associated with the button.
     * @return The screen event type.
     */
	ScreenEventType getEventType();

    /**
     * @brief Sets the type of screen event associated with the button.
     * @param event The screen event type to set.
     */
	void setEventType(ScreenEventType event);

    /**
     * @brief Sets the font for the button text.
     * @param fonts The font to set.
     */
	void setFont(sf::Font& fonts);

    /**
     * @brief Sets the position of the button.
     * @param point The position to set.
     */
	void setPosition(sf::Vector2f point);

    /**
     * @brief Sets the state of the button as clicked or not clicked.
     * @param state The state to set.
     */
	void setIsClicked(bool state);

    /**
     * @brief Gets the state of the button (clicked or not clicked).
     * @return True if the button is clicked, false otherwise.
     */
	bool getIsClicked();

    /**
     * @brief Gets the size of the button.
     * @return The size of the button.
     */
	sf::Vector2f getSize();

    /**
     * @brief Sets the visibility state of the button.
     * @param state The visibility state to set.
     */
	void setIsVisible(bool state);

    /**
     * @brief Gets the visibility state of the button.
     * @return True if the button is visible, false otherwise.
     */
	bool getIsVisible();

    /**
     * @brief Sets the active state of the button.
     * @param state The active state to set.
     */
	void setIsActive(bool state);

    /**
     * @brief Gets the active state of the button.
     * @return True if the button is active, false otherwise.
     */
	bool getIsActive();

    /**
     * @brief Sets the focus state of the button.
     * @param state The focus state to set.
     */
	void setIsFocus(bool state);

    /**
     * @brief Gets the focus state of the button.
     * @return True if the button is in focus, false otherwise.
     */
	bool getIsFocus();

    /**
     * @brief Sets the state of the button as released or not released.
     * @param state The state to set.
     */
	void setWasReleased(bool state);

    /**
     * @brief Gets the state of the button (released or not released).
     * @return True if the button was released, false otherwise.
     */
	bool getWasReleased();

    /**
     * @brief Gets the SFML Text object associated with the button.
     * @return The SFML Text object.
     */
	sf::Text& getText();

    /**
     * @brief Sets the background color and text color when the button is active.
     * @param color The background color to set.
     */
	void setActiveBackColor(sf::Color color);

    /**
     * @brief Sets the text color when the button is active.
     * @param color The text color to set.
     */
	void setActiveTextColor(sf::Color color);

    /**
     * @brief Sets the background color and text color when the button is inactive.
     * @param color The background color to set.
     */
	void setInactiveBackColor(sf::Color color);

    /**
     * @brief Sets the text color when the button is inactive.
     * @param color The text color to set.
     */
	void setInactiveTextColor(sf::Color color);

    /**
     * @brief Sets the background color and text color when the button is in focus.
     * @param color The background color to set.
     */
	void setFocusBackColor(sf::Color color);

    /**
     * @brief Sets the text color when the button is in focus.
     * @param color The text color to set.
     */
	void setFocusTextColor(sf::Color color);

    /**
     * @brief Sets the button state to focused.
     */
	void setButtonFocus();

    /**
     * @brief Sets the button state to unfocused.
     */
	void setButtonUnfocus();

    /**
     * @brief Updates the colors of the button based on its state.
     */
	void updateColors();

   private:
	sf::RectangleShape buttonShape_;   /**< The SFML RectangleShape representing the button. */
	sf::Text text_;                     /**< The SFML Text object representing the text on the button. */
	ScreenEventType type_;              /**< The type of screen event associated with the button. */

	uint btnWidth;                      /**< The width of the button. */
	uint btnHeight;                     /**< The height of the button. */

	bool isClicked_;                    /**< Flag indicating if the button is clicked. */
	bool isVisible_;                    /**< Flag indicating if the button is visible. */
	bool isActive_;                     /**< Flag indicating if the button is active. */
	bool isFocus_;                      /**< Flag indicating if the button is in focus. */
	bool wasReleased_;                  /**< Flag indicating if the button was released. */

	sf::Color activeBackColor_;         /**< The background color when the button is active. */
	sf::Color activeTextColor_;         /**< The text color when the button is active. */
	sf::Color inActiveBackColor_;       /**< The background color when the button is inactive. */
	sf::Color inActiveTextColor_;       /**< The text color when the button is inactive. */
	sf::Color focusBackColor_;          /**< The background color when the button is in focus. */
	sf::Color focusTextColor_;          /**< The text color when the button is in focus. */
};

#endif