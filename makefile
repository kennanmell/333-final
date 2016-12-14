SRC=client.cc provided/CSE333Socket.cc provided/ClientSocket.cc provided/ServerSocket.cc provided/Verify333.c
HEADERS=provided/CSE333.h provided/CSE333Socket.h provided/ClientSocket.h provided/ServerSocket.h
CFLAGS = -Wall -Wpedantic -std=c++11
IFLAGS =  -I../hw2 -I../jansson/include
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS =  -L../hw2 -L../jansson/lib -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

all: hw5

hw5: $(SRC) $(HEADERS) $(LIBS)
	g++ $(CFLAGS) -g -O0 $(IFLAGS) $(GTKCFLAGS) -o hw5-model $(SRC) $(LDFLAGS) $(GTKLDFLAGS)

clean:
	rm -f *.o *~ hw5