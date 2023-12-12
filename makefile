LIBS :=-L SFML/lib/ -l sfml-graphics -l sfml-window -l sfml-system
CXX := g++ -g -std=c++14 -Wextra -Wall -Wpedantic
INC := -I Monopoly/
SRC := Monopoly/*.o
OBJ := Monopoly/main.o Monopoly/gameEngine.o Monopoly/button.o Monopoly/activeScreen.o Monopoly/contextWindow.o Monopoly/monopolyGameEngine.o
all: monopolyVsAI clean
tests: monopolyTests

Monopoly/%.o: Monopoly/%.cpp
	$(CXX) -c $< -o $@ -I SFML/include $(INC)

monopolyVsAI: $(OBJ)
	$(CXX) -o Monopoly/monopolyVsAI $(SRC) $(LIBS) $(INC)

testsConfig:
	mkdir Monopoly/tests/catch2
	git clone https://github.com/catchorg/Catch2.git Monopoly/tests/catch2
	cd Monopoly/tests/catch2
	cmake -Bbuild -H. -DBUILD_TESTING=OFF Monopoly/tests/catch2
	sudo cmake --build build/ --target install
	cd ../..

tests:
	cmake Monopoly/tests/CMakeLists.txt
	make -C Monopoly/tests/

clean:
	rm -f Monopoly/*.o

cleanTests:
	rm -frd Monopoly/tests/CMakeFiles
	rm -f Monopoly/tests/*cmake
	rm -f Monopoly/tests/CMakeCache.txt
	rm -f Monopoly/tests/Makefile
	rm -f Monopoly/tests/tests

