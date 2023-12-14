# Monopoly-vs-AI-Game
Project of Monopoly vs AI game

## Used libraries
* SFML v.2.6.0 - Graphic library used in games and multimedia projects
* Catch v.2 - Unit test framework used for testing our code

## Build project on linux
    1. Run "make" for project building
    2. Run "sh monopolyVsAI.sh" for launching game

## Tests framework Build linux
Unit testing framework-  Catch2.

This part have to be done only once, repeat only if "make cleanAll" or "make cleanTestsConfig" was called
    
    1. Run "make testsConfig" for tests Catch2 framework building

## Tests Build and Run on linux
    1. Run "make tests" for tests files building
    2. Run "sh monopolyTests" for launching testing script

## Code formatting
Before running clang-format you need to have it installed. In case you don't have it installed run:

    sudo apt install clang-format

With clang-format installed code can be formatted by running

    sh clang-format.sh

## Build on windows
