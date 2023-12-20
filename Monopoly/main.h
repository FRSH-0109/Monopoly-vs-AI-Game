#ifndef MAIN_H
#define MAIN_H

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

enum FieldType { PROPERTY, STATION, UTILITY, GO, CHANCE, COMMUNITY_CHEST, TAX, JAIL, FREE_PARKING, GO_TO_JAIL };

enum PropertyTiers { NO_HOUSES, ONE_HOUSE, TWO_HOUESES, THREE_HOUSES, FOUR_HOUSES, HOTEL };

#endif