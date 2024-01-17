/**
 * @file main.h
 *
 * @brief Header file containing structures shared between project files
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef MAIN_H
#define MAIN_H

class DimensionException : public std::exception {
	unsigned int bad_dimension_;

   public:
	DimensionException(unsigned int dimension) : bad_dimension_(dimension){};
	DimensionException(const DimensionException& e) throw() : bad_dimension_(e.bad_dimension_){};
	unsigned int getBadDimension() { return bad_dimension_; };
};

class RotationException : public std::exception {
	float bad_rotation_;

   public:
	RotationException(float rotation) : bad_rotation_(rotation){};
	RotationException(const RotationException& e) throw() : bad_rotation_(e.bad_rotation_){};
	float getBadRotation() { return bad_rotation_; };
};

class SpriteOffsetException : public std::exception {
	float bad_offset_;

   public:
	SpriteOffsetException(float offset) : bad_offset_(offset){};
	SpriteOffsetException(const SpriteOffsetException& e) throw() : bad_offset_(e.bad_offset_){};
	float getBadOffset() { return bad_offset_; };
};

struct playerSettings {
	bool isNone;
	bool isHuman;
	int level;
};

enum TurnState {
	ROLL_DICE,
	FIELD_ACTION,
	BUY_ACTION,
	PAY_RENT,
	TURN_END,
	WITHDRAW_ONGOING,
	RESULTS,
	NO_TURN,
};

enum GameScreenType { BOARDGAME, WITHDRAW_CHOOSE_PLAYER, WITHDRAW_ADD_VALUE, WITHDRAW_DECISION, AUCTION, RESULT };

enum ActiveScreenType { NONE, MAIN_MENU, GAME_MENU, MONOPOLY_GAME };

enum ScreenEventType {
	IDLE,
	EXIT,
	PLAY,
	RETURN_TO_MAIN_MENU,
	PLAYER_1_SET_NONE,
	PLAYER_2_SET_NONE,
	PLAYER_3_SET_NONE,
	PLAYER_4_SET_NONE,
	PLAYER_1_SET_HUMAN,
	PLAYER_2_SET_HUMAN,
	PLAYER_3_SET_HUMAN,
	PLAYER_4_SET_HUMAN,
	PLAYER_1_SET_AI,
	PLAYER_2_SET_AI,
	PLAYER_3_SET_AI,
	PLAYER_4_SET_AI,
	PLAYER_1_SET_AI_LEVEL_1,
	PLAYER_2_SET_AI_LEVEL_1,
	PLAYER_3_SET_AI_LEVEL_1,
	PLAYER_4_SET_AI_LEVEL_1,
	PLAYER_1_SET_AI_LEVEL_2,
	PLAYER_2_SET_AI_LEVEL_2,
	PLAYER_3_SET_AI_LEVEL_2,
	PLAYER_4_SET_AI_LEVEL_2,
	PLAYER_1_SET_AI_LEVEL_3,
	PLAYER_2_SET_AI_LEVEL_3,
	PLAYER_3_SET_AI_LEVEL_3,
	PLAYER_4_SET_AI_LEVEL_3,
	START_GAME,
	GAME_ENDED
};

enum FieldType { STREET, STATION, UTILITY, GO, CHANCE, COMMUNITY_CHEST, TAX, JAIL, FREE_PARKING, GO_TO_JAIL };

enum StreetTiers { NO_HOUSES, ONE_HOUSE, TWO_HOUESES, THREE_HOUSES, FOUR_HOUSES, HOTEL };

enum StationTiers { ONE_STATION, TWO_STATIONS, THREE_STATIONS, FOUR_STATIONS };

enum UtilityTiers { ONE_UTILITY, TWO_UTILITIES };

enum Decision { YES, NO };

enum JailDecision { ROLL, PAY, CARD };

enum BuyDecision { BUY, RESIGN };

#endif
