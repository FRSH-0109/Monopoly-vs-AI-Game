#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <list>
#include <memory>
#include <string>
#include "Player.h"
#include "button.h"
#include "contextWindow.h"
#include "main.h"
// #include "monopolyGameEngine.h"

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
	virtual void draw() = 0;

	sf::Font& getFont();
	void setFont(sf::Font font);
	void addButton(std::shared_ptr<Button> buttonTmp);
	void addText(std::shared_ptr<sf::Text> textTmp);
	std::vector<std::shared_ptr<Button>>& getButtons();
	std::vector<std::shared_ptr<sf::Text>>& getTexts();
	ContextWindow* getContextWindow();
	void setContextWindow(ContextWindow*);
	ActiveScreenType getScreenType();
	void setScreenType(ActiveScreenType type);
	void buttonSetColors(std::shared_ptr<Button> buttonPtr);
	virtual std::vector<std::shared_ptr<playerSettings>> getPlayersSettings() const;
	virtual std::vector<std::shared_ptr<Player>> getPlayersResult();
};

class GameMenuScreen : public ActiveScreen {
	std::vector<std::shared_ptr<playerSettings>> playerSettingsList_;
	void createPlayerSettingsColumn(int colNum, sf::Vector2f posStart, int yStep);

   public:
	GameMenuScreen();
	~GameMenuScreen();
	void gameMenuCreate();
	ScreenEventType worker();

	void setPlayerSettings(unsigned int index, bool isNone, bool isHuman, int level);

	void buttonClickHandle(std::shared_ptr<Button> buttonPtr);
	void setOtherButtonsInactive(std::shared_ptr<Button> buttonPtr);
	int getPlayerNumFromEventType(ScreenEventType event);
	void setAILevelColumnVisibility(int playerNum, bool visible);
	bool isEventTypeAILevel(int playerNum, ScreenEventType event);
	bool isEventTypeSetAI(int playerNum, ScreenEventType event);
	void setDefaultAILevelButtonsFocus(int playerNum);
	std::vector<std::shared_ptr<playerSettings>> getPlayersSettings() const;
	void draw();
};

class MainMenuScreen : public ActiveScreen {
   public:
	MainMenuScreen();
	~MainMenuScreen();
	void mainMenuCreate();
	ScreenEventType worker();
	void draw();
};
