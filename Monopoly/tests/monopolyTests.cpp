/**
 * @file monopolyTests.cpp
 *
 * @brief Source file for project unit tests
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "catch2/src/catch2/catch_all.hpp"
#include "../gameEngine.h"
#include "../contextWindow.h"
#include "../activeScreen.h"
#include "../monopolyGameEngine.h"

using namespace std;

TEST_CASE("Initial tests")
{
    CHECK(true);
}

TEST_CASE("GameEngine constructor")
{
    GameEngine testEngine = GameEngine(30, 1000, 1200);
    REQUIRE(testEngine.getWindowWidth() == 1000);
    REQUIRE(testEngine.getWindowHeight() == 1200);
}

TEST_CASE("ContextWindow Singleton design pattern")
{
    ContextWindow* contextWindow = ContextWindow::GetInstance();
    ContextWindow* newContextWindow = ContextWindow::GetInstance();
    REQUIRE(contextWindow == newContextWindow);
}

TEST_CASE("GameEngine getContextWindow()")
{
    GameEngine testEngine = GameEngine(30, 1000, 1200);
    REQUIRE(testEngine.getContextWindow() == ContextWindow::GetInstance());
}

TEST_CASE("ActiveScreen getContextWindow")
{
    MainMenuScreen mainMenu = MainMenuScreen();
    GameMenuScreen gameMenu = GameMenuScreen();
    ContextWindow* contextWindow = ContextWindow::GetInstance();
    mainMenu.setContextWindow(contextWindow);
    gameMenu.setContextWindow(contextWindow);
    REQUIRE(mainMenu.getContextWindow() == contextWindow);
    REQUIRE(gameMenu.getContextWindow() == contextWindow);
}

TEST_CASE("MainMenuScreen constructor")
{
    MainMenuScreen mainMenu = MainMenuScreen();
    REQUIRE(mainMenu.getScreenType() == MainMenu);
}

TEST_CASE("GameMenuScreen constructor")
{
    GameMenuScreen gameMenu = GameMenuScreen();
    REQUIRE(gameMenu.getScreenType() == GameMenu);
}

TEST_CASE("monopolyGameEngine constructor")
{
    monopolyGameEngine monopolyEngine = monopolyGameEngine();
    REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
    REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
}

TEST_CASE("monopolyGameEngine setPlayersHumanNumber()")
{
    monopolyGameEngine monopolyEngine = monopolyGameEngine();
    REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
    REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
    monopolyEngine.setPlayersHumanNumber(3);
    REQUIRE(monopolyEngine.getPlayersHumanNumber() == 3);
    REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
}

TEST_CASE("monopolyGameEngine setPlayersAINumber()")
{
    monopolyGameEngine monopolyEngine = monopolyGameEngine();
    REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
    REQUIRE(monopolyEngine.getPlayersAINumber() == 1);
    monopolyEngine.setPlayersAINumber(2);
    REQUIRE(monopolyEngine.getPlayersHumanNumber() == 1);
    REQUIRE(monopolyEngine.getPlayersAINumber() == 2);
}
