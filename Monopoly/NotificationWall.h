/**
 * @file NotificationWall.h
 *
 * @brief Header file of class used to display list
 * of messages/notifications as finite list with certain length.
 * It automatically rolls over when gets next message.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef NOTIFICATION_WALL_H
#define NOTIFICATION_WALL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>

class NotificationWall {
   private:
	unsigned int MAX_NOTIFICATIONS_ = 15;
	sf::Vector2f DRAW_OFFSET_ = sf::Vector2f(200, 200);
	unsigned int Y_STEP_OFFSET_ = 25;
	std::vector<std::shared_ptr<sf::Text>> wallTexts_;
	unsigned int FONT_SIZE_ = 25;
	sf::Font font_;

   public:
	NotificationWall();
	void clearWall();
	std::vector<std::shared_ptr<sf::Text>>& getWall();
	void addToWall(std::string text);
	unsigned int getFontSize() const;
	void setFont(sf::Font font);
	sf::Font& getFont();
};

#endif