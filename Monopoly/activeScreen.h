#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <memory>
#include "button.h"
#include "contextWindow.h"
#include "main.h"

class ActiveScreen {
	ContextWindow* contextWindow_;
	sf::Font font_;
	std::vector<std::shared_ptr<Button>> buttons_;
	std::vector<std::shared_ptr<sf::Text>> texts_;
	ActiveScreenType type_;

   public:
	ActiveScreen();
	virtual ~ActiveScreen();
	virtual ScreenEventType worker() = 0;
	virtual void pollForEvents(sf::Event& event) = 0;

	sf::Font& getFont();
	void setFont(sf::Font font);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);
	std::vector<std::shared_ptr<Button>>& getButtons();
	void draw();
	ContextWindow* getContextWindow();
	void setContextWindow(ContextWindow*);
	ActiveScreenType getScreenType();
	void setScreenType(ActiveScreenType type);
};

class GameMenuScreen : public ActiveScreen {
   public:
	GameMenuScreen();
	~GameMenuScreen();
	void gameMenuCreate();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);
};

class MainMenuScreen : public ActiveScreen {
   public:
	MainMenuScreen();
	~MainMenuScreen();
	void mainMenuCreate();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);
};

#endif