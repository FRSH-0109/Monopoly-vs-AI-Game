#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include "button.h"
#include "contextWindow.h"
#include "main.h"

class MenuScreen {
	ContextWindow* contextWindow_;
	sf::Font font_;
	std::vector<std::shared_ptr<Button>> buttons_;
	ActiveScreenType type_;

   public:
	virtual ScreenEventType worker() = 0;
	virtual void pollForEvents(sf::Event& event) = 0;

	sf::Font& getFont();
	void setFont(sf::Font font);
	void addButton(std::shared_ptr<Button> buttonTmp);
	std::vector<std::shared_ptr<Button>>& getButtons();
	void draw();
	ContextWindow* getContextWindow();
	void setContextWindow(ContextWindow*);
	ActiveScreenType getScreenType();
	void setScreenType(ActiveScreenType type);
};

class GameMenuScreen : public MenuScreen {
   public:
	GameMenuScreen();
	void gameMenuCreate();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);
};

class MainMenuScreen : public MenuScreen {
   public:
	MainMenuScreen();
	void mainMenuCreate();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);
};

#endif