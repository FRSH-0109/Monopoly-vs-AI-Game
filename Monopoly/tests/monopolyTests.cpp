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
#include "../Field.h"
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
	monopolyGameEngine monopoly_engine = monopolyGameEngine();
	monopoly_engine.createBoard();
	monopoly_engine.clearPlayers();

	playerSettings player_1_settings;
	player_1_settings.isNone = false;
	player_1_settings.isHuman = true;
	player_1_settings.level = 1;
	std::shared_ptr<playerSettings> player_1_settings_ptr = std::make_shared<playerSettings>(player_1_settings);

	playerSettings player_2_settings;
	player_2_settings.isNone = false;
	player_2_settings.isHuman = false;
	player_2_settings.level = 2;
	std::shared_ptr<playerSettings> player_2_settings_ptr = std::make_shared<playerSettings>(player_2_settings);

	playerSettings player_3_settings;
	player_3_settings.isNone = false;
	player_3_settings.isHuman = true;
	player_3_settings.level = 1;
	std::shared_ptr<playerSettings> player_3_settings_ptr = std::make_shared<playerSettings>(player_3_settings);

	std::vector<std::shared_ptr<playerSettings>> player_settings_list;
	player_settings_list.push_back(player_1_settings_ptr);
	player_settings_list.push_back(player_2_settings_ptr);
	player_settings_list.push_back(player_3_settings_ptr);
	monopoly_engine.createPlayers(player_settings_list);
	monopoly_engine.setPlayerIndexTurn(0);
	REQUIRE(monopoly_engine.getHouseCount() == 32);
	REQUIRE(monopoly_engine.getHotelCount() == 12);

	SECTION("setHouseCount() and setHotelCount() method") {
		monopoly_engine.setHouseCount(10);
		monopoly_engine.setHotelCount(8);
		CHECK(monopoly_engine.getHouseCount() == 10);
		CHECK(monopoly_engine.getHotelCount() == 8);
	}

	SECTION("substractHouses() and addHouses() methods") {
		monopoly_engine.substractHouses(3);
		CHECK(monopoly_engine.getHouseCount() == 29);

		monopoly_engine.addHouses(2);
		CHECK(monopoly_engine.getHouseCount() == 31);
	}

	SECTION("addHotels() and substractHotels() methods") {
		monopoly_engine.substractHotels(1);
		CHECK(monopoly_engine.getHotelCount() == 11);

		monopoly_engine.addHotels(1);
		CHECK(monopoly_engine.getHotelCount() == 12);
	}

	SECTION("groupCompleted() method") {
		SECTION("StreetField") {
			std::vector<unsigned int> PLAYER_FIELDS_1 = {1, 6, 8, 9, 15, 28, 37, 39};
			std::vector<unsigned int> PLAYER_FIELDS_2 = {1, 3, 9, 15, 28, 37, 39};
			StreetField& test_field_8 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(8));
			REQUIRE(monopoly_engine.groupCompleted(PLAYER_FIELDS_1, test_field_8) == true);
			REQUIRE(monopoly_engine.groupCompleted(PLAYER_FIELDS_2, test_field_8) == false);
		}

		SECTION("StationField") {
			std::vector<unsigned int> PLAYER_FIELDS_1 = {1, 5, 9, 15, 25, 28, 35, 37, 39};
			std::vector<unsigned int> PLAYER_FIELDS_2 = {1, 3, 9, 15, 23, 28, 37, 39};
			StationField& test_field_35 = std::get<StationField>(monopoly_engine.getBoard()->getFieldById(35));
			REQUIRE(monopoly_engine.groupCompleted(PLAYER_FIELDS_1, test_field_35) == true);
			REQUIRE(monopoly_engine.groupCompleted(PLAYER_FIELDS_2, test_field_35) == false);
		}

		SECTION("UtilityField") {
			std::vector<unsigned int> PLAYER_FIELDS_1 = {1, 5, 9, 12, 25, 28, 37, 39};
			std::vector<unsigned int> PLAYER_FIELDS_2 = {1, 3, 9, 15, 23, 37, 39};
			UtilityField& test_field_28 = std::get<UtilityField>(monopoly_engine.getBoard()->getFieldById(28));
			REQUIRE(monopoly_engine.groupCompleted(PLAYER_FIELDS_1, test_field_28) == true);
			REQUIRE(monopoly_engine.groupCompleted(PLAYER_FIELDS_2, test_field_28) == false);
		}
	}

	SECTION("calculateGroupFieldOwned() method") {
		SECTION("StreetField") {
			std::vector<unsigned int> PLAYER_FIELDS_1 = {1, 6, 8, 9, 15, 28, 37, 39};
			std::vector<unsigned int> PLAYER_FIELDS_2 = {1, 3, 9, 15, 28, 37, 39};
			StreetField& test_field_8 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(8));
			REQUIRE(monopoly_engine.calculateGroupFieldsOwned(PLAYER_FIELDS_1, test_field_8) == 3);
			REQUIRE(monopoly_engine.calculateGroupFieldsOwned(PLAYER_FIELDS_2, test_field_8) == 2);
		}

		SECTION("StationField") {
			std::vector<unsigned int> PLAYER_FIELDS_1 = {1, 5, 9, 15, 25, 28, 35, 37, 39};
			std::vector<unsigned int> PLAYER_FIELDS_2 = {1, 3, 9, 15, 23, 28, 35, 37, 39};
			StationField& test_field_35 = std::get<StationField>(monopoly_engine.getBoard()->getFieldById(35));
			REQUIRE(monopoly_engine.calculateGroupFieldsOwned(PLAYER_FIELDS_1, test_field_35) == 4);
			REQUIRE(monopoly_engine.calculateGroupFieldsOwned(PLAYER_FIELDS_2, test_field_35) == 2);
		}

		SECTION("UtilityField") {
			std::vector<unsigned int> PLAYER_FIELDS_1 = {1, 5, 9, 12, 25, 28, 37, 39};
			std::vector<unsigned int> PLAYER_FIELDS_2 = {1, 3, 9, 15, 23, 28, 37, 39};
			UtilityField& test_field_28 = std::get<UtilityField>(monopoly_engine.getBoard()->getFieldById(28));
			REQUIRE(monopoly_engine.calculateGroupFieldsOwned(PLAYER_FIELDS_1, test_field_28) == 2);
			REQUIRE(monopoly_engine.calculateGroupFieldsOwned(PLAYER_FIELDS_2, test_field_28) == 1);
		}
	}

	SECTION("calculateRent() method") {
		SECTION("Street is mortaged") {
			std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(3)).setIsMortaged(true);
			unsigned int expected_rent = 0;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 3);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Street with Hotel case") {
			std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(3)).setIsHotel(true);
			unsigned int expected_rent = 450;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 3);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Street with Houses") {
			StreetField& test_field_3 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(3));
			test_field_3.setOwner(monopoly_engine.getPlayers()[0]);
			monopoly_engine.getPlayers()[0]->addFieldOwnedId(3);
			test_field_3.setHouseNumber(3);
			unsigned int expected_rent = 180;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 3);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Street without houses in a full group house") {
			StreetField& test_field_1 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(1));
			test_field_1.setOwner(monopoly_engine.getPlayers()[0]);
			monopoly_engine.getPlayers()[0]->addFieldOwnedId(1);
			StreetField& test_field_3 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(3));
			test_field_3.setOwner(monopoly_engine.getPlayers()[0]);
			monopoly_engine.getPlayers()[0]->addFieldOwnedId(3);
			unsigned int expected_rent = 8;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 3);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Street without a full group house") {
			StreetField& test_field_1 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(1));
			test_field_1.setOwner(monopoly_engine.getPlayers()[2]);
			monopoly_engine.getPlayers()[2]->addFieldOwnedId(1);
			StreetField& test_field_3 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(3));
			test_field_3.setOwner(monopoly_engine.getPlayers()[0]);
			monopoly_engine.getPlayers()[0]->addFieldOwnedId(3);
			unsigned int expected_rent = 4;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 3);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Station is mortaged") {
			std::get<StationField>(monopoly_engine.getBoard()->getFieldById(15)).setIsMortaged(true);
			unsigned int expected_rent = 0;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 15);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("1 station") {
			StationField& test_field_15 = std::get<StationField>(monopoly_engine.getBoard()->getFieldById(15));
			test_field_15.setOwner(monopoly_engine.getPlayers()[0]);
			monopoly_engine.getPlayers()[0]->addFieldOwnedId(15);
			unsigned int expected_rent = 25;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 15);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Multiple stations") {
			StationField& test_field_15 = std::get<StationField>(monopoly_engine.getBoard()->getFieldById(15));
			std::shared_ptr<Player> fields_owner = monopoly_engine.getPlayers()[0];
			test_field_15.setOwner(fields_owner);
			fields_owner->addFieldOwnedId(15);
			StationField& test_field_25 = std::get<StationField>(monopoly_engine.getBoard()->getFieldById(25));
			test_field_25.setOwner(fields_owner);
			fields_owner->addFieldOwnedId(25);
			StationField& test_field_35 = std::get<StationField>(monopoly_engine.getBoard()->getFieldById(35));
			test_field_35.setOwner(fields_owner);
			fields_owner->addFieldOwnedId(35);
			unsigned int expected_rent = 100;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 15);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("Utility is mortaged") {
			std::get<UtilityField>(monopoly_engine.getBoard()->getFieldById(12)).setIsMortaged(true);
			unsigned int expected_rent = 0;
			unsigned int calculated_rent = monopoly_engine.calculateRent(7, 12);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("1 Utility owned") {
			UtilityField& test_field_12 = std::get<UtilityField>(monopoly_engine.getBoard()->getFieldById(12));
			test_field_12.setOwner(monopoly_engine.getPlayers()[0]);
			monopoly_engine.getPlayers()[0]->addFieldOwnedId(12);
			const unsigned int DICE_ROLL = 7;
			unsigned int expected_rent = 4 * DICE_ROLL;
			unsigned int calculated_rent = monopoly_engine.calculateRent(DICE_ROLL, 12);
			CHECK(calculated_rent == expected_rent);
		}

		SECTION("2 Utilities owned") {
			UtilityField& test_field_12 = std::get<UtilityField>(monopoly_engine.getBoard()->getFieldById(12));
			std::shared_ptr<Player> fields_owner = monopoly_engine.getPlayers()[0];
			test_field_12.setOwner(fields_owner);
			fields_owner->addFieldOwnedId(12);
			UtilityField& test_field_28 = std::get<UtilityField>(monopoly_engine.getBoard()->getFieldById(28));
			test_field_28.setOwner(fields_owner);
			fields_owner->addFieldOwnedId(28);
			const unsigned int DICE_ROLL = 4;
			unsigned int expected_rent = 10 * DICE_ROLL;
			unsigned int calculated_rent = monopoly_engine.calculateRent(DICE_ROLL, 12);
			CHECK(calculated_rent == expected_rent);
		}
	}

	SECTION("isBuildingLegal() method") {
		StreetField& test_field_6 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(6));
		StreetField& test_field_8 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(8));
		StreetField& test_field_9 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(9));
		std::shared_ptr<Player> builder = monopoly_engine.getPlayers()[0];
		std::shared_ptr<Player> other_player = monopoly_engine.getPlayers()[2];

		SECTION("Group not completed") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(other_player);
			other_player->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
			std::vector<unsigned int> other_ownes = other_player->getFiledOwnedId();

			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isBuildingLegal(other_player, test_field_9) == false);
		}

		SECTION("Property is mortaged") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setIsMortaged(true);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_9) == false);
		}

		SECTION("Houses are uneven") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setHouseNumber(2);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(3);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(2);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_6) == true);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_9) == true);
		}

		SECTION("Houses are even") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setHouseNumber(2);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(2);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(2);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_6) == true);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_8) == true);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_9) == true);
		}

		SECTION("All properties have full number of houses") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setHouseNumber(4);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(4);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(4);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_9) == false);
		}

		SECTION("Hosue limit is reached") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setHouseNumber(2);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(2);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(2);

			monopoly_engine.setHouseCount(0);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isBuildingLegal(builder, test_field_9) == false);
		}
	}

	SECTION("isDestroyingLegal() method") {
		StreetField& test_field_6 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(6));
		StreetField& test_field_8 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(8));
		StreetField& test_field_9 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(9));
		std::shared_ptr<Player> builder = monopoly_engine.getPlayers()[0];
		std::shared_ptr<Player> other_player = monopoly_engine.getPlayers()[2];

		SECTION("Group color not completed") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(other_player);
			other_player->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
			std::vector<unsigned int> other_ownes = other_player->getFiledOwnedId();

			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isDestroyingLegal(other_player, test_field_9) == false);
		}

		SECTION("Building is mortaged") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setIsMortaged(true);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(1);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(1);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_9) == false);
		}

		SECTION("Property doesn't have building") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_9) == false);
		}

		SECTION("Buildings are uneven") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setHouseNumber(2);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(2);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(1);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_6) == true);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_8) == true);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_9) == false);
		}

		SECTION("Buildings are even") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setHouseNumber(3);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(3);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(3);

			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_6) == true);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_8) == true);
			CHECK(monopoly_engine.isDestroyingLegal(builder, test_field_9) == true);
		}
	}

	SECTION("isHotelBuildingLegal() method") {
		StreetField& test_field_6 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(6));
		StreetField& test_field_8 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(8));
		StreetField& test_field_9 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(9));
		std::shared_ptr<Player> builder = monopoly_engine.getPlayers()[0];
		std::shared_ptr<Player> other_player = monopoly_engine.getPlayers()[2];

		SECTION("Group color not completed") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(other_player);
			other_player->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
			std::vector<unsigned int> other_ownes = other_player->getFiledOwnedId();

			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelBuildingLegal(other_player, test_field_9) == false);
		}

		SECTION("Property is Mortaged") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_9) == false);
		}

		SECTION("No hotels in a bank") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setIsHotel(true);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(4);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(4);

			monopoly_engine.setHouseCount(0);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
			std::vector<unsigned int> other_ownes = other_player->getFiledOwnedId();

			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_8) == true);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_9) == true);
		}

		SECTION("Passing case") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setIsHotel(true);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(4);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setHouseNumber(4);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
			std::vector<unsigned int> other_ownes = other_player->getFiledOwnedId();

			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_8) == true);
			CHECK(monopoly_engine.isHotelBuildingLegal(builder, test_field_9) == true);
		}
	}

	SECTION("isHotelDestroyingLegal() method") {
		StreetField& test_field_6 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(6));
		StreetField& test_field_8 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(8));
		StreetField& test_field_9 = std::get<StreetField>(monopoly_engine.getBoard()->getFieldById(9));
		std::shared_ptr<Player> builder = monopoly_engine.getPlayers()[0];
		std::shared_ptr<Player> other_player = monopoly_engine.getPlayers()[2];

		SECTION("Group color not completed") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(other_player);
			other_player->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();
			std::vector<unsigned int> other_ownes = other_player->getFiledOwnedId();

			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(other_player, test_field_9) == false);
		}

		SECTION("Street is mortaged") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);

			std::vector<unsigned int> builder_ownes = builder->getFiledOwnedId();

			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_9) == false);
		}

		SECTION("Not enough houses in a bank") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setIsHotel(true);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(4);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setIsHotel(true);

			monopoly_engine.setHouseCount(0);

			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_9) == false);

			monopoly_engine.setHouseCount(3);

			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_6) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_9) == false);
		}

		SECTION("Passing case") {
			test_field_6.setOwner(builder);
			builder->addFieldOwnedId(6);
			test_field_6.setIsHotel(true);

			test_field_8.setOwner(builder);
			builder->addFieldOwnedId(8);
			test_field_8.setHouseNumber(4);

			test_field_9.setOwner(builder);
			builder->addFieldOwnedId(9);
			test_field_9.setIsHotel(true);

			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_6) == true);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_8) == false);
			CHECK(monopoly_engine.isHotelDestroyingLegal(builder, test_field_9) == true);
		}
	}
	// REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
	// REQUIRE(monopolyEngine.getPlayersAINumber() == 1);

	// SECTION("monopolyGameEngine setPlayersHumanNumber()") {
	// 	monopolyEngine.setPlayersHumanNumber(3);
	// 	REQUIRE(monopolyEngine.getPlayersHumanNumber() == 3);
	// 	REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
	// }

	// SECTION("monopolyGameEngine setPlayersHumanNumber() - illegal amount of players") {
	// 	monopolyEngine.setPlayersHumanNumber(5);
	// 	REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
	// 	REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
	// }

	// SECTION("monopolyGameEngine setPlayersAINumber()") {
	// 	monopolyEngine.setPlayersAINumber(2);
	// 	REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
	// 	REQUIRE(monopolyEngine.getPlayersAINumber() == 2);
	// }

	// SECTION("monopolyGameEngine setPlayersAINumber() - illegal amount of players") {
	// 	monopolyEngine.setPlayersAINumber(5);
	// 	REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
	// 	REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
	// }
}

TEST_CASE("PropertyField class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	const unsigned int TEST_ID = 1;
	const FieldType TEST_TYPE = STREET;
	const std::string TEST_NAME = "Białystok";
	const std::string TEST_PATH = "./textures_and_fonts/textures/monopoly_single_square_purple.png";
	const unsigned int TEST_WIDTH = 200;
	const unsigned int TEST_HEIGHT = 1000;
	const float TEST_ROTATION = 0.0;
	const sf::Vector2i TEST_POSITION = sf::Vector2i(20, 20);
	const unsigned int TEST_PRICE = 400;
	const std::vector<unsigned int> TEST_GROUP_MEMBERS = {1};
	const unsigned int TEST_MORTAGE = 200;

	PropertyField test_field(TEST_ID, TEST_TYPE, TEST_NAME, TEST_PATH, TEST_WIDTH, TEST_HEIGHT, TEST_ROTATION,
		TEST_POSITION, TEST_PRICE, TEST_GROUP_MEMBERS, TEST_MORTAGE);

	REQUIRE(test_field.getId() == TEST_ID);
	REQUIRE(test_field.getType() == TEST_TYPE);
	REQUIRE(test_field.getName() == TEST_NAME);
	REQUIRE(test_field.getGraphicPath() == TEST_PATH);
	REQUIRE(test_field.getWidth() == TEST_WIDTH);
	REQUIRE(test_field.getHeight() == TEST_HEIGHT);
	REQUIRE(test_field.getRotation() == TEST_ROTATION);
	REQUIRE(test_field.getPrice() == TEST_PRICE);
	REQUIRE(test_field.getGroupMembers() == TEST_GROUP_MEMBERS);
	REQUIRE(test_field.getMortage() == TEST_MORTAGE);
	REQUIRE(test_field.getIsMortaged() == false);
	REQUIRE(test_field.getUnmortageValue() == 220);
	REQUIRE(test_field.getOwner() == nullptr);

	SECTION("PropertyField setters") {
		Player NEW_OWNER = Player();
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		REQUIRE(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		REQUIRE(test_field.getOwner() == OWNER_PTR);

		test_field.setOwner(nullptr);

		REQUIRE(test_field.getOwner() == nullptr);
	}

	SECTION("PropertyField::resetDefault() method") {
		Player NEW_OWNER = Player();
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		CHECK(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		CHECK(test_field.getOwner() == OWNER_PTR);

		test_field.resetDefault();

		REQUIRE(test_field.getIsMortaged() == false);
		REQUIRE(test_field.getOwner() == nullptr);
	}
}

TEST_CASE("StreetField class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	const unsigned int TEST_ID = 1;
	const FieldType TEST_TYPE = STREET;
	const std::string TEST_NAME = "Białystok";
	const std::string TEST_PATH = "./textures_and_fonts/textures/monopoly_single_square_purple.png";
	const unsigned int TEST_WIDTH = 200;
	const unsigned int TEST_HEIGHT = 1000;
	const float TEST_ROTATION = 0.0;
	const sf::Vector2i TEST_POSITION = sf::Vector2i(20, 20);
	const unsigned int TEST_PRICE = 400;
	const unsigned int TEST_HOUSE_PRICE = 200;
	const unsigned int TEST_HOTEL_PRICE = 250;
	const std::map<StreetTiers, unsigned int> TEST_RENT = {{NO_HOUSES, 50}, {ONE_HOUSE, 200}, {TWO_HOUESES, 600},
		{THREE_HOUSES, 1400}, {FOUR_HOUSES, 1700}, {HOTEL, 2000}};
	const std::vector<unsigned int> TEST_GROUP_MEMBERS = {1};
	const unsigned int TEST_MORTAGE = 200;

	StreetField test_field(TEST_ID, TEST_TYPE, TEST_NAME, TEST_PATH, TEST_WIDTH, TEST_HEIGHT, TEST_ROTATION,
		TEST_POSITION, TEST_PRICE, TEST_HOUSE_PRICE, TEST_HOTEL_PRICE, TEST_RENT, TEST_GROUP_MEMBERS, TEST_MORTAGE);

	REQUIRE(test_field.getId() == TEST_ID);
	REQUIRE(test_field.getType() == TEST_TYPE);
	REQUIRE(test_field.getName() == TEST_NAME);
	REQUIRE(test_field.getGraphicPath() == TEST_PATH);
	REQUIRE(test_field.getWidth() == TEST_WIDTH);
	REQUIRE(test_field.getHeight() == TEST_HEIGHT);
	REQUIRE(test_field.getRotation() == TEST_ROTATION);
	REQUIRE(test_field.getPrice() == TEST_PRICE);
	REQUIRE(test_field.getHousePrice() == TEST_HOUSE_PRICE);
	REQUIRE(test_field.getHotelPrice() == TEST_HOTEL_PRICE);
	REQUIRE(test_field.getRentValues() == TEST_RENT);
	REQUIRE(test_field.getGroupMembers() == TEST_GROUP_MEMBERS);
	REQUIRE(test_field.getMortage() == TEST_MORTAGE);
	REQUIRE(test_field.getHouseNumber() == 0);
	REQUIRE(test_field.getIsHotel() == false);
	REQUIRE(test_field.getIsMortaged() == false);
	REQUIRE(test_field.getUnmortageValue() == 220);
	REQUIRE(test_field.getOwner() == nullptr);

	test_field.getContextWindow();

	SECTION("Field class setters - basic scenario") {
		const unsigned int NEW_TEST_WIDTH = 600;
		const unsigned int NEW_TEST_HEIGHT = 800;
		const float NEW_TEST_ROTATION = 45.9;

		test_field.setWidth(NEW_TEST_WIDTH);
		test_field.setHeight(NEW_TEST_HEIGHT);
		test_field.setRotation(NEW_TEST_ROTATION);

		CHECK(test_field.getWidth() == NEW_TEST_WIDTH);
		CHECK(test_field.getHeight() == NEW_TEST_HEIGHT);
		CHECK(test_field.getRotation() == NEW_TEST_ROTATION);
	}

	SECTION("Field class setters - exception throws") {
		const unsigned int NEW_TEST_WIDTH = 1200;
		const unsigned int NEW_TEST_HEIGHT = 0;
		const float NEW_TEST_ROTATION = -10.2;

		CHECK_THROWS_AS(test_field.setWidth(NEW_TEST_WIDTH), DimensionException);
		CHECK_THROWS_AS(test_field.setHeight(NEW_TEST_HEIGHT), DimensionException);
		CHECK_THROWS_AS(test_field.setRotation(NEW_TEST_ROTATION), RotationException);
	}

	SECTION("StreetField class setters - basic scenario") {
		Player NEW_OWNER = Player();
		const unsigned int NEW_HOSUE_NUMBER = 3;
		const bool NEW_HOTEL_STATE = true;
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

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

	SECTION("StreetField class setters - exception throws") {
		const unsigned int NEW_HOSUE_NUMBER = 7;

		REQUIRE_THROWS_AS(test_field.setHouseNumber(NEW_HOSUE_NUMBER), HouseException);
	}

	SECTION("StreetField::resetDefault() method") {
		Player NEW_OWNER = Player();
		const unsigned int NEW_HOSUE_NUMBER = 3;
		const bool NEW_HOTEL_STATE = true;
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

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

TEST_CASE("StationField class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	const unsigned int TEST_ID = 1;
	const FieldType TEST_TYPE = STREET;
	const std::string TEST_NAME = "Dworzec Centralny";
	const std::string TEST_PATH = "./textures_and_fonts/textures/monopoly_single_square_purple.png";
	const unsigned int TEST_WIDTH = 200;
	const unsigned int TEST_HEIGHT = 1000;
	const float TEST_ROTATION = 0.0;
	sf::Vector2i TEST_POSITION = sf::Vector2i(20, 20);
	const unsigned int TEST_PRICE = 200;
	const std::map<StationTiers, unsigned int> TEST_RENT = {
		{ONE_STATION, 25}, {TWO_STATIONS, 50}, {THREE_STATIONS, 100}, {FOUR_STATIONS, 200}};
	const std::vector<unsigned int> TEST_GROUP_MEMBERS = {15, 25, 35};
	const unsigned int TEST_MORTAGE = 100;

	StationField test_field(TEST_ID, TEST_TYPE, TEST_NAME, TEST_PATH, TEST_WIDTH, TEST_HEIGHT, TEST_ROTATION,
		TEST_POSITION, TEST_PRICE, TEST_RENT, TEST_GROUP_MEMBERS, TEST_MORTAGE);

	REQUIRE(test_field.getId() == TEST_ID);
	REQUIRE(test_field.getType() == TEST_TYPE);
	REQUIRE(test_field.getName() == TEST_NAME);
	REQUIRE(test_field.getGraphicPath() == TEST_PATH);
	REQUIRE(test_field.getWidth() == TEST_WIDTH);
	REQUIRE(test_field.getHeight() == TEST_HEIGHT);
	REQUIRE(test_field.getRotation() == TEST_ROTATION);
	REQUIRE(test_field.getPrice() == TEST_PRICE);
	REQUIRE(test_field.getRentValues() == TEST_RENT);
	REQUIRE(test_field.getGroupMembers() == TEST_GROUP_MEMBERS);
	REQUIRE(test_field.getMortage() == TEST_MORTAGE);
	REQUIRE(test_field.getIsMortaged() == false);
	REQUIRE(test_field.getUnmortageValue() == 110);
	REQUIRE(test_field.getOwner() == nullptr);

	SECTION("StationField setters") {
		Player NEW_OWNER = Player();
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		REQUIRE(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		REQUIRE(test_field.getOwner() == OWNER_PTR);

		test_field.setOwner(nullptr);

		REQUIRE(test_field.getOwner() == nullptr);
	}

	SECTION("StationField::resetDefault() method") {
		Player NEW_OWNER = Player();
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		CHECK(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		CHECK(test_field.getOwner() == OWNER_PTR);

		test_field.resetDefault();

		REQUIRE(test_field.getIsMortaged() == false);
		REQUIRE(test_field.getOwner() == nullptr);
	}
}

TEST_CASE("Utility class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	const unsigned int TEST_ID = 1;
	const FieldType TEST_TYPE = STREET;
	const std::string TEST_NAME = "Elektrownia";
	const std::string TEST_PATH = "./textures_and_fonts/textures/monopoly_single_square_purple.png";
	const unsigned int TEST_WIDTH = 200;
	const unsigned int TEST_HEIGHT = 1000;
	const float TEST_ROTATION = 0.0;
	sf::Vector2i TEST_POSITION = sf::Vector2i(20, 20);
	const unsigned int TEST_PRICE = 150;
	const std::map<UtilityTiers, unsigned int> TEST_RENT = {{ONE_UTILITY, 4}, {TWO_UTILITIES, 10}};
	const std::vector<unsigned int> TEST_GROUP_MEMBERS = {28};
	const unsigned int TEST_MORTAGE = 75;

	UtilityField test_field(TEST_ID, TEST_TYPE, TEST_NAME, TEST_PATH, TEST_WIDTH, TEST_HEIGHT, TEST_ROTATION,
		TEST_POSITION, TEST_PRICE, TEST_RENT, TEST_GROUP_MEMBERS, TEST_MORTAGE);

	REQUIRE(test_field.getId() == TEST_ID);
	REQUIRE(test_field.getType() == TEST_TYPE);
	REQUIRE(test_field.getName() == TEST_NAME);
	REQUIRE(test_field.getGraphicPath() == TEST_PATH);
	REQUIRE(test_field.getWidth() == TEST_WIDTH);
	REQUIRE(test_field.getHeight() == TEST_HEIGHT);
	REQUIRE(test_field.getRotation() == TEST_ROTATION);
	REQUIRE(test_field.getPrice() == TEST_PRICE);
	REQUIRE(test_field.getRentMultipliers() == TEST_RENT);
	REQUIRE(test_field.getGroupMembers() == TEST_GROUP_MEMBERS);
	REQUIRE(test_field.getMortage() == TEST_MORTAGE);
	REQUIRE(test_field.getIsMortaged() == false);
	REQUIRE(test_field.getUnmortageValue() == 83);
	REQUIRE(test_field.getOwner() == nullptr);

	SECTION("UtilityField class setters") {
		Player NEW_OWNER = Player();
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		REQUIRE(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		REQUIRE(test_field.getOwner() == OWNER_PTR);

		test_field.setOwner(nullptr);

		REQUIRE(test_field.getOwner() == nullptr);
	}

	SECTION("UtilityField::resetDefault() method") {
		Player NEW_OWNER = Player();
		const bool NEW_MORTAGE_STATE = true;
		std::shared_ptr<Player> OWNER_PTR = std::make_shared<Player>(NEW_OWNER);

		test_field.setIsMortaged(NEW_MORTAGE_STATE);
		test_field.setOwner(OWNER_PTR);

		CHECK(test_field.getIsMortaged() == NEW_MORTAGE_STATE);
		CHECK(test_field.getOwner() == OWNER_PTR);

		test_field.resetDefault();

		REQUIRE(test_field.getIsMortaged() == false);
		REQUIRE(test_field.getOwner() == nullptr);
	}
}

TEST_CASE("TaxField class") {
	GameEngine test_engine = GameEngine(30, 1000, 1200);

	REQUIRE(test_engine.getWindowWidth() == 1000);
	REQUIRE(test_engine.getWindowHeight() == 1200);

	const unsigned int TEST_ID = 2;
	const FieldType TEST_TYPE = TAX;
	const std::string TEST_NAME = "Domiar Podatkowy";
	const std::string TEST_PATH = "./textures_and_fonts/textures/monopoly_single_square_empty.png";
	const unsigned int TEST_WIDTH = 200;
	const unsigned int TEST_HEIGHT = 1000;
	const float TEST_ROTATION = 0.0;
	sf::Vector2i TEST_POSITION = sf::Vector2i(20, 20);
	const unsigned int TEST_TAX_VALUE = 100;

	TaxField test_tax_field = TaxField(TEST_ID, TEST_TYPE, TEST_NAME, TEST_PATH, TEST_WIDTH, TEST_HEIGHT, TEST_ROTATION,
		TEST_POSITION, TEST_TAX_VALUE);

	REQUIRE(test_tax_field.getId() == TEST_ID);
	REQUIRE(test_tax_field.getType() == TEST_TYPE);
	REQUIRE(test_tax_field.getName() == TEST_NAME);
	REQUIRE(test_tax_field.getGraphicPath() == TEST_PATH);
	REQUIRE(test_tax_field.getWidth() == TEST_WIDTH);
	REQUIRE(test_tax_field.getHeight() == TEST_HEIGHT);
	REQUIRE(test_tax_field.getRotation() == TEST_ROTATION);
	REQUIRE(test_tax_field.getTaxValue() == TEST_TAX_VALUE);
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

	// MONEY TESTS
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

	// POSITION TESTS
	REQUIRE(PLAYER1.getPosition() == 0);
	PLAYER1.setPosition(POSITION_1);
	REQUIRE(PLAYER1.getPosition() == POSITION_1);
	PLAYER1.setPosition(POSITION_2);
	REQUIRE(PLAYER1.getPosition() == POSITION_2);

	// JAILSTATUS TESTS
	REQUIRE(PLAYER1.getJailStatus() == 0);
	PLAYER1.setJailStatus(JAILSTATUS_1);
	REQUIRE(PLAYER1.getJailStatus() == JAILSTATUS_1);
	PLAYER1.reduceJailStatus();
	REQUIRE(PLAYER1.getJailStatus() == JAILSTATUS_2);

	// FILED OWNED TESTS
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

	SECTION("setSpritePositionX() and setSpritePositionY() - correct data") {
		PLAYER1.setSpriteOffsetX(0.5);
		PLAYER1.setSpriteOffsetY(0.25);

		REQUIRE(PLAYER1.getSpriteOffsetX() == 0.5);
		REQUIRE(PLAYER1.getSpriteOffsetY() == 0.25);
	}

	SECTION("setSpritePositionX() and setSpritePositionY() - wrong data") {
		REQUIRE_THROWS_AS(PLAYER1.setSpriteOffsetX(-0.4), SpriteOffsetException);
		REQUIRE_THROWS_AS(PLAYER1.setSpriteOffsetX(25.1), SpriteOffsetException);
		REQUIRE_THROWS_AS(PLAYER1.setSpriteOffsetY(1.2), SpriteOffsetException);
		REQUIRE_THROWS_AS(PLAYER1.setSpriteOffsetY(-0.5), SpriteOffsetException);
	}
}

TEST_CASE("Board class") {
	/*
	* TODO:
	Sprawić by zapewnić, żeby ścieżka zawsze działała
	*/
	std::vector<PossibleFields> test_board;

	unsigned int test_id = 0;
	FieldType test_type = GO;
	std::string test_name = "Start";
	std::string test_path = "textures_and_fonts/textures/monopoly_single_square_clear.png";
	unsigned int test_width = 200;
	unsigned int test_height = 200;
	float test_rotation = 0.0;
	sf::Vector2i test_position = sf::Vector2i(20, 20);

	const Field test_field =
		Field(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position);
	test_board.push_back(test_field);

	test_id = 1;
	test_type = STREET;
	test_name = "Świętochłowice";
	test_path = "textures_and_fonts/textures/monopoly_single_square_brown.png";
	test_width = 80;
	test_height = 200;
	test_rotation = 0.0;
	test_position = sf::Vector2i(40, 40);
	unsigned int test_price = 60;
	unsigned int test_house_price = 50;
	unsigned int test_hotel_price = 70;
	std::map<StreetTiers, unsigned int> test_rent = {
		{NO_HOUSES, 2}, {ONE_HOUSE, 10}, {TWO_HOUESES, 30}, {THREE_HOUSES, 90}, {FOUR_HOUSES, 160}, {HOTEL, 250}};
	std::vector<unsigned int> test_group_members = {3};
	unsigned int test_mortage = 30;

	const StreetField test_field_1 =
		StreetField(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position,
			test_price, test_house_price, test_hotel_price, test_rent, test_group_members, test_mortage);

	test_board.push_back(test_field_1);

	test_id = 2;
	test_type = COMMUNITY_CHEST;
	test_name = "Kasa Społeczna";
	test_width = 80;
	test_height = 200;
	test_rotation = 0.0;
	test_position = sf::Vector2i(60, 60);

	const Field test_field_2 =
		Field(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position);

	test_board.push_back(test_field_2);

	test_id = 3;
	test_type = STREET;
	test_name = "Bełchatów";
	test_path = "textures_and_fonts/textures/monopoly_single_square_brown.png";
	test_width = 80;
	test_height = 200;
	test_rotation = 0.0;
	test_price = 60;
	test_house_price = 50;
	test_hotel_price = 70;
	test_rent = {
		{NO_HOUSES, 4}, {ONE_HOUSE, 20}, {TWO_HOUESES, 60}, {THREE_HOUSES, 180}, {FOUR_HOUSES, 320}, {HOTEL, 450}};
	test_group_members = {1};
	test_mortage = 30;
	test_position = sf::Vector2i(80, 80);

	StreetField test_field_3 =
		StreetField(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position,
			test_price, test_house_price, test_hotel_price, test_rent, test_group_members, test_mortage);

	test_board.push_back(test_field_3);

	test_id = 4;
	test_type = TAX;
	test_name = "Podatek Dochodowy";
	test_path = "textures_and_fonts/textures/monopoly_single_square_empty.png";
	test_width = 80;
	test_height = 200;
	test_rotation = 0.0;
	test_position = sf::Vector2i(100, 100);
	unsigned int test_tax_value = 200;

	const TaxField test_field_4 = TaxField(test_id, test_type, test_name, test_path, test_width, test_height,
		test_rotation, test_position, test_tax_value);
	test_board.push_back(test_field_4);

	test_id = 5;
	test_type = STATION;
	test_name = "Dworzec Centralny";
	test_path = "textures_and_fonts/textures/monopoly_single_square_purple.png";
	test_width = 80;
	test_height = 200;
	test_rotation = 0.0;
	test_position = sf::Vector2i(120, 120);
	test_price = 200;
	std::map<StationTiers, unsigned int> test_rent_station = {
		{ONE_STATION, 25}, {TWO_STATIONS, 50}, {THREE_STATIONS, 100}, {FOUR_STATIONS, 200}};
	test_group_members = {15, 25, 35};
	test_mortage = 100;

	const StationField test_field_5 = StationField(test_id, test_type, test_name, test_path, test_width, test_height,
		test_rotation, test_position, test_price, test_rent_station, test_group_members, test_mortage);

	test_board.push_back(test_field_5);

	test_id = 7;
	test_type = CHANCE;
	test_name = "Szansa";
	test_path = "textures_and_fonts/textures/monopoly_single_square_clear.png";
	test_width = 80;
	test_height = 200;
	test_rotation = 0.0;
	test_position = sf::Vector2i(140, 140);

	const Field test_field_6 =
		Field(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position);

	test_board.push_back(test_field_6);

	test_id = 10;
	test_type = JAIL;
	test_name = "Więzienie";
	test_path = "textures_and_fonts/textures/monopoly_single_square_clear.png";
	test_width = 200;
	test_height = 200;
	test_rotation = 0.0;
	test_position = sf::Vector2i(160, 160);

	const Field test_field_7 =
		Field(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position);

	test_board.push_back(test_field_7);

	test_id = 12;
	test_type = UTILITY;
	test_name = "Elektrownia";
	test_path = "textures_and_fonts/textures/monopoly_single_square_empty.png";
	test_width = 80;
	test_height = 200;
	test_rotation = 90.0;
	test_position = sf::Vector2i(180, 180);
	test_price = 150;
	std::map<UtilityTiers, unsigned int> test_rent_multipliers = {{ONE_UTILITY, 4}, {TWO_UTILITIES, 10}};
	test_group_members = {28};
	test_mortage = 75;

	const UtilityField test_field_8 = UtilityField(test_id, test_type, test_name, test_path, test_width, test_height,
		test_rotation, test_position, test_price, test_rent_multipliers, test_group_members, test_mortage);

	test_board.push_back(test_field_8);

	test_id = 20;
	test_type = FREE_PARKING;
	test_name = "Bezpłatny Parking";
	test_path = "textures_and_fonts/textures/monopoly_single_square_clear.png";
	test_width = 200;
	test_height = 200;
	test_rotation = 180.0;
	test_position = sf::Vector2i(200, 200);

	const Field test_field_9 =
		Field(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position);

	test_board.push_back(test_field_9);

	test_id = 30;
	test_type = GO_TO_JAIL;
	test_name = "Idziesz do więzienia!";
	test_path = "textures_and_fonts/textures/monopoly_single_square_clear.png";
	test_width = 200;
	test_height = 200;
	test_rotation = 180.0;
	test_position = sf::Vector2i(220, 220);

	const Field test_field_10 =
		Field(test_id, test_type, test_name, test_path, test_width, test_height, test_rotation, test_position);

	test_board.push_back(test_field_10);

	std::string TEST_PATH = "Monopoly/tests/test_board.json";
	Board TEST_BOARD = Board(TEST_PATH);

	std::vector<PossibleFields> created_board = TEST_BOARD.getBoard();

	SECTION("Constructor") {
		REQUIRE(TEST_BOARD.getFieldNumber() == created_board.size());
		REQUIRE(test_board.size() == created_board.size());

		for (int i = 0; i < test_board.size(); ++i) {
			FieldType test_field_type = std::visit([](Field& field) { return field.getType(); }, test_board[i]);

			switch (test_field_type) {
				case STREET: {
					StreetField field_from_created = std::get<StreetField>(created_board[i]);
					StreetField field_from_test = std::get<StreetField>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					CHECK(field_from_created.getPrice() == field_from_test.getPrice());
					CHECK(field_from_created.getHousePrice() == field_from_test.getHousePrice());
					CHECK(field_from_created.getHotelPrice() == field_from_test.getHotelPrice());
					CHECK(field_from_created.getRentValues() == field_from_test.getRentValues());
					CHECK(field_from_created.getGroupMembers() == field_from_test.getGroupMembers());
					CHECK(field_from_created.getMortage() == field_from_test.getMortage());
					break;
				}

				case STATION: {
					StationField field_from_created = std::get<StationField>(created_board[i]);
					StationField field_from_test = std::get<StationField>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					CHECK(field_from_created.getPrice() == field_from_test.getPrice());
					CHECK(field_from_created.getRentValues() == field_from_test.getRentValues());
					CHECK(field_from_created.getGroupMembers() == field_from_test.getGroupMembers());
					CHECK(field_from_created.getMortage() == field_from_test.getMortage());
					break;
				}

				case UTILITY: {
					UtilityField field_from_created = std::get<UtilityField>(created_board[i]);
					UtilityField field_from_test = std::get<UtilityField>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					CHECK(field_from_created.getPrice() == field_from_test.getPrice());
					CHECK(field_from_created.getRentMultipliers() == field_from_test.getRentMultipliers());
					CHECK(field_from_created.getGroupMembers() == field_from_test.getGroupMembers());
					CHECK(field_from_created.getMortage() == field_from_test.getMortage());
					break;
				}

				case GO: {
					Field field_from_created = std::get<Field>(created_board[i]);
					Field field_from_test = std::get<Field>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					break;
				}

				case CHANCE: {
					Field field_from_created = std::get<Field>(created_board[i]);
					Field field_from_test = std::get<Field>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					break;
				}

				case COMMUNITY_CHEST: {
					Field field_from_created = std::get<Field>(created_board[i]);
					Field field_from_test = std::get<Field>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					break;
				}

				case TAX: {
					TaxField field_from_created = std::get<TaxField>(created_board[i]);
					TaxField field_from_test = std::get<TaxField>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					CHECK(field_from_created.getTaxValue() == field_from_test.getTaxValue());
					break;
				}

				case JAIL: {
					Field field_from_created = std::get<Field>(created_board[i]);
					Field field_from_test = std::get<Field>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					break;
				}

				case FREE_PARKING: {
					Field field_from_created = std::get<Field>(created_board[i]);
					Field field_from_test = std::get<Field>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					break;
				}

				case GO_TO_JAIL: {
					Field field_from_created = std::get<Field>(created_board[i]);
					Field field_from_test = std::get<Field>(test_board[i]);
					CHECK(field_from_created.getId() == field_from_test.getId());
					CHECK(field_from_created.getType() == field_from_test.getType());
					CHECK(field_from_created.getName() == field_from_test.getName());
					CHECK(field_from_created.getGraphicPath() == field_from_test.getGraphicPath());
					CHECK(field_from_created.getWidth() == field_from_test.getWidth());
					CHECK(field_from_created.getHeight() == field_from_test.getHeight());
					CHECK(field_from_created.getRotation() == field_from_test.getRotation());
					break;
				}
			}
		}
	}

	SECTION("clearBoard() method") {
		TEST_BOARD.clearBoard();
		REQUIRE(TEST_BOARD.getFieldNumber() == 0);
	}

	SECTION("getFieldById() method") {
		StreetField given_field = std::get<StreetField>(TEST_BOARD.getFieldById(3));
		CHECK(given_field.getId() == test_field_3.getId());
		CHECK(given_field.getType() == test_field_3.getType());
		CHECK(given_field.getName() == test_field_3.getName());
		CHECK(given_field.getGraphicPath() == test_field_3.getGraphicPath());
		CHECK(given_field.getWidth() == test_field_3.getWidth());
		CHECK(given_field.getHeight() == test_field_3.getHeight());
		CHECK(given_field.getRotation() == test_field_3.getRotation());
		CHECK(given_field.getPrice() == test_field_3.getPrice());
		CHECK(given_field.getHousePrice() == test_field_3.getHousePrice());
		CHECK(given_field.getHotelPrice() == test_field_3.getHotelPrice());
		CHECK(given_field.getRentValues() == test_field_3.getRentValues());
		CHECK(given_field.getGroupMembers() == test_field_3.getGroupMembers());
		CHECK(given_field.getMortage() == test_field_3.getMortage());
	}
}
