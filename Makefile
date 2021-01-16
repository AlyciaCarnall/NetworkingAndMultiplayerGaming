CXXFLAGS=-std=c++14
CFLAGS=-g -lsfml-graphics -lsfml-window -lsfml-system
CPPFLAGS=
LDFLAGS=
LDLIBS=
LIBS=
CPP=g++

all: main server

main: main.o entity.o tile.o builder.o world.o game.o message.o network.o receiver.o util.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

server: util.o server.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

clean:
	\rm -f *.o main
