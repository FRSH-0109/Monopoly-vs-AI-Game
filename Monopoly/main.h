#pragma once

class DimensionException : public std::exception {
	unsigned int bad_dimension_;

   public:
	DimensionException(unsigned int dimension) : bad_dimension_(dimension){};
	DimensionException(const DimensionException& e) throw() : bad_dimension_(e.bad_dimension_){};
	const unsigned int getBadDimension() { return bad_dimension_; };
};

class RotationException : public std::exception {
	float bad_rotation_;

   public:
	RotationException(float rotation) : bad_rotation_(rotation){};
	RotationException(const RotationException& e) throw() : bad_rotation_(e.bad_rotation_){};
	const float getBadRotation() { return bad_rotation_; };
};

class SpriteOffsetException : public std::exception {
	float bad_offset_;

	public:
	SpriteOffsetException(float offset) : bad_offset_(offset) {};
	SpriteOffsetException(const SpriteOffsetException& e) throw() : bad_offset_(e.bad_offset_) {};
	const float getBadOffset() { return bad_offset_; };
};

struct playerSettings {
	bool isNone;
	bool isHuman;
	int level;
};

enum ActiveScreenType { None, MainMenu, GameMenu, MonopolyGame };

enum ScreenEventType {
	Idle,
	Exit,
	Play,
	ReturnToMainMenu,
	Player1SetNone,
	Player2SetNone,
	Player3SetNone,
	Player4SetNone,
	Player1SetHuman,
	Player2SetHuman,
	Player3SetHuman,
	Player4SetHuman,
	Player1SetAI,
	Player2SetAI,
	Player3SetAI,
	Player4SetAI,
	Player1SetAILevel1,
	Player2SetAILevel1,
	Player3SetAILevel1,
	Player4SetAILevel1,
	Player1SetAILevel2,
	Player2SetAILevel2,
	Player3SetAILevel2,
	Player4SetAILevel2,
	Player1SetAILevel3,
	Player2SetAILevel3,
	Player3SetAILevel3,
	Player4SetAILevel3,
	StartGame,
};

enum FieldType { STREET, STATION, UTILITY, GO, CHANCE, COMMUNITY_CHEST, TAX, JAIL, FREE_PARKING, GO_TO_JAIL };

enum StreetTiers { NO_HOUSES, ONE_HOUSE, TWO_HOUESES, THREE_HOUSES, FOUR_HOUSES, HOTEL };

enum StationTiers { ONE_STATION, TWO_STATIONS, THREE_STATIONS, FOUR_STATIONS };

enum UtilityTiers { ONE_UTILITY, TWO_UTILITIES };
