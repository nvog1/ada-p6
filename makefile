#Nicolás Van Olmen Giménez, 74383864R
all: maze.cc
	g++ -g -Wall -o maze maze.cc
clean:
	rm maze
tar: maze.cc
	tar cvzf 74383864R.tar.gz maze.cc makefile
