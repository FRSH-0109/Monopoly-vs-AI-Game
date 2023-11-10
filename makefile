LIBS=-L SFML/lib/ -l sfml-graphics -l sfml-window -l sfml-system

all: monopolyVsAI

Monopoly/main.o: Monopoly/main.cpp
	g++ -c "Monopoly/main.cpp" -o Monopoly/main.o -I SFML/include

monopolyVsAI: Monopoly/main.o
	g++ -o Monopoly/monopolyVsAI Monopoly/main.o $(LIBS)

clean:
	rm -f monopolyVsAI *.o