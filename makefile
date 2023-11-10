LIBS=-L SFML/lib/ -l sfml-graphics -l sfml-window -l sfml-system
CXX := g++

all: monopolyVsAI clean

Monopoly/%.o: Monopoly/%.cpp
	$(CXX) -c $< -o $@ -I SFML/include

monopolyVsAI: Monopoly/main.o
	$(CXX) -o Monopoly/monopolyVsAI Monopoly/main.o $(LIBS)

clean:
	rm -f Monopoly/*.o