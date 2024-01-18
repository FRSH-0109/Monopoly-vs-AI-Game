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

/**
 * @brief Default constructor for the Player class.
 *
 * Initializes the player with default values.
 */
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

/**
 * @brief Constructor for the Player class with initial money.
 *
 * Initializes the player with the specified amount of money.
 *
 * @param money Initial amount of money for the player.
 */
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

/**
 * @brief Get the current position of the player.
 *
 * @return Current position of the player.
 */
unsigned int Player::getPosition() const {
	return position_;
}

/**
 * @brief Set the current position of the player.
 *
 * @param new_position New position for the player.
 */
void Player::setPosition(unsigned int new_position) {
	position_ = new_position;
}

/**
 * @brief Check if the player owns a property with the specified ID.
 *
 * @param id ID of the property to check.
 * @return True if the player owns the property, false otherwise.
 */
bool Player::hasFieldOwnedId(unsigned int id) const {
	for (auto filedId : fields_owned_id_) {
		if (filedId == id) {
			return true;
		}
	}
	return false;
}

/**
 * @brief Get a vector of property IDs owned by the player.
 *
 * @return Vector of property IDs owned by the player.
 */
std::vector<unsigned int> Player::getFieldOwnedId() const {
	return fields_owned_id_;
}

/**
 * @brief Add a property with the specified ID to the list of properties owned by the player.
 *
 * @param id ID of the property to add.
 */
void Player::addFieldOwnedId(unsigned int id) {
	if (hasFieldOwnedId(id) == false) {
		fields_owned_id_.push_back(id);
		std::sort(fields_owned_id_.begin(), fields_owned_id_.end());
	}
}

/**
 * @brief Remove a property with the specified ID from the list of properties owned by the player.
 *
 * @param id ID of the property to remove.
 */
void Player::removeFieldOwnedId(unsigned int id) {
	fields_owned_id_.erase(std::remove(fields_owned_id_.begin(), fields_owned_id_.end(), id), fields_owned_id_.end());
}

/**
 * @brief Clear the list of properties owned by the player.
 */
void Player::clearFieldOwnedId() {
	fields_owned_id_.clear();
}

/**
 * @brief Set the amount of money the player has.
 *
 * @param value New amount of money for the player.
 */
void Player::setMoney(unsigned int value) {
	money_ = value;
}

/**
 * @brief Get the current amount of money the player has.
 *
 * @return Current amount of money the player has.
 */
unsigned int Player::getMoney() const {
	return money_;
}

/**
 * @brief Add a specified amount of money to the player's balance.
 *
 * @param value Amount of money to add.
 */
void Player::addMoney(unsigned int value) {
	if (money_ + value <= UINT_MAX) {
		money_ += value;
	}
}

/**
 * @brief Subtract a specified amount of money from the player's balance.
 *
 * @param value Amount of money to subtract.
 * @return True if the player had enough money and the subtraction was successful, false otherwise.
 */
bool Player::substractMoney(unsigned int value) {
	if (value > money_) {
		return false;
	} else {
		money_ -= value;
		return true;
	}
}

/**
 * @brief Set the jail status of the player.
 *
 * @param new_jail_status New jail status for the player.
 */
void Player::setJailStatus(unsigned int new_jail_status) {
	jail_status_ = new_jail_status;
}

/**
 * @brief Get the current jail status of the player.
 *
 * @return Current jail status of the player.
 */
unsigned int Player::getJailStatus() const {
	return jail_status_;
}

/**
 * @brief Reduce the jail status of the player by one.
 */
void Player::reduceJailStatus() {
	if (jail_status_ != 0) {
		jail_status_ -= 1;
	}
}

/**
 * @brief Set the number of jail cards the player has.
 *
 * @param new_jail_cards New number of jail cards for the player.
 */
void Player::setJailCards(unsigned int new_jail_cards) {
	jail_cards_ = new_jail_cards;
}

/**
 * @brief Get the current number of jail cards the player has.
 *
 * @return Current number of jail cards the player has.
 */
unsigned int Player::getJailCards() const {
	return jail_cards_;
}

/**
 * @brief Set the unique identifier for the player.
 *
 * @param new_id New unique identifier for the player.
 */
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

/**
 * @brief Get the unique identifier of the player.
 *
 * @return Unique identifier of the player.
 */
unsigned int Player::getId() const {
	return id_;
}

/**
 * @brief Set the color associated with the player.
 *
 * @param new_color New color for the player.
 */
void Player::setColor(sf::Color new_color) {
	color_ = new_color;
}

/**
 * @brief Get the color associated with the player.
 *
 * @return Color associated with the player.
 */
sf::Color Player::getColor() const {
	return color_;
}

/**
 * @brief Set the AI status of the player.
 *
 * @param new_is_ai New AI status for the player.
 */
void Player::setIsAi(bool new_is_ai) {
	is_ai_ = new_is_ai;
}

/**
 * @brief Get the AI status of the player.
 *
 * @return True if the player is controlled by AI, false otherwise.
 */
bool Player::getIsAi() const {
	return is_ai_;
}

/**
 * @brief Set the AI level of the player.
 *
 * @param ai_level New AI level for the player.
 */
void Player::setAiLevel(unsigned int ai_level) {
	ai_level_ = ai_level;
}

/**
 * @brief Get the AI level of the player.
 *
 * @return AI level of the player.
 */
unsigned int Player::getAiLevel() const {
	return ai_level_;
}

/**
 * @brief Set the final result place of the player in the game.
 *
 * @param place Final result place of the player in the game.
 */
void Player::setResultPlace(unsigned int place) {
	result_place_ = place;
}

/**
 * @brief Get the final result place of the player in the game.
 *
 * @return Final result place of the player in the game.
 */
unsigned int Player::getResultPlace() const {
	return result_place_;
}

/**
 * @brief Create the sprite for the player.
 */
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

/**
 * @brief Get the texture of the player's sprite.
 *
 * @return Texture of the player's sprite.
 */
sf::Texture& Player::getTexture() {
	return player_texture_;
}

/**
 * @brief Get the sprite representing the player on the game board.
 *
 * @return Sprite representing the player on the game board.
 */
sf::Sprite& Player::getSprite() {
	return player_sprite_;
}

/**
 * @brief Get the offset of the player sprite along the X-axis.
 *
 * @return Offset of the player sprite along the X-axis.
 */
float Player::getSpriteOffsetX() const {
	return sprite_offset_x_;
}

/**
 * @brief Get the offset of the player sprite along the Y-axis.
 *
 * @return Offset of the player sprite along the Y-axis.
 */
float Player::getSpriteOffsetY() const {
	return sprite_offset_y_;
}

/**
 * @brief Set the offset of the player sprite along the X-axis.
 *
 * @param offset_x New offset value.
 */
void Player::setSpriteOffsetX(const float offset_x) {
	if (offset_x >= 0.0 && offset_x <= 1.0) {
		sprite_offset_x_ = offset_x;
	} else {
		throw(SpriteOffsetException(offset_x));
	}
}

/**
 * @brief Set the offset of the player sprite along the Y-axis.
 *
 * @param offset_y New offset value.
 */
void Player::setSpriteOffsetY(const float offset_y) {
	if (offset_y >= 0.0 && offset_y <= 1.0) {
		sprite_offset_y_ = offset_y;
	} else {
		throw(SpriteOffsetException(offset_y));
	}
}

/**
 * @brief Set the position of the player sprite.
 *
 * @param new_pos New position for the player sprite.
 */
void Player::setSpritePosition(sf::Vector2f new_pos) {
	player_sprite_.setPosition(new_pos);
}

/**
 * @brief Make a buying decision.
 *
 * @param index Index of the property to consider.
 * @return BuyDecision object representing the decision.
 */
BuyDecision Player::decideBuy(unsigned int index) {
	return BUY;
}

/**
 * @brief Make a jail decision.
 *
 * @return JailDecision object representing the decision.
 */
JailDecision Player::decideJail() {
	return ROLL;
}

/**
 * @brief Make a mortgage decision.
 *
 * @param index Index of the property to consider.
 * @return Decision object representing the mortgage decision.
 */
Decision Player::decideMortgage(unsigned int index) {
	if (getMoney() <= 0) {
		return YES;
	} else {
		return NO;
	}
}

/**
 * @brief Make an unmortgage decision.
 *
 * @param index Index of the property to consider.
 * @return Decision object representing the unmortgage decision.
 */
Decision Player::decideUnmortgage(unsigned int index) {
	return YES;
}

/**
 * @brief Make a decision for auction bidding.
 *
 * @param price Current price in the auction.
 * @return The bid amount decided by the player.
 */
unsigned int Player::decideAuctionBid(unsigned int price) {
	return price;
}

/**
 * @brief Make a decision for building a house.
 *
 * @return The index of the property on which to build a house.
 */
unsigned int Player::decideBuildHouse() {
	return 15;
}

/**
 * @brief Make a decision for selling a house.
 *
 * @return The index of the property from which to sell a house.
 */
unsigned int Player::decideSellHouse() {
	if (getMoney() <= 0) {
		return 15;
	} else {
		return 0;
	}
}

/**
 * @brief Make a decision for offering a trade.
 *
 * @return Decision object representing the trade offer.
 */
Decision Player::decideOfferTrade() {
	return NO;
}

/**
 * @brief Make a decision for accepting a trade.
 *
 * @return Decision object representing the acceptance of the trade.
 */
Decision Player::decideAcceptTrade() {
	return NO;
}

// =============================================================================
// AiPlayer Class Implementation
// =============================================================================

/**
 * @brief Default constructor for the AiPlayer class.
 *
 * Initializes an AI player with default values.
 */
AiPlayer::AiPlayer() : Player() {
	setIsAi(true);
}

/**
 * @brief Constructor for AiPlayer class with initial money.
 *
 * Initializes the AI player with the specified amount of money.
 *
 * @param money Initial amount of money for the AI player.
 */
AiPlayer::AiPlayer(unsigned int money) : Player(money) {
	setIsAi(true);
};

/**
 * @brief Get the AI adapter associated with the AI player.
 *
 * @return Reference to the AI adapter.
 */
AiAdapter& AiPlayer::getAdapter() {
	return adapter_;
}

/**
 * @brief Get the neural network associated with the AI player.
 *
 * @return Reference to the neural network.
 */
ann::neuralnet& AiPlayer::getNeuralNetwork() {
	return neural_network_;
}

/**
 * @brief Make a buying decision for the AI player (override from base class).
 *
 * This function implements the decision-making process for buying properties by the AI player.
 *
 * @param index Index of the property to consider.
 * @return BuyDecision object representing the decision.
 */
BuyDecision AiPlayer::decideBuy(unsigned int index) {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// if (Y[0] > 0.5f) {
	// 	return BUY;
	// } else {
	// 	return RESIGN;
	// }
	if (index % 2 == 0) {
		return BUY;
	} else {
		return RESIGN;
	}
}

/**
 * @brief Make a jail decision for the AI player (override from base class).
 *
 * This function implements the decision-making process for the AI player when in jail.
 *
 * @return JailDecision object representing the decision.
 */
JailDecision AiPlayer::decideJail() {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// if (Y[1] < 0.333f) {
	// 	return CARD;
	// } else if (Y[1] < 0.666f) {
	// 	return ROLL;
	// } else {
	// 	return PAY;
	// }
	if (getJailStatus() != 1) {
		return ROLL;
	} else {
		return PAY;
	}
}

Decision AiPlayer::decideMortgage(unsigned int index) {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// if (Y[2] < 0.5f) {
	// 	return YES;
	// } else {
	// 	return NO;
	// }

	if (getMoney() <= 0) {
		return YES;
	} else {
		return NO;
	}
}

Decision AiPlayer::decideUnmortgage(unsigned int index) {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// if (Y[3] < 0.5f) {
	// 	return YES;
	// } else {
	// 	return NO;
	// }

	return YES;
}

unsigned int AiPlayer::decideAuctionBid(unsigned int price) {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// double result = Y[4];
	// double money = adapter_.convertMoneyValue(result);

	// return (int)money;

	return price;
}

unsigned int AiPlayer::decideBuildHouse() {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// double result = Y[5];
	// double money = adapter_.convertHouseValue(result);

	// return (int)money;

	return 15;
}

unsigned int AiPlayer::decideSellHouse() {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// double result = Y[6];
	// double money = adapter_.convertHouseValue(result);

	// return (int)money;

	if (getMoney() <= 0) {
		return 15;
	} else {
		return 0;
	}
}

Decision AiPlayer::decideOfferTrade() {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// if (Y[7] > 0.5f) {
	// return YES;
	// } else {
	// return NO;
	// }

	return NO;
}

Decision AiPlayer::decideAcceptTrade() {
	// std::vector<double> Y;
	// std::vector<double> inputs = adapter_.getInputs();
	// neural_network_.evaluate(inputs, Y);

	// if (Y[8] > 0.5f) {
	// return YES;
	// } else {
	// return NO;
	// }

	return NO;
}
