#include <iostream>
#include <string>
#include <cstdlib>
#include <string.h>
#include <thread>
#include <list>
#include <mutex>

using namespace std;

#include "provided/ServerSocket.h"
#include "provided/ClientSocket.h"

#define BUFSIZE 1

list<hw5_net::ClientSocket*> sockets;
mutex mtx;

// Get a JSON response from the server, going through the internet using peerSocket.
// Sets responseJson to represent the model's JSON response.
// Returns 0 if successful, 1 otherwise. responseJson is undefined if 1 is returned.
char readChar(hw5_net::ClientSocket* peerSocket) {
    char buf[1];
    peerSocket->WrappedRead(buf, 1);
    return *buf;
    /*
    cout << "Reading" << endl;
    
    int curlyBraceCount = 0;
    int stringJsonAllocatedSize = sizeof(char) * BUFSIZE;
    char* stringJson = (char*) malloc(stringJsonAllocatedSize);
    int size = 0; //how much is actually being used
    do {
        printf("in do-loop");
        char buf[BUFSIZE];
        int readCount = peerSocket->WrappedRead(buf, BUFSIZE - 1);
        
        //if allocated space isn't big enough, reallocate
        if (readCount != 1) {
            printf("readCount %d\n", readCount);
        }
        if (size + readCount > stringJsonAllocatedSize){
            stringJson = (char*) realloc(stringJson, stringJsonAllocatedSize + BUFSIZE * sizeof(char));
            stringJsonAllocatedSize += BUFSIZE * sizeof(char);
        }
        //move chars into allocated space and count curly braces to see if we're done or not
        for (int i = 0; i < readCount; i++){
            stringJson[size] = buf[i];
            size++;
            if (buf[i] == '{'){
                curlyBraceCount++;
            } else if (buf[i] == '}'){
                curlyBraceCount--;
            }
        }
    } while (curlyBraceCount != 0);
    
    stringJson[size] = '\0';
    cout.flush();
    cout << "done" << endl;
    
    printf("stringJson: %s\n", stringJson);
    json_error_t error;
    json_t* resJson = json_loads(stringJson, 0, &error);
    
    free(stringJson);
    
    if (!resJson) {
        printf("Response is not json value.");
        return 1;
    } else {
        *responseJson = resJson;
        return 0;
    }
     */
    
}

void threadExec(int port) {
    try {
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
        sockets.push_front(peerSocket);
        mtx.unlock();
        
        thread* t = new thread(&threadExec, port);
        
        while (true) {
            char c = readChar(peerSocket);
            string toWrite(c);
            mtx.lock();
            for (int i = 0; i < sockets.length(); i++) {
                sockets[i].wrappedWrite(toWrite.c_str(), toWrite.length());
            }
            mtx.unlock();
        }
    } catch(string errString) {
        cout << errString << endl;
        return 1;
    }
}

int main(int argc, char *argv[]) {
    
    // Make sure arguments are correct.
    if ( argc != 2 && argc != 3 ) usage(argv[0]);
    
    int port = 0;
    try {
        if ( argc == 3 ) port = stoi(argv[2]);
    } catch (...) {
        usage(argv[0]);
    }
    
    try {
        thread* t = new thread(&threadExec, port);
    } catch(string errString) {
        cout << errString << endl;
        return 1;
    }
    
    cout << "Closing" << endl;
    
    return EXIT_SUCCESS;
}
