#include "NotificationWall.h"

NotificationWall::NotificationWall() {
	clearWall();
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());
}

void NotificationWall::clearWall() {
	wallTexts_.clear();
}

std::vector<std::shared_ptr<sf::Text>>& NotificationWall::getWall() {
	return wallTexts_;
}

void NotificationWall::addToWall(std::string text) {
	std::shared_ptr<sf::Text> newNoti(new sf::Text(text, getFont(), getFontSize() - 2));
	newNoti->setColor(sf::Color::Black);
	wallTexts_.push_back(newNoti);
	if (wallTexts_.size() > MAX_NOTIFICATIONS_) {
		wallTexts_.erase(wallTexts_.begin());
	}

	unsigned int i = 0;
	for (auto notiTextPtr : wallTexts_) {
		notiTextPtr->setPosition(sf::Vector2f(DRAW_OFFSET_.x, DRAW_OFFSET_.y + Y_STEP_OFFSET_ * i));
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
