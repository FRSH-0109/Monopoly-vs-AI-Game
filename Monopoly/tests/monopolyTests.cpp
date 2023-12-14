/**
 * @file monopolyTests.cpp
 *
 * @brief Source file for project unit tests
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "../activeScreen.h"
#include "../contextWindow.h"
#include "../gameEngine.h"
#include "../monopolyGameEngine.h"
#include "catch2/src/catch2/catch_all.hpp"

using namespace std;

TEST_CASE("GameEngine class") {
	GameEngine testEngine = GameEngine(30, 1000, 1200);

	REQUIRE(testEngine.getWindowWidth() == 1000);
	REQUIRE(testEngine.getWindowHeight() == 1200);

	SECTION("GameEngine getContextWindow()") {
		REQUIRE(testEngine.getContextWindow() == ContextWindow::GetInstance());
	}
}

TEST_CASE("ContextWindow Singleton design pattern") {
	ContextWindow* contextWindow = ContextWindow::GetInstance();
	ContextWindow* newContextWindow = ContextWindow::GetInstance();

	REQUIRE(contextWindow == newContextWindow);
}

TEST_CASE("ActiveScreen classes") {
	MainMenuScreen mainMenu = MainMenuScreen();
	GameMenuScreen gameMenu = GameMenuScreen();

	REQUIRE(mainMenu.getScreenType() == MainMenu);
	REQUIRE(gameMenu.getScreenType() == GameMenu);

	SECTION("ActiveScreen getContextWindow") {
		ContextWindow* contextWindow = ContextWindow::GetInstance();
		mainMenu.setContextWindow(contextWindow);
		gameMenu.setContextWindow(contextWindow);

		REQUIRE(mainMenu.getContextWindow() == contextWindow);
		REQUIRE(gameMenu.getContextWindow() == contextWindow);
	}
}

TEST_CASE("monopolyGameEngine") {
	monopolyGameEngine monopolyEngine = monopolyGameEngine();
	REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
	REQUIRE(monopolyEngine.getPlayersAINumber() == 1);

	SECTION("monopolyGameEngine setPlayersHumanNumber()") {
		monopolyEngine.setPlayersHumanNumber(3);
		REQUIRE(monopolyEngine.getPlayersHumanNumber() == 3);
		REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
	}

	SECTION("monopolyGameEngine setPlayersAINumber()") {
		monopolyEngine.setPlayersAINumber(2);
		REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
		REQUIRE(monopolyEngine.getPlayersAINumber() == 2);
	}
}
