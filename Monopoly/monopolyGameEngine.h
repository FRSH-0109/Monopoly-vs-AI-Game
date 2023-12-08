#ifndef MONOPOLY_GAME_ENGINE_H
#define MONOPOLY_GAME_ENGINE_H

#include <memory>
#include "main.h"

class monopolyGameEngine {
	unsigned int playersHumanNumber_;
	unsigned int playersAINumber_;
	const unsigned int playersMax_ = 4;
	const unsigned int playersMin_ = 2;

   public:
	monopolyGameEngine();
	unsigned int getPlayersHumanNumber();
	void setPlayersHumanNumber(int number);
	unsigned int getPlayersAINumber();
	void setPlayersAINumber(int number);
};

#endif