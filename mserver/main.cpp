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
int currentScope = 0;
map<string,int> nameToOffsetMap;
map<string,string> nameToTypeMap;
map<string,int> nameToScopeMap;

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


void resetMemory(){
    nameToOffsetMap.clear();
    nameToTypeMap.clear();
    nameToScopeMap.clear();
    mainOffset = 0;
    currentScope = 0;
    free(startAdress);
    startAdress = malloc(10000);
}

string getVariableValue(string variableName){
    if(nameToOffsetMap.count(variableName) > 0){
        int variableOffset = nameToOffsetMap[variableName];
        string variableType = nameToTypeMap[variableName];
        char* variableAdress = (char*) startAdress;
        variableAdress = variableAdress + variableOffset;
        if (variableType == "int"){
            int* correctVariableAdress = (int*) variableAdress;
            int variableValue = *correctVariableAdress;
            return to_string(variableValue);
        } else if (variableType == "float"){
            float* correctVariableAdress = (float*) variableAdress;
            float variableValue = *correctVariableAdress;
            return to_string(variableValue);
        } else if (variableType == "double"){
            double* correctVariableAdress = (double*) variableAdress;
            double variableValue = *correctVariableAdress;
            stringstream ss;
            ss << setprecision(15) << variableValue;
            string variableValueDouble;
            ss >> variableValueDouble;
            return variableValueDouble;
        } else if (variableType == "long"){
            long* correctVariableAdress = (long*) variableAdress;
            long variableValue = *correctVariableAdress;
            return to_string(variableValue);
        } else{
            char variableValue = *variableAdress;
            string s;
            s += variableValue;
            return s;
        }
    }else{
        return "ERROR";
    }
}

double checkNumericalValueOfExpression(string stringExpression);
bool checkBooleanValueOfExpression(string stringExpression){
    QString qExpression = QString::fromStdString(stringExpression);
    qExpression.remove(" ");
    if(qExpression.contains("&&")){
        QStringList split = qExpression.split("&&");
        bool value = true;
        for(QString element : split){
            value = (value && checkBooleanValueOfExpression(element.toStdString()));
        } return value;
    } else if(qExpression.contains("||")){
        QStringList split = qExpression.split("||");
        bool value = false;
        for(QString element : split){
            value = (value && checkBooleanValueOfExpression(element.toStdString()));
        } return value;
    } else if(qExpression.contains("==")){
        QStringList split = qExpression.split("==");
        auto a = checkNumericalValueOfExpression(split.at(0).toStdString());
        auto b = checkNumericalValueOfExpression(split.at(1).toStdString());
        return (a == b);
    } else if(qExpression.contains("!=")){
        QStringList split = qExpression.split("==");
        auto a = checkNumericalValueOfExpression(split.at(0).toStdString());
        auto b = checkNumericalValueOfExpression(split.at(1).toStdString());
        return (a != b);
    } else if(qExpression.contains("<")){
        QStringList split = qExpression.split("==");
        auto a = checkNumericalValueOfExpression(split.at(0).toStdString());
        auto b = checkNumericalValueOfExpression(split.at(1).toStdString());
        return (a < b);
    } else if(qExpression.contains(">")){
        QStringList split = qExpression.split("==");
        auto a = checkNumericalValueOfExpression(split.at(0).toStdString());
        auto b = checkNumericalValueOfExpression(split.at(1).toStdString());
        return (a > b);
    } else if(qExpression.contains(">=")){
        QStringList split = qExpression.split("==");
        auto a = checkNumericalValueOfExpression(split.at(0).toStdString());
        auto b = checkNumericalValueOfExpression(split.at(1).toStdString());
        return (a >= b);
    } else if(qExpression.contains("<=")){
        QStringList split = qExpression.split("==");
        auto a = checkNumericalValueOfExpression(split.at(0).toStdString());
        auto b = checkNumericalValueOfExpression(split.at(1).toStdString());
        return (a <= b);
    }
}

double checkNumericalValueOfExpression(string stringExpression){
    QString qExpression = QString::fromStdString(stringExpression);
       qExpression.remove(" ");
       long double ld;
       if(qExpression.contains("+")){
           QStringList split = qExpression.split("+");
           double value = 0;
           for(QString element : split){
               value += checkNumericalValueOfExpression(element.toStdString());
           } return value;
        } else if(qExpression.contains("-")){
           if(qExpression.contains("--")){
               qExpression.replace(QString("--"),QString("+"));
               return checkNumericalValueOfExpression(qExpression.toStdString());
           } else {
               QStringList split = qExpression.split("-");
               double value = checkNumericalValueOfExpression(split.at(0).toStdString());
               split.pop_front();
               for(QString element : split){
                   value -= checkNumericalValueOfExpression(element.toStdString());
               } return value;
           }
       } else if(qExpression.contains("*")){
           QStringList split = qExpression.split("*");
           double value = 1;
           for(QString element : split){
               value *= checkNumericalValueOfExpression(element.toStdString());
           } return value;
       } else if(qExpression.contains("/")){
           QStringList split = qExpression.split("/");
           double value = checkNumericalValueOfExpression(split.at(0).toStdString());
           split.pop_front();
           for(QString element : split){
               value /= checkNumericalValueOfExpression(element.toStdString());
           } return value;
       } else{
           string sExpression = qExpression.toStdString();
           if((std::istringstream(sExpression) >> ld >> std::ws).eof()){
               return stod(sExpression);
           } else{
               string variableValue = getVariableValue(sExpression);
               return stod(variableValue);
           }
       }

   }

vector<string> removeScopeMemory(){
    vector<string> listOfNames;
    for (auto const& element : nameToScopeMap){
        listOfNames.push_back(element.first);
    }
    vector<string> eliminatedVariableNames;
    for (int i = 0; i < (int) listOfNames.size(); i++){
        string variableName = listOfNames[i];
        int scopeOfVariable = nameToScopeMap[variableName];
        cout << "variable: "<< variableName << "   scope: " << scopeOfVariable << "    currentScope: " << currentScope << endl;
        if (scopeOfVariable == currentScope){
            int variableOffset = nameToOffsetMap[variableName];
            string variableType = nameToTypeMap[variableName];
            char* variableAdress = (char*) startAdress;
            variableAdress = variableAdress + variableOffset;
            int sizeOfVariable;
            if (variableType == "int"){
                int* correctVariableAdress = (int*) variableAdress;
                *correctVariableAdress = 0;
                sizeOfVariable = 4;
            } else if (variableType == "float"){
                float* correctVariableAdress = (float*) variableAdress;
                *correctVariableAdress = 0;
                sizeOfVariable = 4;
            } else if (variableType == "double"){
                double* correctVariableAdress = (double*) variableAdress;
                *correctVariableAdress = 0;
                sizeOfVariable = 8;
            } else if (variableType == "long"){
                long* correctVariableAdress = (long*) variableAdress;
                *correctVariableAdress = 0;
                sizeOfVariable = 8;
            } else{
                *variableAdress = '\0';
                sizeOfVariable = 1;
            }
            mainOffset = mainOffset - sizeOfVariable;
            nameToOffsetMap.erase(variableName);
            nameToScopeMap.erase(variableName);
            nameToTypeMap.erase(variableName);
            eliminatedVariableNames.push_back(variableName);
        }
    }
    currentScope = currentScope - 1;
    return eliminatedVariableNames;
}

void analizeBuffer(){
    if (buffer[0] == '{'){
        void* returningAdress;
        json jsonBuffer = json::parse(buffer);
        if (jsonBuffer["scope"] == "Ended"){
            vector<string> eliminatedVariableNames = removeScopeMemory();
            jsonBuffer["value"] = eliminatedVariableNames;
            string sendJson = jsonBuffer.dump();
            cout << sendJson << endl;

            /*
            memset(buffer,0,255);
            strncpy(buffer, sendJson.c_str(),255);
            int n = write(newsockfd,buffer,strlen(buffer));
            if (n < 0){
                error("ERROR writing to socket");
            }
            */
            return;
        }
        if (jsonBuffer["scope"] == "Started"){
            currentScope = currentScope + 1;
            return;
        }
        if (jsonBuffer["ifFlag"] == "true"){
            cout << "it worked" << endl;
            return;
        }
        bool declarationFlag = false;
        if (jsonBuffer["value"] == "NULL"){
            declarationFlag = true;
        }
        if (jsonBuffer["type"] == "NULL"){
            if(nameToOffsetMap.count(jsonBuffer["name"]) > 0){
                string variableType = nameToTypeMap[jsonBuffer["name"]];
                if (variableType == "char"){
                    try {
                        string variableValue = jsonBuffer["value"];
                        if (variableValue.length() == 3 && variableValue.front() == '\'' && variableValue.back() == '\''){
                            int variableOffset = nameToOffsetMap[jsonBuffer["name"]];
                            char* variableAdress = (char*) startAdress;
                            variableAdress = variableAdress + variableOffset;
                            *variableAdress = variableValue[1];
                            jsonBuffer["value"] = variableValue[1];
                            string sendJson = jsonBuffer.dump();
                            memset(buffer,0,255);
                            strncpy(buffer, sendJson.c_str(),255);
                            int n = write(newsockfd,buffer,strlen(buffer));
                            if (n < 0){
                                error("ERROR writing to socket");
                            }
                            return;
                        }else if(nameToOffsetMap.count(variableValue) > 0){
                            string recievedVariableValue = getVariableValue(variableValue);
                            if (recievedVariableValue == "ERROR"){
                                throw variableValue;
                            }
                            char charToStore = recievedVariableValue[0];
                            int variableOffset = nameToOffsetMap[jsonBuffer["name"]];
                            char* variableAdress = (char*) startAdress;
                            variableAdress = variableAdress + variableOffset;
                            *variableAdress = charToStore;
                            jsonBuffer["value"] = recievedVariableValue;
                            string sendJson = jsonBuffer.dump();
                            memset(buffer,0,255);
                            strncpy(buffer, sendJson.c_str(),255);
                            int n = write(newsockfd,buffer,strlen(buffer));
                            if (n < 0){
                                error("ERROR writing to socket");
                            }
                            return;
                        }else{
                            throw variableValue;
                        }
                    }  catch (string variableValue){
                        string storageError ="ERROR   " + variableValue + " is not a valid char.";
                        cout << storageError << endl;
                        memset(buffer,0,255);
                        strncpy(buffer, storageError.c_str(),255);
                        int n = write(newsockfd,buffer,strlen(buffer));
                        if (n < 0){
                            error("ERROR writing to socket");
                        }
                        resetMemory();
                        return;
                    } catch( exception exerror  ) {
                        string strChar(exerror.what());
                        string storageError = "ERROR   " + strChar;
                        cout << storageError << endl;
                        memset(buffer,0,255);
                        strncpy(buffer, storageError.c_str(),255);
                        int n = write(newsockfd,buffer,strlen(buffer));
                        if (n < 0){
                            error("ERROR writing to socket");
                        }
                        resetMemory();
                        return;
                    }
                }else{
                    try {
                        double valueToAssign = checkNumericalValueOfExpression(jsonBuffer["value"]);
                        cout << valueToAssign << endl;
                        int variableOffset = nameToOffsetMap[jsonBuffer["name"]];
                        char* variableAdress = (char*) startAdress;
                        variableAdress = variableAdress + variableOffset;
                        if (variableType == "int"){
                            int* correctVariableAdress = (int*) variableAdress;
                            *correctVariableAdress = valueToAssign;
                            int intValueToAssign = valueToAssign;
                            jsonBuffer["value"] = to_string(intValueToAssign);
                        } else if (variableType == "float"){
                            float* correctVariableAdress = (float*) variableAdress;
                            *correctVariableAdress = valueToAssign;
                            float floatValueToAssign = valueToAssign;
                            jsonBuffer["value"] = to_string(floatValueToAssign);
                        } else if (variableType == "double"){
                            double* correctVariableAdress = (double*) variableAdress;
                            *correctVariableAdress = valueToAssign;
                            stringstream ss;
                            ss << setprecision(15) << valueToAssign;
                            string variableValueDouble;
                            ss >> variableValueDouble;
                            jsonBuffer["value"] = variableValueDouble;
                        } else{
                            long* correctVariableAdress = (long*) variableAdress;
                            *correctVariableAdress = valueToAssign;
                            long longValueToAssign = valueToAssign;
                            jsonBuffer["value"] = to_string(longValueToAssign);
                        }
                        jsonBuffer["value"] = to_string(valueToAssign);
                        string sendJson = jsonBuffer.dump();
                        memset(buffer,0,255);
                        strncpy(buffer, sendJson.c_str(),255);
                        int n = write(newsockfd,buffer,strlen(buffer));
                        if (n < 0){
                            error("ERROR writing to socket");
                        }
                        return;

                    }  catch (exception e) {
                        string strChar(e.what());
                        string storageError = "ERROR: " + strChar;
                        cout << storageError << endl;
                        error("FATAL ERROR: Llame a Jose Retana si esto ocurre");
                    }

                }
            }else{
                string storageError ="ERROR: Variable " + (string)jsonBuffer["name"] + " has not been declared yet";
                cout << storageError << endl;

                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(newsockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }
                resetMemory();
                return;
            }

        }else if (jsonBuffer["type"] == "int"){
            if(nameToOffsetMap.count(jsonBuffer["name"]) > 0){
                string invalidName = jsonBuffer["name"];
                string storageError ="ERROR   " + invalidName + " has already been declared.";
                cout << storageError << endl;
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(newsockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }
                resetMemory();
                return;
            }
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            int* placementAdress = (int*) modifiedVoidPointer;
            if(declarationFlag == false){
                string variableValue = jsonBuffer["value"];
                try {
                    long double ld;
                    if (variableValue.back() == 'f'){
                        variableValue.pop_back();
                        if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                            *placementAdress = stoi(variableValue);
                        }else{
                            throw variableValue;
                        }
                    }else if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                        *placementAdress = stoi(variableValue);
                    }else{
                        double valueOfExpression = checkNumericalValueOfExpression(variableValue);
                        int intToStore = valueOfExpression;
                        *placementAdress = intToStore;
                    }
                }  catch (string variableValue){
                    string storageError ="ERROR   " + variableValue + " is not a valid int.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;

                } catch( exception exerror ) {
                    string strChar(exerror.what());
                    string mainError = "ERROR   " + strChar;
                    cout << mainError << endl;
                    string storageError ="ERROR   " + variableValue + " is not a valid int.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                }
                returningAdress = placementAdress;
                int i = *placementAdress;
                string sInt = to_string(i);
                jsonBuffer["value"] = sInt;
                cout << i << endl;
            } else {
                returningAdress = placementAdress;
            }
        }else if (jsonBuffer["type"] == "float"){
            if(nameToOffsetMap.count(jsonBuffer["name"]) > 0){
                string invalidName = jsonBuffer["name"];
                string storageError ="ERROR   " + invalidName + " has already been declared.";
                cout << storageError << endl;
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(newsockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }
                resetMemory();
                return;
            }
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            float* placementAdress = (float*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            if(declarationFlag == false){
                try {
                    long double ld;
                    if (variableValue.back() == 'f'){
                        variableValue.pop_back();
                        if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                            *placementAdress = stof(variableValue);
                        }else{
                            throw variableValue;
                        }
                    }else if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                        *placementAdress = stof(variableValue);
                    }else{
                        double valueOfExpression = checkNumericalValueOfExpression(variableValue);
                        float floatToStore = valueOfExpression;
                        *placementAdress = floatToStore;
                    }
                }  catch (string variableValue){
                    string storageError ="ERROR   " + variableValue + " is not a valid float.";
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                } catch( exception exerror ) {
                    string strChar(exerror.what());
                    string mainError = "ERROR   " + strChar;
                    cout << mainError << endl;
                    string storageError ="ERROR   " + variableValue + " is not a valid float.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                }
                returningAdress = placementAdress;
                float f = *placementAdress;
                string sFloat = to_string(f);
                jsonBuffer["value"] = sFloat;
                cout << jsonBuffer["value"] << endl;
                cout << f << endl;
            }else{
                returningAdress = placementAdress;
            }
        }else if (jsonBuffer["type"] == "double"){
            if(nameToOffsetMap.count(jsonBuffer["name"]) > 0){
                string invalidName = jsonBuffer["name"];
                string storageError ="ERROR   " + invalidName + " has already been declared.";
                cout << storageError << endl;
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(newsockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }
                resetMemory();
                return;
            }
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            double* placementAdress = (double*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            if(declarationFlag == false){
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
                        double valueOfExpression = checkNumericalValueOfExpression(variableValue);
                        *placementAdress = valueOfExpression;
                    }
                }  catch (string variableValue){
                    string storageError ="ERROR   " + variableValue + " is not a valid double.";
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                } catch( exception exerror ) {
                    string strChar(exerror.what());
                    string mainError = "ERROR   " + strChar;
                    cout << mainError << endl;
                    string storageError ="ERROR   " + variableValue + " is not a valid double.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                }
                returningAdress = placementAdress;
                double d = *placementAdress;
                stringstream ss;
                ss << setprecision(15) << d;
                ss >> jsonBuffer["value"];
                cout << jsonBuffer["value"] << endl;
                cout << d << endl;
            }else{
                returningAdress = placementAdress;
            }
        } else if (jsonBuffer["type"] == "long"){
            if(nameToOffsetMap.count(jsonBuffer["name"]) > 0){
                string invalidName = jsonBuffer["name"];
                string storageError ="ERROR   " + invalidName + " has already been declared.";
                cout << storageError << endl;
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(newsockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }
                resetMemory();
                return;
            }
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            long* placementAdress = (long*) modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            if(declarationFlag == false){
                try {
                    long double ld;
                    if ((std::istringstream(variableValue) >> ld >> std::ws).eof()){
                        *placementAdress = stol(variableValue);
                    }else{
                        double valueOfExpression = checkNumericalValueOfExpression(variableValue);
                        long floatToStore = valueOfExpression;
                        *placementAdress = floatToStore;
                    }
                }  catch (string variableValue){
                    string storageError ="ERROR   " + variableValue + " is not a valid long.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                } catch( exception exerror) {
                    string strChar(exerror.what());
                    string mainError = "ERROR   " + strChar;
                    cout << mainError << endl;
                    string storageError ="ERROR   " + variableValue + " is not a valid long.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                }
                returningAdress = placementAdress;
                long l = *placementAdress;
                string sLong = to_string(l);
                jsonBuffer["value"] = sLong;
                cout << l << endl;
            }else{
                returningAdress = placementAdress;
            }
        } else if (jsonBuffer["type"] == "char"){
            if(nameToOffsetMap.count(jsonBuffer["name"]) > 0){
                string invalidName = jsonBuffer["name"];
                string storageError ="ERROR   " + invalidName + " has already been declared.";
                cout << storageError << endl;
                memset(buffer,0,255);
                strncpy(buffer, storageError.c_str(),255);
                int n = write(newsockfd,buffer,strlen(buffer));
                if (n < 0){
                    error("ERROR writing to socket");
                }
                resetMemory();
                return;
            }
            char* modifiedVoidPointer = (char*)  startAdress + mainOffset;
            char* placementAdress = modifiedVoidPointer;
            string variableValue = jsonBuffer["value"];
            if(declarationFlag == false){
                try {
                    if (variableValue.length() == 3 && variableValue.front() == '\'' && variableValue.back() == '\''){
                        *placementAdress = variableValue[1];
                    }else if(nameToOffsetMap.count(variableValue) > 0){
                        string recievedVariableValue = getVariableValue(variableValue);
                        if (recievedVariableValue == "ERROR"){
                            throw variableValue;
                        }
                        char charToStore = recievedVariableValue[0];
                        *placementAdress = charToStore;
                    }
                }  catch (string variableValue){
                    string storageError ="ERROR   " + variableValue + " is not a valid char.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                } catch( exception exerror  ) {
                    string strChar(exerror.what());
                    string mainError = "ERROR   " + strChar;
                    cout << mainError << endl;
                    string storageError ="ERROR   " + variableValue + " is not a valid long.";
                    cout << storageError << endl;
                    memset(buffer,0,255);
                    strncpy(buffer, storageError.c_str(),255);
                    int n = write(newsockfd,buffer,strlen(buffer));
                    if (n < 0){
                        error("ERROR writing to socket");
                    }
                    resetMemory();
                    return;
                }
                returningAdress = placementAdress;
                char c = *placementAdress;
                string sChar;
                sChar += c;
                jsonBuffer["value"] = sChar;
                cout << c << endl;
            }else{
                returningAdress = placementAdress;
            }
        }
        string variableScopeString = jsonBuffer["scope"];
        int variableScope = stoi(variableScopeString);
        cout << variableScope << endl;
        string variableName = jsonBuffer["name"];
        string variableType = jsonBuffer["type"];
        int offset = mainOffset;
        mainOffset = mainOffset + (int)jsonBuffer["size"];
        nameToOffsetMap.insert(pair<string, int>(variableName,offset));
        nameToTypeMap.insert(pair<string, string>(variableName,variableType));
        nameToScopeMap.insert(pair<string,int>(variableName,variableScope));
        std::stringstream ss;
        ss << returningAdress;
        string returningAdressString = ss.str();
        jsonBuffer["adress"] = returningAdressString;
        jsonBuffer["referenceCounter"] = "1";
        string sendJson = jsonBuffer.dump();
        cout << startAdress << endl;
        cout << returningAdress << endl;
        memset(buffer,0,255);
        strncpy(buffer, sendJson.c_str(),255);
        int n = write(newsockfd,buffer,strlen(buffer));
        if (n < 0){
            error("ERROR writing to socket");
        }
   }
}


void readBuffer(){
    memset(buffer, 0, 255);
    cout << "waiting for message..." << endl;
    int n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Message received: %s\n",buffer);
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

    while(true){
        readBuffer();
        analizeBuffer();
    }

    return a.exec();
}
