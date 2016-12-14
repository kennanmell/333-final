#include <iostream>
#include <string>
#include <cstdlib>
#include <string.h>

using namespace std;

#include "provided/ServerSocket.h"
#include "provided/ClientSocket.h"

#define BUFSIZE 1

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

int main(int argc, char** argv){
    if (argc != 4){
        printf("Usage: ./q12-client host port");
        return 1;
    }
    int serverPort;
    try {
        serverPort = stoi(argv[2]);
    } catch (...){
    }
    
    try {
        string serverName(argv[1]);
        //hw5_net::ClientSocket clientSocket(serverName, serverPort);
        hw5_net::ClientSocket* clientSocket = new hw5_net::ClientSocket(serverName, serverPort);
        while (true) {
            string helloMessage = string(argv[3]);
            clientSocket->WrappedWrite(helloMessage.c_str(), helloMessage.length());
            char c = readChar(clientSocket);
            printf("%c", c);
        }
    } catch(string errString) {
        cout << errString << endl;
        printf("error\n");
        return 1;
    }
    printf("exiting\n");
    return 0;
}
