#pragma once

#include "Player.h"
#include "ActiveScreen.h"
#include "MonopolyGameEngine.h"

class GameScreen : public ActiveScreen {
	monopolyGameEngine monopoly_game_engine_;

   public:
	GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_);
	~GameScreen();
	ScreenEventType worker();
	void draw();
	std::vector<std::shared_ptr<Player>> getPlayersResult();
};
