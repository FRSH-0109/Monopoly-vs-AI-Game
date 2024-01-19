/**
 * @file NotificationWall.cc
 *
 * @brief Source file for the NotificationWall class.
 *
 * The NotificationWall class is used to display a list of messages/notifications
 * as a finite list with a certain length. It automatically rolls over when it
 * receives the next message.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 */

#include "NotificationWall.h"

// =============================================================================
// NotificationWall Class Implementation
// =============================================================================

NotificationWall::NotificationWall() {
	clearWall();
	// if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.otf"))
	// 	std::cout << "Font not found!\n";

	setFont(getFont());
}

void NotificationWall::clearWall() {
	wall_texts_.clear();
}

std::vector<std::shared_ptr<sf::Text>>& NotificationWall::getWall() {
	return wall_texts_;
}

void NotificationWall::addToWall(std::string text) {
	std::shared_ptr<sf::Text> new_noti(new sf::Text(text, getFont(), getFontSize() - 2));
	new_noti->setFillColor(sf::Color::Black);
	wall_texts_.push_back(new_noti);
	if (wall_texts_.size() > MAX_NOTIFICATIONS_) {
		wall_texts_.erase(wall_texts_.begin());
	}

	unsigned int i = 0;
	for (auto noti_text_ptr : wall_texts_) {
		noti_text_ptr->setPosition(sf::Vector2f(DRAW_OFFSET_.x, DRAW_OFFSET_.y + Y_STEP_OFFSET_ * i));
		++i;
	}
}

unsigned int NotificationWall::getFontSize() const {
	return FONT_SIZE_;
}

void NotificationWall::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& NotificationWall::getFont() {
	return font_;
}
