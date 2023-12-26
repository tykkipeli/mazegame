main: main.o Game.o Map.o utils.o Player.o Keyboardinput.o physics.o Monster.o GameMenu.o Button.o
	g++ main.o Game.o Map.o utils.o Player.o Keyboardinput.o physics.o Monster.o GameMenu.o Button.o -lSDL2 -lSDL2_ttf -lSDL2main -o main
	
main.o: main.cpp
	g++ -c main.cpp
	
Game.o: Game.cpp Game.hpp Map.o Player.o Monster.o GameMenu.o
	g++ -c Game.cpp
	
Map.o: Map.cpp Map.hpp utils.o
	g++ -c Map.cpp
	
utils.o: utils.cpp utils.hpp
	g++ -c utils.cpp
	
Player.o: Player.cpp Player.hpp Keyboardinput.o Map.o physics.o utils.o
	g++ -c Player.cpp
	
Keyboardinput.o: Keyboardinput.cpp Keyboardinput.hpp
	g++ -c Keyboardinput.cpp
	
physics.o: physics.cpp physics.hpp
	g++ -c physics.cpp
	
Monster.o: Monster.cpp Monster.hpp utils.o
	g++ -c Monster.cpp
	
GameMenu.o: GameMenu.cpp GameMenu.hpp Keyboardinput.o Button.o
	g++ -c GameMenu.cpp
	
Button.o: Button.cpp Button.hpp
	g++ -c Button.cpp

clean:
	rm*.o math
