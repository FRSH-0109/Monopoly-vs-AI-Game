# Monopoly-vs-AI-Game
Project of Monopoly vs AI game

Authors: Kamil Kośnik, Kacper Radzikowski

## Used libraries
* SFML v.2.6.0 - Graphic library used in games and multimedia projects
* Catch v.2 - Unit test framework used for testing our code

## Build and run project on linux
For building project run

    make
For launching game

    sh monopolyVsAI.sh

## Tests framework Build on linux
Unit testing framework - Catch2.

This part have to be done only once, repeat only if "make cleanAll" or "make cleanTestsConfig" was called

For downloading and building tests framework run
    
    make testsConfig

## Tests Build and Run on linux
To build avaiable tests run

    make tests
For launching testing script run
    
    sh monopolyTests

## Code formatting
Before running clang-format you need to have it installed. In case you don't have it installed run:

    sudo apt install clang-format

With clang-format installed code can be formatted by running

    sh clang-format.sh

## Cleaning project directory
For cleaning all project run

    make cleanAll

For cleaning only game files run

    make clean

For cleaninig only tests files run

    make cleanTests

For cleaning tests framework run

    make cleanTestsConfig
