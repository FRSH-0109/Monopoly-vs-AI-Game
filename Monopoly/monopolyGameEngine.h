#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <vector>
#include "Board.h"
#include "activeScreen.h"
#include "main.h"

class monopolyGameEngine {
	const unsigned int playersMax_ = 4;
	const unsigned int playersMin_ = 2;

	const std::string GAMEBOARD_FILE_PATH = "Monopoly/game_config_json/board.json";
	std::shared_ptr<Board> gameboard_;
	std::vector<Player> players_;

   public:
	monopolyGameEngine();

	void createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list);
	void clearPlayers();
	void createBoard();
	void clearBoard();
	std::shared_ptr<Board> getBoard();
	std::vector<Player>& getPlayers();
};
