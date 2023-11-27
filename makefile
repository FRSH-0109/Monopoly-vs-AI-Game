LIBS :=-L SFML/lib/ -l sfml-graphics -l sfml-window -l sfml-system
CXX := g++ -g -std=c++11 -Wextra -Wall -Wpedantic
INC := -I Monopoly/
SRC := Monopoly/*.o
OBJ := Monopoly/main.o Monopoly/gameEngine.o Monopoly/button.o Monopoly/mainMenu.o Monopoly/contextWindow.o
all: monopolyVsAI clean

Monopoly/%.o: Monopoly/%.cpp
	$(CXX) -c $< -o $@ -I SFML/include $(INC)

monopolyVsAI: $(OBJ)
	$(CXX) -o Monopoly/monopolyVsAI $(SRC) $(LIBS) $(INC)

clean:
	rm -f Monopoly/*.o