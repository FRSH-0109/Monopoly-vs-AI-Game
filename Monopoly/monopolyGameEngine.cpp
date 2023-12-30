#include "monopolyGameEngine.h"

monopolyGameEngine::monopolyGameEngine() {}

void monopolyGameEngine::createBoard() {
	gameboard_ = std::make_shared<Board>(GAMEBOARD_FILE_PATH);
}

void monopolyGameEngine::createPlayers(std::vector<std::shared_ptr<playerSettings>> player_settings_list) {
	std::vector<sf::Color> PLAYER_COLORS = {sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Yellow};
	int i = 0;
	// TODO Parsować po wektorze playerSettings i generować na jego podstawie przeshufflowaną listę graczy)
	for (auto it : player_settings_list) {
		if (!(it->isNone)) {
			Player new_player = Player();
			new_player.setIsAi(!(it->isHuman));
			new_player.setAiLevel(it->level);
			new_player.setColor(PLAYER_COLORS[i]);
			players_.push_back(new_player);
			++i;
		}
	};
	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(players_), std::end(players_), rng);
	for (Player& player: players_) {
		player.createSprite();
	}
}

void monopolyGameEngine::clearPlayers() {
	players_.clear();
}

void monopolyGameEngine::clearBoard() {
	gameboard_->clearBoard();
}

std::shared_ptr<Board> monopolyGameEngine::getBoard() {
	return gameboard_;
}

std::vector<Player> monopolyGameEngine::getPlayers() {
	return players_;
}