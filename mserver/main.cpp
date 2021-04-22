#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <json.hpp>
#include <map>
#include <sstream>

using namespace std;
using json = nlohmann::json;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int baseSocketNumber = 5000;
int sockfd, newsockfd;
char buffer[255];
void* startAdress;
int mainOffset = 0;
map<string,int> nameToOffsetMap;
map<string,string> nameToTypeMap;

void bindingProcess(int* sockfd, int* portno, struct sockaddr_in* serv_addr){

    *portno = baseSocketNumber;
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(*portno);
    serv_addr->sin_addr.s_addr = INADDR_ANY;

    cout << "Binding to port " << *portno << "..." << endl;
    if (bind(*sockfd, (struct sockaddr *) serv_addr , sizeof(*serv_addr)) < 0){
        cout << "Error on Binding: Address already in use" << endl;
        baseSocketNumber += 1;
        bindingProcess(sockfd, portno, serv_addr);
    }
}
void readBuffer();

void analizeBuffer(){
    if (buffer[0] == '{'){
        void* returningAdress;
        json jsonBuffer = json::parse(buffer);
        if (jsonBuffer["type"] == "int"){
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            int* placementAdress = (int*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            try {
                long double ld;
                if (variableValue.back() == 'f'){
                    variableValue.pop_back();
                    if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                        *placementAdress = atoi(variableValue.c_str());
                    }else{
                        throw variableValue;
                    }
                }if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                    *placementAdress = atoi(variableValue.c_str());
                }else{
                    throw variableValue;
                }
            }  catch (string variableValue){
                string storageError ="ERROR: " + variableValue + " is not a valid int.";
                cout << storageError << endl;
                /*
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(sockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }*/
            }
            returningAdress = placementAdress;
            int i = *placementAdress;
            cout << i << endl;
        }if (jsonBuffer["type"] == "float"){
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            float* placementAdress = (float*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            try {
                long double ld;
                if (variableValue.back() == 'f'){
                    variableValue.pop_back();
                    if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                        *placementAdress = stof(variableValue);
                    }else{
                        throw variableValue;
                    }
                }
                if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                    *placementAdress = stof(variableValue);
                }else{
                    throw variableValue;
                }
            }  catch (string variableValue){
                string storageError ="ERROR: " + variableValue + " is not a valid float.";
                /*
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(sockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }*/
            }
            returningAdress = placementAdress;
            float f = *placementAdress;
            cout << f << endl;
        }if (jsonBuffer["type"] == "double"){
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            double* placementAdress = (double*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            try {
                long double ld;
                if (variableValue.back() == 'f'){
                    variableValue.pop_back();
                    if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                        *placementAdress = stod(variableValue);
                    }else{
                        throw variableValue;
                    }
                }
                if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                    *placementAdress = stod(variableValue);
                }else{
                    throw variableValue;
                }
            }  catch (string variableValue){
                string storageError ="ERROR: " + variableValue + " is not a valid float.";
                /*
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(sockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }*/
            }
            returningAdress = placementAdress;
            double d = *placementAdress;
            cout << d << endl;
        } if (jsonBuffer["type"] == "long"){
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            long* placementAdress = (long*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            try {
                long double ld;
                if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                    *placementAdress = stol(variableValue);
                }else{
                    throw variableValue;
                }
            }  catch (string variableValue){
                string storageError ="ERROR: " + variableValue + " is not a valid long.";
                cout << storageError << endl;
                /*
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(sockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }*/
            }
            returningAdress = placementAdress;
            long l = *placementAdress;
            cout << l << endl;
        } if (jsonBuffer["type"] == "char"){
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            char* placementAdress = modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            try {
                if (variableValue.length() == 3 && variableValue.front() == '\'' && variableValue.back() == '\''){
                    *placementAdress = variableValue[1];
                }else{
                    throw variableValue;
                }
            }  catch (string variableValue){
                string storageError ="ERROR: " + variableValue + " is not a valid char.";
                cout << storageError << endl;
                /*
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(sockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }*/
            }
            returningAdress = placementAdress;
            char c = *placementAdress;
            cout << c << endl;
        }
        string variableName = jsonBuffer["name"];
        string variableType = jsonBuffer["type"];
        int offset = mainOffset;
        mainOffset = mainOffset + (int)jsonBuffer["size"];
        nameToOffsetMap.insert(pair<string, int>(variableName,offset));
        nameToTypeMap.insert(pair<string, string>(variableName,variableType));
        std::stringstream ss;
        ss << returningAdress;
        string returningAdressString = ss.str();
        jsonBuffer["adress"] = returningAdressString;
        string sendJson = jsonBuffer.dump();
        cout << startAdress << endl;
        cout << returningAdress << endl;
        /*
        memset(buffer,0,255);
        strncpy(buffer, sendJson.c_str(),255);
        int n = write(sockfd,buffer,strlen(buffer));
        if (n < 0){
            error("ERROR writing to socket");
        }*/
   }
   readBuffer();
}


void readBuffer(){
    memset(buffer, 0, 255);
    cout << "waiting for message..." << endl;
    int n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Message received: %s\n",buffer);
    analizeBuffer();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout.precision(16);
    int portno;
    int option = 1;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    cout << "Opening Socket..." << endl;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&option,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    if (sockfd < 0){
        error("ERROR opening socket");
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    bindingProcess(&sockfd, &portno, &serv_addr);


    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    cout << "Waiting for c! client..." << endl;
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    cout << "Connected to C! client" << endl;

    /*
    memset(buffer, 0, 255);

    int n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);

    close(newsockfd);
    close(sockfd);
    */



    startAdress = malloc(10000);

    readBuffer();







    return a.exec();
}
