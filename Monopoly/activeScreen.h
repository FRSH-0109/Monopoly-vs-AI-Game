#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <list>
#include <memory>
#include <string>
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

	virtual void eventHandle(ScreenEventType) = 0;
};

class GameMenuScreen : public ActiveScreen {
	struct playerSettings {
		bool isNone;
		bool isHuman;
		int level;
	};

	std::vector<std::shared_ptr<playerSettings>> playerSettingsList_;

	void createPlayerSettingsColumn(int colNum, sf::Vector2f posStart, int yStep);

   public:
	GameMenuScreen();
	~GameMenuScreen();
	void gameMenuCreate();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);

	void setPlayerSettings(int index, bool isNone, bool isHuman, int level);

	void eventHandle(ScreenEventType eventType);
};

class MainMenuScreen : public ActiveScreen {
   public:
	MainMenuScreen();
	~MainMenuScreen();
	void mainMenuCreate();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);

	void eventHandle(ScreenEventType eventType);
};

#endif