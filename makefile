LIBS=-L SFML/lib/ -l sfml-graphics -l sfml-window -l sfml-system

all: monopolyVsAI

main.o: Monopoly/main.cpp
	g++ -c "Monopoly/main.cpp" -o main.o -I SFML/include

monopolyVsAI: main.o
	g++ -o monopolyVsAI main.o $(LIBS)
	export LD_LIBRARY_PATH=SFML/lib

clean:
	rm -f monopolyVsAI *.o