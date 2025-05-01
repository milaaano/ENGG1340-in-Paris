FLAGS = -pedantic-errors -std=c++11

start: main_menu
	./main_menu

main_menu: main_menu.o DataHandling/users.o 1340_Group_Project/battleship.o Multiplayer/multiplayer.o
	g++ $(FLAGS) $^ -o $@

main_menu.o: main_menu.cpp main_menu.h
	g++ $(FLAGS) -c $< -o $@

DataHandling/users.o: DataHandling/users.cpp DataHandling/users.h
	g++ $(FLAGS) -c $< -o $@

1340_Group_Project/battleship.o: 1340_Group_Project/battleship.cpp 1340_Group_Project/battleship.h
	g++ $(FLAGS) -c $< -o $@

Multiplayer/multiplayer.o: Multiplayer/multiplayer.cpp Multiplayer/multiplayer.h
	g++ $(FLAGS) -c $< -o $@

clean:
	rm -rf main_menu main_menu.o DataHandling/users.o 1340_Group_Project/battleship.o Multiplayer/multiplayer.o

.PHONY: start clean