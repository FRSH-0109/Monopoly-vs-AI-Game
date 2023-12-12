cmake_minimum_required(VERSION 3.16)
project(MonopolyVsAI LANGUAGES CXX)

find_package(Catch2 REQUIRED)
# These tests can use the Catch2-provided main
set(SOURCES monopolyTests.cpp ../gameEngine.cpp ../gameEngine.h ../monopolyGameEngine.cpp ../monopolyGameEngine.h ../activeScreen.cpp ../activeScreen.h ../button.cpp ../button.h ../contextWindow.cpp ../contextWindow.h)
#set(HEADER_FILES ../gameEngine.h)

#add_library(gameEngineLib ./gameEngine.cpp ../gameEngine.h)
#target_include_directories(gameEngineLib PRIVATE .)
add_executable(tests ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE ../../SFML/include)
target_include_directories(tests PRIVATE ../../SFML/lib)
target_include_directories(tests PRIVATE ../../SFML/bin)
target_link_libraries(tests PRIVATE sfml-graphics)
target_link_libraries(tests PRIVATE sfml-window)
target_link_libraries(tests PRIVATE sfml-system)
target_compile_features(tests PRIVATE cxx_std_17)
target_link_directories(tests PRIVATE ../../SFML/include)
target_link_directories(tests PRIVATE ../../SFML/lib)
target_link_directories(tests PRIVATE ../../SML/bin)

# # These tests need their own main
# add_executable(custom-main-tests test.cpp test-main.cpp)
# target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)