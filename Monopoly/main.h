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

#endif