
LIBS = -ggdb `pkg-config gtkmm-3.0 --cflags --libs`

all: main.o MyControl.o Maze.o AppWin.o Block.o
	g++ -Wall -o maze main.o MyControl.o Maze.o AppWin.o Block.o ${LIBS}

main.o: main.cpp
	g++ -Wall -c main.cpp ${LIBS}
MyControl.o: MyControl.cpp
	g++ -Wall -c MyControl.cpp ${LIBS}
Maze.o: Maze.cpp
	g++ -Wall -c Maze.cpp ${LIBS}
AppWin.o: AppWin.cpp
	g++ -Wall -c AppWin.cpp ${LIBS}
Block.o: Block.cpp
	g++ -Wall -c Block.cpp ${LIBS}

clean:
	rm *.o
	rm maze
