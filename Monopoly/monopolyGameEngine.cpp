#include "monopolyGameEngine.h"

monopolyGameEngine::monopolyGameEngine() {
	playersHumanNumber_ = 1;
	playersAINumber_ = 1;
}

unsigned int monopolyGameEngine::getPlayersHumanNumber() {
	return playersHumanNumber_;
}

void monopolyGameEngine::setPlayersHumanNumber(int number) {
	if (number >= 0) {
		if (number > playersHumanNumber_) {
			if (number + playersAINumber_ <= playersMax_) {
				playersHumanNumber_ = number;
			}
		} else if (number < playersHumanNumber_) {
			if (number + playersAINumber_ >= playersMin_) {
				playersHumanNumber_ = number;
			}
		}
	}
}

unsigned int monopolyGameEngine::getPlayersAINumber() {
	return playersAINumber_;
}
void monopolyGameEngine::setPlayersAINumber(int number) {
	if (number >= 0) {
		if (number > playersAINumber_) {
			if (number + playersHumanNumber_ <= playersMax_) {
				playersAINumber_ = number;
			}
		} else if (number < playersAINumber_) {
			if (number + playersHumanNumber_ >= playersMin_) {
				playersAINumber_ = number;
			}
		}
	}
}

void monopolyGameEngine::createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list) {
	// TODO Parsować po wektorze playerSettings i generować na jego podstawie przeshufflowaną listę graczy)
	for (auto it : player_settings_list)
	{
		it->isNone;
	};
}