/**
 * @file monopolyTests.cpp
 *
 * @brief Source file for project unit tests
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include <fstream>
#include <map>
#include <string>
#include "../Board.h"
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

	SECTION("PropertyField class setters - basic scenario") {
		Player NEW_OWNER = Player();
		const unsigned int NEW_HOSUE_NUMBER = 3;
		const bool NEW_HOTEL_STATE = true;
		const bool NEW_MORTAGE_STATE = true;
		Player* OWNER_PTR = &NEW_OWNER;

		test_field.setHouseNumber(NEW_HOSUE_NUMBER);
		test_field.setIsHotel(NEW_HOTEL_STATE);
		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		REQUIRE(test_field.getHouseNumber() == NEW_HOSUE_NUMBER);
		REQUIRE(test_field.getIsHotel() == NEW_HOTEL_STATE);
		REQUIRE(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		REQUIRE(test_field.getOwner() == OWNER_PTR);

		test_field.setOwner(nullptr);

		REQUIRE(test_field.getOwner() == nullptr);
	}

	SECTION("PropertyField class setters - exception throws") {
		const unsigned int NEW_HOSUE_NUMBER = 7;

		REQUIRE_THROWS_AS(test_field.setHouseNumber(NEW_HOSUE_NUMBER), HouseException);
	}

	SECTION("PropertyField::resetDefault() method") {
		Player NEW_OWNER = Player();
		const unsigned int NEW_HOSUE_NUMBER = 3;
		const bool NEW_HOTEL_STATE = true;
		const bool NEW_MORTAGE_STATE = true;
		Player* OWNER_PTR = &NEW_OWNER;

		test_field.setHouseNumber(NEW_HOSUE_NUMBER);
		test_field.setIsHotel(NEW_HOTEL_STATE);
		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		CHECK(test_field.getHouseNumber() == NEW_HOSUE_NUMBER);
		CHECK(test_field.getIsHotel() == NEW_HOTEL_STATE);
		CHECK(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		CHECK(test_field.getOwner() == OWNER_PTR);

		test_field.resetDefault();

		REQUIRE(test_field.getHouseNumber() == 0);
		REQUIRE(test_field.getIsHotel() == false);
		REQUIRE(test_field.getIsMortaged() == false);
		REQUIRE(test_field.getOwner() == nullptr);
	}
}

TEST_CASE("Board class") {
	/*
	* TODO:
	Sprawić by zapewnić, żeby ścieżka zawsze działała
	*/
	std::string TEST_PATH = "Monopoly/tests/test_board.json";
	std::ifstream f(TEST_PATH);
	std::cout << "Testing path" << std::endl;
	std::cout << f.rdbuf();
	std::cout << "Finished testing path" << std::endl;
	Board TEST_BOARD = Board(TEST_PATH);
}

TEST_CASE("Player class") {
	unsigned int FIELD_ID_1 = 1;
	unsigned int FIELD_ID_2 = 2;
	unsigned int FIELD_ID_3 = 3;
	unsigned int FIELD_ID_4 = 4;
	unsigned int AMOUT_1 = 123;
	unsigned int AMOUT_2 = 23;
	unsigned int AMOUT_3 = 100;
	unsigned int POSITION_1 = 6;
	unsigned int POSITION_2 = 8;
	unsigned int JAILSTATUS_1 = 3;
	unsigned int JAILSTATUS_2 = 2;
	Player PLAYER1;
	Player PLAYER2(AMOUT_1);

	//MONEY TESTS
	REQUIRE(PLAYER1.getMoney() == 0);
	PLAYER1.addMoney(AMOUT_2);
	REQUIRE(PLAYER1.getMoney() == AMOUT_2);
	PLAYER1.addMoney(AMOUT_3);
	REQUIRE(PLAYER1.getMoney() == AMOUT_3 + AMOUT_2);
	REQUIRE(PLAYER1.substractMoney(AMOUT_1) == true);
	REQUIRE(PLAYER1.getMoney() == 0);
	REQUIRE(PLAYER1.substractMoney(AMOUT_1) == false);
	REQUIRE(PLAYER1.getMoney() == 0);
	REQUIRE(PLAYER2.getMoney() == AMOUT_1);

	//POSITION TESTS
	REQUIRE(PLAYER1.getPositon() == 0);	
	PLAYER1.setPositon(POSITION_1);	
	REQUIRE(PLAYER1.getPositon() == POSITION_1);
	PLAYER1.setPositon(POSITION_2);
	REQUIRE(PLAYER1.getPositon() == POSITION_2);

	//JAILSTATUS TESTS
	REQUIRE(PLAYER1.getJailStatus() == 0);
	PLAYER1.setJailStatus(JAILSTATUS_1);
	REQUIRE(PLAYER1.getJailStatus() == JAILSTATUS_1);
	PLAYER1.reduceJailStatus();
	REQUIRE(PLAYER1.getJailStatus() == JAILSTATUS_2);

	//FILED OWNED TESTS
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_1) == false);
	PLAYER1.addFieldOwnedId(FIELD_ID_1);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_1) == true);
	PLAYER1.addFieldOwnedId(FIELD_ID_2);
	PLAYER1.addFieldOwnedId(FIELD_ID_3);
	PLAYER1.addFieldOwnedId(FIELD_ID_4);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_1) == true);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_2) == true);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_3) == true);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_4) == true);
	PLAYER1.removeFiledOwnedId(FIELD_ID_1);
	PLAYER1.removeFiledOwnedId(FIELD_ID_3);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_1) == false);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_2) == true);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_3) == false);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_4) == true);
	PLAYER1.clearFiledOwnedId();
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_1) == false);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_2) == false);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_3) == false);
	REQUIRE(PLAYER1.hasFiledOwnedId(FIELD_ID_4) == false);
}
