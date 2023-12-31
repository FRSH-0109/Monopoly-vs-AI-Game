#pragma once

#include "activeScreen.h"
#include "monopolyGameEngine.h"

class GameScreen : public ActiveScreen {
	monopolyGameEngine monopoly_game_engine_;

   public:
	GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_);
	~GameScreen();
	ScreenEventType worker();
	void pollForEvents(sf::Event& event);
	void draw();
};
