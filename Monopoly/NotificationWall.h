/**
 * @file NotificationWall.h
 *
 * @brief Header file for the NotificationWall class.
 *
 * The NotificationWall class is used to display a list of messages/notifications
 * as a finite list with a certain length. It automatically rolls over when it
 * receives the next message.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 */

#ifndef NOTIFICATION_WALL_H
#define NOTIFICATION_WALL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>

/**
 * @class NotificationWall
 *
 * @brief Represents a notification wall that displays messages.
 */
class NotificationWall {
   private:
	unsigned int MAX_NOTIFICATIONS_ = 15;				/**< Maximum number of notifications to display. */
	sf::Vector2f DRAW_OFFSET_ = sf::Vector2f(200, 200); /**< Offset for drawing notifications on the screen. */
	unsigned int Y_STEP_OFFSET_ = 25;					/**< Vertical step offset between notifications. */
	std::vector<std::shared_ptr<sf::Text>>
		wall_texts_;			  /**< Vector to store shared pointers to sf::Text for notifications. */
	unsigned int FONT_SIZE_ = 25; /**< Font size for notifications. */
	sf::Font font_;				  /**< Font used for rendering notifications. */

   public:
	/** Default constructor for the NotificationWall class. */
	NotificationWall();

	/** Clear all notifications from the wall. */
	void clearWall();

	/** Get the vector of shared pointers to sf::Text for notifications. */
	std::vector<std::shared_ptr<sf::Text>>& getWall();

	/**
	 * Add a new message to the notification wall.
	 *
	 * @param text The text of the message to be added.
	 */
	void addToWall(std::string text);

	/** Get the font size used for rendering notifications. */
	unsigned int getFontSize() const;

	/**
	 * Set the font for rendering notifications.
	 *
	 * @param font The font to be set.
	 */
	void setFont(sf::Font font);

	/** Get the font used for rendering notifications. */
	sf::Font& getFont();
};

#endif