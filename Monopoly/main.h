#ifndef MAIN_H
#define MAIN_H

enum ActiveScreenType { None, MainMenu, GameMenu, MonopolyGame };

enum ScreenEventType {
	Idle,
	Exit,
	Play,
	ReturnToMainMenu,
	AddPlayer,
	RemovePlayer,
	AddAIPlayer,
	RemoveAIPlayer,
	StartGame,
};

enum FieldType {
	PROPERTY,
	STATION,
	UTILITY,
	GO,
	CHANCE,
	COMMUNITY_CHEST,
	TAX,
	JAIL,
	FREE_PARKING,
	GO_TO_JAIL
};

enum PropertyTiers {
	NO_HOUESES,
	ONE_HOUSE,
	TWO_HOUESES,
	THREE_HOUSES,
	FOUR_HOUSES,
	HOTEL
};

#endif