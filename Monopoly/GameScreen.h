/**
 * @file GameScreen.h
 *
 * @brief Header file for game screen class deriving from
 * ActiveScreen class. Used to handle monopoly game activities
 * and drawing.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "ActiveScreen.h"
#include "MonopolyGameEngine.h"
#include "Player.h"

class GameScreen : public ActiveScreen {
	monopolyGameEngine monopoly_game_engine_;

   public:
	GameScreen(std::vector<std::shared_ptr<Player>>& players_);
	ScreenEventType worker();
	void draw();
	std::vector<std::shared_ptr<Player>> getPlayersResult();
};

#endif