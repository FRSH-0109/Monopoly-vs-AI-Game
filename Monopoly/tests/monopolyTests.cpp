#include "catch2/src/catch2/catch_all.hpp"
// #include "SFML/Graphics.hpp"
#include "../gameEngine.h"

using namespace std;

TEST_CASE("Initial tests")
{
    CHECK(true);
}

TEST_CASE("Test of Game Engine constructor")
{
    GameEngine testEngine = GameEngine(30, 1000, 1200);
    REQUIRE(testEngine.getWindowWidth() == 1000);
    REQUIRE(testEngine.getWindowHeight() == 1200);
}