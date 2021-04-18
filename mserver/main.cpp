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

using namespace std;
using json = nlohmann::json;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int baseSocketNumber = 5000;
void bindingProcess(int* sockfd, int* portno, struct sockaddr_in* serv_addr){

    *portno = baseSocketNumber;
    serv_addr->sin_family = AF_UNIX;
    serv_addr->sin_port = htons(*portno);
    serv_addr->sin_addr.s_addr = INADDR_ANY;

    cout << "Binding to port " << *portno << "..." << endl;
    if (bind(*sockfd, (struct sockaddr *) serv_addr , sizeof(*serv_addr)) < 0){
        cout << "Error on Binding: Address already in use" << endl;
        baseSocketNumber += 1;
        bindingProcess(sockfd, portno, serv_addr);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    int sockfd, newsockfd, portno, n;
    int option = 1;
    socklen_t clilen;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;

    cout << "Opening Socket..." << endl;
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
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


    void* startAdress = malloc(10000);

    cout << startAdress << endl;

    free(startAdress);




    return a.exec();
}
