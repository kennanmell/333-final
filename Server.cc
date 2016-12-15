#include <iostream>
#include <string>
#include <cstdlib>
#include <string.h>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

#include "provided/ServerSocket.h"
#include "provided/ClientSocket.h"

#define BUFSIZE 1

vector<hw5_net::ClientSocket*>* sockets = new vector<hw5_net::ClientSocket*>();
vector<thread*>* threads = new vector<thread*>();
mutex mtx;

// Get a JSON response from the server, going through the internet using peerSocket.
// Sets responseJson to represent the model's JSON response.
// Returns 0 if successful, 1 otherwise. responseJson is undefined if 1 is returned.
char readChar(hw5_net::ClientSocket* peerSocket) {
    char buf[1];
    peerSocket->WrappedRead(buf, 1);
    return *buf;
}

void threadExec(hw5_net::ClientSocket* peerSocket) {
    try {
        while (true) {
	    mtx.lock();
		printf("in loop \n");
            char c = readChar(peerSocket);
            char* ct;
            sprintf(ct, "%c", c);
            string toWrite(ct);
            for (uint i = 0; i < sockets->size(); i++) {
                sockets->at(i)->WrappedWrite(toWrite.c_str(), toWrite.length());
            }
            mtx.unlock();
        }
    } catch(string errString) {
        cout << errString << endl;
    }
}

int main(int argc, char *argv[]) {
    
    // Make sure arguments are correct.
    if ( argc != 2 ) {
      printf("Usage: ./q12 port\n");
      return 1;
    }
    
    int port = 0;
    try {
	port = stoi(argv[1]);
    } catch (...) {
	printf("Usage: ./q12 port\n");
    }

    try {
        while (true) {
            int socketFd;
            hw5_net::ServerSocket sock(port);
            sock.BindAndListen(AF_INET, &socketFd);
            cout << "Created bound socket. port = " << sock.port() << endl;
            
            // wait for client connection
            int acceptedFd;
            string clientAddr;
            uint16_t clientPort;
            string clientDNSName;
            string serverAddress;
            string serverDNSName;
            
            sock.Accept( &acceptedFd,
                        &clientAddr,
                        &clientPort,
                        &clientDNSName,
                        &serverAddress,
                        &serverDNSName );
            
            cout << "Got connection from: " << endl
            << "\tacceptedFd\t" << acceptedFd << endl
            << "\tclientPort\t" << clientPort << endl
            << "\tclientDNSName\t" << clientDNSName << endl
            << "\tserverAddress\t" << serverAddress << endl
            << "\tserverDNSName\t" << serverDNSName << endl;
            
            // wrap connection to peer with a CientSocket
            hw5_net::ClientSocket* peerSocket = new hw5_net::ClientSocket(acceptedFd);
            mtx.lock();
            sockets->push_back(peerSocket);
            mtx.unlock();
            printf("in loop\n");
            thread* t = new thread(&threadExec, peerSocket);
	    threads->push_back(t);
        }
        port = stoi(argv[1]);
    } catch (...) {
        printf("Invalid port number");
        return 1;
    }
    
    cout << "Closing" << endl;
    
    return EXIT_SUCCESS;
}
