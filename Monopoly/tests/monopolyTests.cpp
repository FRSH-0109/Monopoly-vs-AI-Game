/**
 * @file monopolyTests.cpp
 *
 * @brief Source file for project unit tests
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include <map>
#include <string>
#include "../Field.hpp"
#include "../activeScreen.h"
#include "../contextWindow.h"
#include "../gameEngine.h"
#include "../main.h"
#include "../monopolyGameEngine.h"
#include "catch2/src/catch2/catch_all.hpp"

using namespace std;

TEST_CASE("GameEngine class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	SECTION("GameEngine getContextWindow()") {
		REQUIRE(test_engine.getContextWindow() == ContextWindow::GetInstance());
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

	SECTION("monopolyGameEngine setPlayersHumanNumber() - illegal amount of players") {
		monopolyEngine.setPlayersHumanNumber(5);
		REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
		REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
	}

	SECTION("monopolyGameEngine setPlayersAINumber()") {
		monopolyEngine.setPlayersAINumber(2);
		REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
		REQUIRE(monopolyEngine.getPlayersAINumber() == 2);
	}

	SECTION("monopolyGameEngine setPlayersAINumber() - illegal amount of players") {
		monopolyEngine.setPlayersAINumber(5);
		REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
		REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
	}
}

TEST_CASE("PropertyField class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	const unsigned int TEST_ID = 1;
	const FieldType TEST_TYPE = PROPERTY;
	const std::string TEST_NAME = "Białystok";
	const std::string TEST_PATH = "./textures_and_fonts/textures/monopoly_single_square_purple.png";
	const unsigned int TEST_WIDTH = 200;
	const unsigned int TEST_HEIGHT = 1000;
	const float TEST_ROTATION = 0.0;
	const unsigned int TEST_PRICE = 400;
	const std::map<PropertyTiers, unsigned int> TEST_RENT = {{NO_HOUESES, 50}, {ONE_HOUSE, 200}, {TWO_HOUESES, 600},
		{THREE_HOUSES, 1400}, {FOUR_HOUSES, 1700}, {HOTEL, 2000}};
	const std::vector<unsigned int> TEST_GROUP_MEMBERS = {1};
	const unsigned int TEST_MORTAGE = 200;

	PropertyField test_field = PropertyField(TEST_ID, TEST_TYPE, TEST_NAME, TEST_PATH, TEST_WIDTH, TEST_HEIGHT,
		TEST_ROTATION, TEST_PRICE, TEST_RENT, TEST_GROUP_MEMBERS, TEST_MORTAGE);

	REQUIRE(test_field.getId() == TEST_ID);
	REQUIRE(test_field.getName() == TEST_NAME);
	REQUIRE(test_field.getGraphicPath() == TEST_PATH);
	REQUIRE(test_field.getWidth() == TEST_WIDTH);
	REQUIRE(test_field.getHeight() == TEST_HEIGHT);
	REQUIRE(test_field.getRotation() == TEST_ROTATION);
	REQUIRE(test_field.getPrice() == TEST_PRICE);
	REQUIRE(test_field.getRentValues() == TEST_RENT);
	REQUIRE(test_field.getGroupMembers() == TEST_GROUP_MEMBERS);
	REQUIRE(test_field.getMortage() == TEST_MORTAGE);
	REQUIRE(test_field.getHouseNumber() == 0);
	REQUIRE(test_field.getIsHotel() == false);
	REQUIRE(test_field.getIsMortaged() == false);
	REQUIRE(test_field.getUnmortageValue() == 220);
	REQUIRE(test_field.getOwner() == nullptr);

	SECTION("Field class setters - basic scenario") {
		const unsigned int NEW_TEST_WIDTH = 600;
		const unsigned int NEW_TEST_HEIGHT = 800;
		const float NEW_TEST_ROTATION = 45.9;

		test_field.setWidth(NEW_TEST_WIDTH, test_engine);
		test_field.setHeight(NEW_TEST_HEIGHT, test_engine);
		test_field.setRotation(NEW_TEST_ROTATION);

		REQUIRE(test_field.getWidth() == NEW_TEST_WIDTH);
		REQUIRE(test_field.getHeight() == NEW_TEST_HEIGHT);
		REQUIRE(test_field.getRotation() == NEW_TEST_ROTATION);
	}

	SECTION("Field class setters - exception throws") {
		const unsigned int NEW_TEST_WIDTH = 1200;
		const unsigned int NEW_TEST_HEIGHT = 0;
		const float NEW_TEST_ROTATION = -10.2;

		REQUIRE_THROWS_AS(test_field.setWidth(NEW_TEST_WIDTH, test_engine), DimensionException);
		REQUIRE_THROWS_AS(test_field.setHeight(NEW_TEST_HEIGHT, test_engine), DimensionException);
		REQUIRE_THROWS_AS(test_field.setRotation(NEW_TEST_ROTATION), RotationException);
	}
}
