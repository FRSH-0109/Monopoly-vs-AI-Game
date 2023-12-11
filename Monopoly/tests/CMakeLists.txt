cmake_minimum_required(VERSION 3.22)
project(MonopolyVsAI)
#udo apt install catch2 -y
find_package(Catch2 2 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests monopolyTests.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# # These tests need their own main
# add_executable(custom-main-tests test.cpp test-main.cpp)
# target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)