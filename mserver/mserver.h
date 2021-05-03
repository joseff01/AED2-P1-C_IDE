#ifndef MSERVER_H
#define MSERVER_H

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
#include <iomanip>

using namespace std;
using json = nlohmann::json;

class Mserver
{
    public:

        Mserver();
        void error(const char *msg);
        void bindingProcess(int* sockfd, int* portno, struct sockaddr_in* serv_addr);
        void resetMemory();
        string getVariableValue(string variableName);
        double checkNumericalValueOfExpression(string stringExpression);
        bool checkBooleanValueOfExpression(string stringExpression);
        vector<string> removeScopeMemory();
        void analizeBuffer();
        void readBuffer();
        void serverSetup();


};

#endif // MSERVER_H
