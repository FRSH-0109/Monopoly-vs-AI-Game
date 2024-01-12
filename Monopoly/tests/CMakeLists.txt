cmake_minimum_required(VERSION 3.16)
project(MonopolyVsAI LANGUAGES CXX)

find_package(Catch2 REQUIRED)
# These tests can use the Catch2-provided main
set(SOURCES monopolyTests.cc ../GameEngine.cc ../GameEngine.h ../MonopolyGameEngine.cc ../MonopolyGameEngine.h ../ActiveScreen.cc
../ActiveScreen.h ../Button.cc ../Button.h ../ContextWindow.cc ../ContextWindow.h ../Field.h ../Field.cc ../Board.h ../Board.cc
../Player.h ../Player.cc ../GameScreen.h ../GameScreen.cc ../NotificationWall.cc ../NotificationWall.h ../Withdraw.h ../Withdraw.cc
../AiAdapter.h ../AiAdapter.cc ../Chance.h ../Chance.cc)
#set(HEADER_FILES ../gameEngine.h)

#add_library(gameEngineLib ./gameEngine.cc ../gameEngine.h)
#target_include_directories(gameEngineLib PRIVATE .)
add_executable(tests ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE ../../SFML_Monopoly/include)
target_include_directories(tests PRIVATE ../../SFML_Monopoly/lib)
target_include_directories(tests PRIVATE ../../SFML_Monopoly/bin)
target_link_libraries(tests PRIVATE sfml-graphics)
target_link_libraries(tests PRIVATE sfml-window)
target_link_libraries(tests PRIVATE sfml-system)
target_compile_features(tests PRIVATE cxx_std_17)
target_link_directories(tests PRIVATE ../../SFML_Monopoly/include)
target_link_directories(tests PRIVATE ../../SFML_Monopoly/lib)
target_link_directories(tests PRIVATE ../../SFML_Monopoly/bin)

# # These tests need their own main
# add_executable(custom-main-tests test.cc test-main.cc)
# target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)