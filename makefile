SRC=Server.cc provided/CSE333Socket.cc provided/ClientSocket.cc provided/ServerSocket.cc provided/Verify333.c
HEADERS=provided/CSE333.h provided/CSE333Socket.h provided/ClientSocket.h provided/ServerSocket.h
CFLAGS = -Wall -Wpedantic -std=c++11

all: q12

q12: $(SRC) $(HEADERS)
	g++ -pthread $(CFLAGS) -g -O0 -o q12 $(SRC)

clean:
	rm -f *.o *~ q12
