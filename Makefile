all: compile link

compile:
	g++ -Isrc/include -c pp.cpp

link:
	g++ pp.o -o pp -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system