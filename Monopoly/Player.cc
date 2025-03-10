/**
 * @file Player.cc
 *
 * @brief Implementation file for Player class and AI Player class,
 * containing data and methods for a player in a Monopoly game.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 */

#include "Player.h"

// =============================================================================
// Player Class Implementation
// =============================================================================

Player::Player() {
	setAiLevel(0);
	setId(0);
	setPosition(0);
	clearFieldOwnedId();
	setMoney(0);
	setJailStatus(0);
	setJailCards(0);
	setResultPlace(0);
	setIsAi(false);
}

Player::Player(unsigned int money) {
	setAiLevel(0);
	setId(0);
	setPosition(0);
	clearFieldOwnedId();
	setMoney(money);
	setJailStatus(0);
	setJailCards(0);
	setResultPlace(0);
	setIsAi(false);
}

unsigned int Player::getPosition() const {
	return position_;
}

void Player::setPosition(unsigned int new_position) {
	position_ = new_position;
}

bool Player::hasFieldOwnedId(unsigned int id) const {
	for (auto filedId : fields_owned_id_) {
		if (filedId == id) {
			return true;
		}
	}
	return false;
}

std::vector<unsigned int> Player::getFieldOwnedId() const {
	return fields_owned_id_;
}

void Player::addFieldOwnedId(unsigned int id) {
	if (hasFieldOwnedId(id) == false) {
		fields_owned_id_.push_back(id);
		std::sort(fields_owned_id_.begin(), fields_owned_id_.end());
	}
}

void Player::removeFieldOwnedId(unsigned int id) {
	fields_owned_id_.erase(std::remove(fields_owned_id_.begin(), fields_owned_id_.end(), id), fields_owned_id_.end());
}

void Player::clearFieldOwnedId() {
	fields_owned_id_.clear();
}

void Player::setMoney(unsigned int value) {
	money_ = value;
}

unsigned int Player::getMoney() const {
	return money_;
}

void Player::addMoney(unsigned int value) {
	if (money_ + value <= UINT_MAX) {
		money_ += value;
	}
}

bool Player::substractMoney(unsigned int value) {
	if (value > money_) {
		return false;
	} else {
		money_ -= value;
		return true;
	}
}

void Player::setJailStatus(unsigned int new_jail_status) {
	jail_status_ = new_jail_status;
}

unsigned int Player::getJailStatus() const {
	return jail_status_;
}

void Player::reduceJailStatus() {
	if (jail_status_ != 0) {
		jail_status_ -= 1;
	}
}

void Player::setJailCards(unsigned int new_jail_cards) {
	jail_cards_ = new_jail_cards;
}

unsigned int Player::getJailCards() const {
	return jail_cards_;
}

void Player::setId(unsigned int new_id) {
	if (new_id <= 3) {
		id_ = new_id;
		switch (id_) {
			case 0:
				setColor(sf::Color::Green);
				break;
			case 1:
				setColor(sf::Color::Red);
				break;
			case 2:
				setColor(sf::Color::Blue);
				break;
			case 3:
				setColor(sf::Color::Yellow);
				break;
			default:
				break;
		}
	}
}

unsigned int Player::getId() const {
	return id_;
}

void Player::setColor(sf::Color new_color) {
	color_ = new_color;
}

sf::Color Player::getColor() const {
	return color_;
}

void Player::setIsAi(bool new_is_ai) {
	is_ai_ = new_is_ai;
}

bool Player::getIsAi() const {
	return is_ai_;
}

void Player::setAiLevel(unsigned int ai_level) {
	ai_level_ = ai_level;
}

unsigned int Player::getAiLevel() const {
	return ai_level_;
}

void Player::setResultPlace(unsigned int place) {
	result_place_ = place;
}

unsigned int Player::getResultPlace() const {
	return result_place_;
}

void Player::createSprite() {
	const float WIDTH = 20.0;
	const float HEIGHT = 20.0;
	const std::string TEXTURE_PATH = "textures_and_fonts/textures/Pionek_monopoly.png";
	if (!player_texture_.loadFromFile(TEXTURE_PATH)) {
		player_sprite_.setColor(sf::Color::Black);
	}
	player_sprite_.setTexture(player_texture_, true);
	player_sprite_.setColor(color_);
	player_sprite_.setOrigin(
		player_sprite_.getGlobalBounds().getSize() / 2.f + player_sprite_.getLocalBounds().getPosition());
	sf::Vector2u texture_dim = player_texture_.getSize();
	float scale_x = (float)WIDTH / (float)texture_dim.x;
	float scale_y = (float)HEIGHT / (float)texture_dim.y;
	const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
	player_sprite_.setScale(SCALE_VECT);
}

sf::Texture& Player::getTexture() {
	return player_texture_;
}

sf::Sprite& Player::getSprite() {
	return player_sprite_;
}

float Player::getSpriteOffsetX() const {
	return sprite_offset_x_;
}

float Player::getSpriteOffsetY() const {
	return sprite_offset_y_;
}

void Player::setSpriteOffsetX(const float offset_x) {
	if (offset_x >= 0.0 && offset_x <= 1.0) {
		sprite_offset_x_ = offset_x;
	} else {
		throw(SpriteOffsetException(offset_x));
	}
}

void Player::setSpriteOffsetY(const float offset_y) {
	if (offset_y >= 0.0 && offset_y <= 1.0) {
		sprite_offset_y_ = offset_y;
	} else {
		throw(SpriteOffsetException(offset_y));
	}
}

void Player::setSpritePosition(sf::Vector2f new_pos) {
	player_sprite_.setPosition(new_pos);
}

AiAdapter& Player::getAdapter() {
	AiAdapter adapter_;
	return adapter_;
}

BuyDecision Player::decideBuy(unsigned int index) {
	return BUY;
}

JailDecision Player::decideJail() {
	return ROLL;
}

Decision Player::decideMortgage(unsigned int index) {
	if (getMoney() <= 0) {
		return YES;
	} else {
		return NO;
	}
}

Decision Player::decideUnmortgage(unsigned int index) {
	return YES;
}

unsigned int Player::decideAuctionBid(unsigned int price) {
	return price;
}

unsigned int Player::decideBuildHouse() {
	return 15;
}

unsigned int Player::decideSellHouse() {
	if (getMoney() <= 0) {
		return 15;
	} else {
		return 0;
	}
}

Decision Player::decideOfferTrade() {
	return NO;
}

Decision Player::decideAcceptTrade() {
	return NO;
}

// =============================================================================
// AiPlayer Class Implementation
// =============================================================================

AiPlayer::AiPlayer() : Player() {
	setIsAi(true);
}

AiPlayer::AiPlayer(unsigned int money) : Player(money) {
	setIsAi(true);
};

AiPlayer::AiPlayer(unsigned int money, ann::neuralnet nn) : Player(money) {
	setIsAi(true);
	neural_network_ = nn;
}

AiAdapter& AiPlayer::getAdapter() {
	return adapter_;
}

ann::neuralnet& AiPlayer::getNeuralNetwork() {
	return neural_network_;
}

void AiPlayer::setNeuralNetwork(ann::neuralnet new_nn) {
	neural_network_ = new_nn;
}

BuyDecision AiPlayer::decideBuy(unsigned int index) {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	if (getAiLevel() == 1) {
		return BUY;
	} else if (getAiLevel() == 2) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision >= 0.5) {
			return BUY;
		} else if (decision < 0.5) {
			return RESIGN;
		}

	} else if (getAiLevel() == 3) {
		if (getMoney() > 500) {
			return BUY;
		} else if (getMoney() > 400 && getMoney() <= 500) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dist(0.0, 1.0);
			double decision = dist(gen);

			if (decision >= 0.5) {
				return BUY;
			} else if (decision < 0.5) {
				return RESIGN;
			}
		} else if (getMoney() < 400) {
			return RESIGN;
		}
	}

	/*
	Logic prepared for NEAT AI
	*/
	if (Y[0] > 0.5f) {
		return BUY;
	} else {
		return RESIGN;
	}

	// if (index % 2 == 0) {
	// 	return BUY;
	// } else {
	// 	return RESIGN;
	// }
}

JailDecision AiPlayer::decideJail() {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	if (getAiLevel() == 1) {
		if (getMoney() > 50) {
			return PAY;
		} else {
			return ROLL;
		}
	} else if (getAiLevel() == 2) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (getMoney() > 50 && decision < 0.1) {
			return PAY;
		} else {
			return ROLL;
		}

	} else if (getAiLevel() == 3) {
		if (getFieldOwnedId().size() < 3 && getMoney() > 50) {
			return PAY;
		} else {
			return ROLL;
		}
	}

	/*
	Logic prepared for NEAT AI
	*/
	if (Y[1] < 0.333f) {
		return CARD;
	} else if (Y[1] < 0.666f) {
		return ROLL;
	} else {
		return PAY;
	}

	// if (getJailStatus() != 1) {
	// 	return ROLL;
	// } else {
	// 	return PAY;
	// }
}

Decision AiPlayer::decideMortgage(unsigned int index) {
	std::vector<std::vector<unsigned int>> SETS = {
		{1, 3}, {6, 8, 9}, {11, 13, 14}, {16, 18, 19}, {21, 23, 24}, {26, 27, 29}, {31, 32, 34}, {37, 39}};

	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	std::vector<unsigned int> group;

	for (auto set: SETS) {
		if (std::find(set.begin(), set.end(), index) != set.end()) {
			group = set;
			break;
		}
	}

	if (getMoney() < 50) {
		return YES;
	} else if (std::includes(getFieldOwnedId().begin(), getFieldOwnedId().end(), group.begin(), group.end())) {
		return NO;
	} else {
		return YES;
	}

	/*
	Logic prepared for NEAT AI
	*/
	if (Y[2] < 0.5f) {
		return YES;
	} else {
		return NO;
	}
}

Decision AiPlayer::decideUnmortgage(unsigned int index) {
	std::vector<std::vector<unsigned int>> SETS = {
		{1, 3}, {6, 8, 9}, {11, 13, 14}, {16, 18, 19}, {21, 23, 24}, {26, 27, 29}, {31, 32, 34}, {37, 39}};

	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	std::vector<unsigned int> group;

	for (auto set: SETS) {
		if (std::find(set.begin(), set.end(), index) != set.end()) {
			group = set;
			break;
		}
	}

	if (getMoney() > 900) {
		return YES;
	} else if (std::includes(getFieldOwnedId().begin(), getFieldOwnedId().end(), group.begin(), group.end()) && getMoney() > 440) {
		return YES;
	} else {
		return NO;
	}

	/*
	Logic prepared for NEAT AI
	*/
	if (Y[3] < 0.5f) {
		return YES;
	} else {
		return NO;
	}

	// return YES;
}

unsigned int AiPlayer::decideAuctionBid(unsigned int price) {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	if (getAiLevel() == 1) {
		return price;
	} else if (getAiLevel() == 2) {
		std::random_device rd;
		std::mt19937 gen(rd());
		double lower_bound = (double)price * 0.8;
		double upper_bound = (double)price * 2.0;
		std::uniform_real_distribution<double> dist(lower_bound, upper_bound);
		double decision = dist(gen);

		return (unsigned int)decision;

	} else if (getAiLevel() == 3) {
		std::random_device rd;
		std::mt19937 gen(rd());
		double lower_bound = (double)price * 0.5;
		double upper_bound = (double)price * 1.2;
		std::uniform_real_distribution<double> dist(lower_bound, upper_bound);
		double decision = dist(gen);

		return (unsigned int)decision;
	}

	double result = Y[4];
	double money = adapter_.convertMoneyValue(result);

	return (int)money;

	// return price;
}

unsigned int AiPlayer::decideBuildHouse() {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	double result = Y[5];
	double money = adapter_.convertHouseValue(result);

	if (getAiLevel() == 1) {
		if (getMoney() > 1000) {
			return 15;
		} else if (getMoney() > 500) {
			return 6;
		} else if (getMoney() > 300) {
			return 1;
		} else if (getMoney() <= 300) {
			return 0;
		}
	} else if (getAiLevel() == 2) {
		if (getMoney() > 1000) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(11, 15);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() > 500) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(6, 10);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() > 300) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(1, 5);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() <= 300) {
			return 0;
		}
	} else if (getAiLevel() == 3) {
		if (getMoney() > 1300) {
			return 15;
		} else if (getMoney() > 1000) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(11, 15);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() > 500) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(6, 10);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() > 300) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(1, 5);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() <= 300) {
			return 0;
		}
	}

	return (int)money;

	// return 15;
}

unsigned int AiPlayer::decideSellHouse() {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	double result = Y[6];
	double money = adapter_.convertHouseValue(result);

	if (getAiLevel() == 1) {
		if (getMoney() > 1000) {
			return 0;
		} else if (getMoney() > 500) {
			return 1;
		} else if (getMoney() > 100) {
			return 8;
		} else if (getMoney() <= 100) {
			return 15;
		}
	} else if (getAiLevel() == 2) {
		if (getMoney() > 1000) {
			return 0;
		} else if (getMoney() > 300) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(1, 3);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() > 100) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(4, 6);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() <= 100) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(7, 15);
			double decision = dist(gen);

			return decision;
		}
	} else if (getAiLevel() == 3) {
		if (getMoney() > 1300) {
			return 0;
		} else if (getMoney() > 1000) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dist(0.0, 1.0);
			double decision = dist(gen);

			if (decision > 0.9) {
				return 1;
			} else if (decision <= 0.9) {
				return 0;
			}

		} else if (getMoney() > 300) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(4, 6);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() > 100) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<unsigned int> dist(7, 15);
			double decision = dist(gen);

			return decision;
		} else if (getMoney() <= 100) {
			return 15;
		}
	}

	return (int)money;

	// if (getMoney() <= 0) {
	// 	return 15;
	// } else {
	// 	return 0;
	// }
}

Decision AiPlayer::decideOfferTrade() {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	if (getAiLevel() == 1) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision > 0.98) {
			return YES;
		} else if (decision <= 0.98) {
			return NO;
		}
	} else if (getAiLevel() == 2) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision > 0.93) {
			return YES;
		} else if (decision <= 0.93) {
			return NO;
		}

	} else if (getAiLevel() == 3) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision > 0.9) {
			return YES;
		} else if (decision <= 0.9) {
			return NO;
		}
	}

	if (Y[7] > 0.5f) {
		return YES;
	} else {
		return NO;
	}

	// return NO;
}

Decision AiPlayer::decideAcceptTrade() {
	std::vector<double> Y(9, 0.0);
	std::vector<double> inputs = adapter_.getInputs();
	neural_network_.evaluate(inputs, Y);

	if (getAiLevel() == 1) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision > 0.2) {
			return YES;
		} else if (decision <= 0.2) {
			return NO;
		}
	} else if (getAiLevel() == 2) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision > 0.5) {
			return YES;
		} else if (decision <= 0.5) {
			return NO;
		}

	} else if (getAiLevel() == 3) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double decision = dist(gen);

		if (decision > 0.7) {
			return YES;
		} else if (decision <= 0.7) {
			return NO;
		}
	}

	if (Y[8] > 0.5f) {
		return YES;
	} else {
		return NO;
	}

	// return NO;
}
