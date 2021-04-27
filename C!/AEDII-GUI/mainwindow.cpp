#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <list>
#include "json.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

/*
 * struct book {
int a;
int b;
}

book pito;

pito.a =3;
pito.b =4;
*/

using json = nlohmann::json;

//Server Connection Glstdio::obal Variables:
int sockfd;
char buffer[255];
void serverError(const char *msg);
//Scope variables
int lastScope;
int whileNum = -1;
int max = 5;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->scopeNum = 0;
    lastScope = this->getScopeNum();
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    ::close(sockfd);
    delete ui;
}

/*
* QStringList identifyStatrt(QString):
* Receives a QString
* Returns a list with the identified type,name and value.
*/

string MainWindow::ifAndElse(QString text,bool isWhile){
    QString textSend = text.remove("(");
    json sendText;
    sendText["type"] = "NULL";
    sendText["name"] = "NULL";
    sendText["value"] = textSend.toStdString();
    sendText["scope"] = "NULL";
    sendText["ifFlag"] = "true";


    memset(buffer,0,255);
    string jsonString = sendText.dump();
    QString display = QString::fromStdString(jsonString);
    ui->terminalTextEdit->append("\n"+display);

    strncpy(buffer, jsonString.c_str(),255);
    int n = write(sockfd,buffer,strlen(buffer));
    ui->applicationLogTextEdit->append("INFO       Sending Json verification to server");
    if (n < 0){
        serverError("ERROR writing to socket");
    }
    ui->applicationLogTextEdit->append("INFO       message verification recieved from server");
    memset(buffer, 0, 255);
    int num = read(sockfd,buffer,255);
    if (num < 0) serverError("ERROR reading from socket");

    ui->applicationLogTextEdit->append("WARN    parsing message from the server");
    if (buffer[0] == '{')
    {
        json jsonBuffer = json::parse(buffer);
        string value = jsonBuffer["value"];

        if("true"==value){
            if(!isWhile){
                setTrueIf(true);
            }return "true";
        }
        else{
            if(!isWhile){
                setTrueIf(false);
            }return "false";
        }
    }
    else{
        string newstr(buffer);
        QString str = QString::fromStdString(newstr);
        ui->applicationLogTextEdit->append(str);
        return "error";
    }
}

QStringList MainWindow::identifyStart(QString text)
{
    QString nameType; //Struct book { int a =3
    QString value;
    QString type;
    QString name;
    QString scope;
    QString contains = "Null";
    QString endScope = "false";
    QString structName = "Null";
    QString whileContains = "Null";
    QString couText;
    QString pointFlag = "false";

    //While condition
    /*
    if(text.contains("while")){
        if(text.startsWith("{")){
            this->setScopeNum(this->getScopeNum()+1);
            text.remove(0,1);
        }
        //Getting condition
        std::cout << text.toStdString() << std::endl;
        QString condition = text.mid(text.indexOf("("),text.indexOf(")"));
        std::cout << condition.toStdString() << std::endl;
        text.remove(0,text.indexOf("=)")+1);
        //Send condition to server
        //Get  server response
        bool serverResponse;
        if(serverResponse){

        }
    }
    */

    //If definition
    if (text.contains("if")){
        text.remove("if(");
        QStringList ifSplit = text.split("{");
        contains = ifSplit.at(0);
        contains.remove(")");
        text = text.mid(text.indexOf("{"));

    }
    if(text.contains("struct",Qt::CaseSensitive)){
        QStringList structList = text.split("{");
        structName = structList.at(0);
        structName = structName.remove("struct");
        text = text.mid(text.indexOf("{"));

    }
    if (text.contains("while")){
        text.remove("while(");
        QStringList ifSplit = text.split("{");
        whileContains = ifSplit.at(0);
        whileContains.remove(")");
        text = text.mid(text.indexOf("{"));
    }
    if(text.contains("reference")){
        text.remove("reference<").remove(">");
        pointFlag = "true";

    }

    //Scope  definition
    if(text.contains("{")){
        this->setScopeNum(this->getScopeNum()+1);
        text.remove("{");
    } else if(text.contains("}")){
        this->setScopeNum(this->getScopeNum()-1);
        text.remove("}");
        endScope = "true";
    } scope = QString::fromStdString(std::to_string(this->getScopeNum()));

    //Division by =
    if(text.contains("cout")){
        QString temp = text;
        temp.remove(temp.lastIndexOf(")"),temp.lastIndexOf(")"));
        temp.remove("cout(");
        value = temp;
        nameType = text;
    }else if(text.contains("=",Qt::CaseSensitive)){
        QStringList equalSplit = text.split("=");
        nameType = equalSplit.at(0);
        value = equalSplit.at(1);
    } else {
        nameType = text;
        value = "NULL";
    }

    if(nameType.contains("int",Qt::CaseSensitive)){
        type = "int";
        name = nameType.remove("int",Qt::CaseSensitive).remove(" ").remove("\n");
    } else if(nameType.contains("double",Qt::CaseSensitive)){
        type = "double";
        name = nameType.remove("double",Qt::CaseSensitive).remove(" ").remove("\n");
    } else if(nameType.contains("long",Qt::CaseSensitive)){
        type = "long";
        name = nameType.remove("long",Qt::CaseSensitive).remove(" ").remove("\n");
    } else if(nameType.contains("char",Qt::CaseSensitive)){
        type = "char";
        name = nameType.remove("char",Qt::CaseSensitive).remove(" ").remove("\n");
    } else if(nameType.contains("float",Qt::CaseSensitive)){
        type = "float";
        name = nameType.remove("float",Qt::CaseSensitive).remove(" ").remove("\n");
    }else if(nameType.contains("cout",Qt::CaseSensitive)){
        type = "cout";
        name = "NULL";
    }else {
        type = "NULL";
        name = nameType.remove(" ").remove("\n");

    }
    if (nameType.contains("else")){
        contains = "else";
    }

    QStringList package;
    package << type << name << value << scope<<contains<<endScope<<structName<<whileContains<<pointFlag;;
    QString tempName =package.join("\n");
    ui->applicationLogTextEdit->setText(tempName);


    return package;
}



void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setDisabled(true);
    //Gettingg text from editor
    QString text = ui->textEdit->toPlainText();
    QStringList list = text.split(QLatin1Char(';'));
    QStringList package;
    std::list<QStringList> mainList;

    for(int i =0; i<list.size();i++)
    {
        QString line = list.at(i);
        line = line.remove("\n").remove(" ");
        if (line != "")
        {
            package = identifyStart(line);
            mainList.push_back(package);
        }
    }
    this->setMainList(mainList);
}

void serverError(const char *msg)
{
    perror(msg);
    exit(1);
}

void MainWindow::ramView(QString memory, QString value, QString name, QString reference){
    ui->memoryTextEdit->append(memory);
    ui->valueTextEdit->append(value);
    ui->nameTextEdit->append(name);
    ui->referenceTextEdit->append(reference);
}

void MainWindow::readBuffer(){
    ui->applicationLogTextEdit->append("INFO       message recieved from server");
    memset(buffer, 0, 255);
    int n = read(sockfd,buffer,255);
    if (n < 0) serverError("ERROR reading from socket");

    if (buffer[0] == '{'){
        ui->applicationLogTextEdit->append("WARN    parsing message from the server");
        json jsonBuffer = json::parse(buffer);
        std::cout << jsonBuffer<<endl;
        string referenceFlag = jsonBuffer["referenceFlag"];


        if(referenceFlag == "true1"){
            vector<string> names = jsonBuffer["value"];
            vector<string> addresses = jsonBuffer["adress"];
            vector<string> references = jsonBuffer["referenceCounter"];

            int i;
            for(i=0;i<names.size();i++){
                QString variableName = QString::fromStdString(names[i]);
                QString variableAddresses = QString::fromStdString(addresses[i]);
                QString variableReference = QString::fromStdString(references[i]);
                ui->nameTextEdit->append(variableName);
                ui->memoryTextEdit->append(variableAddresses);
                ui->valueTextEdit->append("NULL");
                ui->referenceTextEdit->append(variableReference);
            }
        }
        else if(referenceFlag == "true2"){
            QString referenceText= ui->referenceTextEdit->toPlainText();
            QStringList referenceList = referenceText.split("\n");
            QString memory = ui->memoryTextEdit->toPlainText();
            QStringList memoryList = memory.split("\n");
            vector<string> addresses = jsonBuffer["adress"];

            int i;
            for(i=0;i<addresses.size();i++){
                QString variableAddresses = QString::fromStdString(addresses[i]);
                string numString = referenceList[memoryList.indexOf(variableAddresses)].toStdString();
                int num = stoi(numString);
                num =num+1;
                referenceList[memoryList.indexOf(variableAddresses)] = QString::number(num);
                QString temp =referenceList.join("\n");
                ui->referenceTextEdit->setText(temp);
            }


        }else{
            QString name = QString::fromStdString(jsonBuffer["name"]);
            QString memory = QString::fromStdString(jsonBuffer["adress"]);
            QString type = QString::fromStdString(jsonBuffer["type"]);
            QString text = ui->nameTextEdit->toPlainText();
            QStringList textList = text.split("\n");

        if(textList.contains(name)){
            QString value = QString::fromStdString(jsonBuffer["value"]);
            QString valueText= ui->valueTextEdit->toPlainText();
            QStringList valueList = valueText.split("\n");
            valueList[textList.indexOf(name)]= value;
            QString temp =valueList.join("\n");
            ui->valueTextEdit->setText(temp);

        }else if(type =="cout"){
            cout(jsonBuffer["value"]);
        }
        else if(memory == "freeing space"){
            vector<string> value = jsonBuffer["value"];
            QString valueText= ui->valueTextEdit->toPlainText();
            QString nameText= ui->nameTextEdit->toPlainText();
            QString memoryText= ui->memoryTextEdit->toPlainText();
            QString referenceText= ui->referenceTextEdit->toPlainText();

            QStringList valueList = valueText.split("\n");
            QStringList nameList = nameText.split("\n");
            QStringList memoryList = memoryText.split("\n");
            QStringList referenceList = referenceText.split("\n");

            int i;
            for(i=0;i<value.size();i++){
                QString val = QString::fromStdString(value[i]);
                int num = nameList.indexOf(val);
                nameList.removeAt(num);
                valueList.removeAt(num);
                memoryList.removeAt(num);
                referenceList.removeAt(num);

            }
            QString tempName =nameList.join("\n");
            QString tempValue =valueList.join("\n");
            QString tempMemory =memoryList.join("\n");
            QString tempReference =referenceList.join("\n");
            ui->nameTextEdit->setText(tempName);
            ui->valueTextEdit->setText(tempValue);
            ui->memoryTextEdit->setText(tempMemory);
            ui->referenceTextEdit->setText(tempReference);
        }
        else{
            QString value = QString::fromStdString(jsonBuffer["value"]);
            QString memory = QString::fromStdString(jsonBuffer["adress"]);
            QString reference = QString::fromStdString(jsonBuffer["referenceCounter"]);
            ramView(memory, value, name, reference);
        }
        }
    }
    else{
        string newstr(buffer);
        QString str = QString::fromStdString(newstr);
        ui->applicationLogTextEdit->append(str);
        on_deleteButton_clicked();
    }

}
void MainWindow::structJson(std::list<QStringList> structList, string structName){
    vector<string> types;
    vector<string> values;

    QStringList structNameList = this->getStructName();
    structNameList.push_back(QString::fromStdString(structName));
    setStructName(structNameList);

    for(QStringList package:structList){
        if(!(package.at(0)=="NULL"&& package.at(1)=="")){
            types.push_back(package.at(0).toStdString());
            values.push_back(package.at(1).toStdString());
        }
    }
    json j;
    j["value"] = values;
    j["type"] =types;
    j["name"] = structName;
    j["struct"] = true;
    ui->terminalTextEdit->append(QString::fromStdString(j.dump()));

    memset(buffer,0,255);
    string jsonString = j.dump();
    QString display = QString::fromStdString(jsonString);
    ui->terminalTextEdit->append("\n"+display);
    strncpy(buffer, jsonString.c_str(),255);
    int n = write(sockfd,buffer,strlen(buffer));
    ui->applicationLogTextEdit->append("INFO       Sending Json to server");
    if (n < 0){
        serverError("ERROR writing to socket");
    }
}


void MainWindow::on_nextButton_clicked()
{
    if(!this->getMainList().empty()){

        QStringList package = this->getMainList().front();
        std::list<QStringList> temp = this->getMainList();
        temp.pop_front();
        this->setMainList(temp);
        QString type = package.at(0);
        QString name = package.at(1);
        QString cont = package.at(4);
        QString endScope = package.at(5);
        QString structName = package.at(6);
        QString containsWhile= package.at(7);

        /*bool  whileFlag;
        int num;
        int max = 5;

        if(hay un while){
        enviarle la vara a jose para que nos idque  true o false;
        if whileflag activado{
          num = num+1;
          } else avanzar hasta lastScope.
        }

        if (num =!-1){ // {int a = a+1; while(algo){ b = b+1; c = c+3;}}
        int i;
        for(int i = 0; i<=num; i++){ agregar package a todas las listas}
        if(hay endscope){
            agregar whileList.at(num) a mainList;
            eliminar whilelist.at(num)
            num -1;
          }
        }*/

        if(containsWhile!="Null"){
            string booleanWhile = ifAndElse(containsWhile, true);
            if(booleanWhile =="true"){
                whileNum = whileNum +1;
                std::cout << "Num +1 " << std::endl;
            }else{
                QStringList currentPackage = this->getMainList().front();
                QString currentEndScope = currentPackage.at(5);
                QString currentScope = currentPackage.at(3);

                while(currentEndScope != "true" && currentScope != package.at(3)){
                    std::list<QStringList> tempList = this->getMainList();
                    tempList.pop_front();
                    this->setMainList(tempList);
                    currentPackage = this->getMainList().front();
                    currentScope = currentPackage.at(5);
                    currentScope = currentPackage.at(3);
                } return;
            }
        } if(whileNum > -1){
            std::vector<std::vector<QStringList>> whileVector = this->getWhileVector();
            int whileVectorSize = whileVector.size();
            if(whileVectorSize-1 < whileNum){

                std::vector<QStringList> element;
                whileVector.push_back(element);
            }
            whileVectorSize = whileVector.size();
            for(int i = 0; i < whileVectorSize; i++){
                whileVector[i].push_back(package);

            } this->setWhileVector(whileVector);
            if(package.at(5) == "true"){

                std::list<QStringList> newMainList = this->getMainList();;
                std::vector<QStringList> element = this->getWhileVector()[whileNum];

                int elementSize = element.size();
                for(int i = 0;i < elementSize; i++){
                    newMainList.push_back(element[i]);
                } this->setMainList(newMainList);

                std::vector<std::vector<QStringList>> whileVector = this->getWhileVector();
                whileVector[whileNum].clear();
                this->setWhileVector(whileVector);
                whileNum -=1 ;
            }
        }
        if(structName!="Null"){
            std::list<QStringList> structList;
            structList.push_back(package);

            QStringList currentPackage = this->getMainList().front();
            QString currentScope = currentPackage.at(5);

            while(currentScope != "true"){

                std::list<QStringList> tempList = this->getMainList();
                tempList.pop_front();
                this->setMainList(tempList);
                structList.push_back(currentPackage);
                QStringList currentPackage = this->getMainList().front();
                currentScope = currentPackage.at(5);

            } structJson(structList,structName.toStdString());
        }else{
            if(cont.contains("else")){
                name.remove("else").remove("{").remove("}");
                package[1]= name;
                if(getTrueIf()){
                    setTrueIf(false);
                }
                else{
                    setTrueIf(true);
                }
            } else if(cont != "Null"){
                ifAndElse(cont, false);
            }
            if(endScope.contains("true")){ setTrueIf(true);}

            if(getTrueIf())
            {
                json j;
                j["type"] = "NULL";
                j["name"] = "NULL";
                j["value"] = "NULL";
                j["ifFlag"] = "false";

                if(lastScope > package.at(3).toInt()){
                    j["scope"] = "Ended";
                    memset(buffer,0,255);
                    string jsonString = j.dump();
                    QString display = QString::fromStdString(jsonString);
                    ui->terminalTextEdit->append("\n"+display);
                    strncpy(buffer, jsonString.c_str(),255);
                    int n = write(sockfd,buffer,strlen(buffer));
                    ui->applicationLogTextEdit->append("INFO       Sending Json to server");
                    if (n < 0){
                        serverError("ERROR writing to socket");
                    }readBuffer();
                }
                else if(lastScope < package.at(3).toInt()){
                    j["scope"] = "Started";
                    memset(buffer,0,255);
                    string jsonString = j.dump();
                    QString display = QString::fromStdString(jsonString);
                    ui->terminalTextEdit->append("\n"+display);
                    strncpy(buffer, jsonString.c_str(),255);
                    int n = write(sockfd,buffer,strlen(buffer));
                    ui->applicationLogTextEdit->append("INFO       Sending Json to server");

                    if (n < 0){
                        serverError("ERROR writing to socket");
                    }
                }lastScope = package.at(3).toInt();

                j;
                j["type"] = type.toStdString();
                j["name"] = package.at(1).toStdString();
                j["value"] = package.at(2).toStdString();
                j["scope"] = package.at(3).toStdString();
                j["ifFlag"] = "false";
                if(package.at(8)=="true")
                { j["pointFlag"]= true;}
                else{j["pointFlag"]= false;}

                for(QString structName:getStructName()){
                    if(package.at(1).contains(structName)){
                        QString newStructName = package.at(1);
                        j["name"] = newStructName.remove(structName).toStdString();
                        j["type"] = structName.toStdString();
                        ui->applicationLogTextEdit->setText(newStructName.remove(structName));
                        ui->applicationLogTextEdit->append(structName);

                    }
                }

                if(type.contains("int",Qt::CaseSensitive)){
                    j["size"] = 4;
                } else if(type.contains("double",Qt::CaseSensitive)){
                    j["size"] = 8;
                } else if(type.contains("long",Qt::CaseSensitive)){
                    j["size"] = 8;
                } else if(type.contains("char",Qt::CaseSensitive)){
                    j["size"] = 1;
                } else if(type.contains("reference",Qt::CaseSensitive)){
                    j["size"] = 4;
                } else if(type.contains("float",Qt::CaseSensitive)){
                    j["size"] = 4;
                } else {
                    j["size"] = "NULL";
                }
                if(j.at("name") != ""){
                    memset(buffer,0,255);
                    string jsonString = j.dump();
                    QString display = QString::fromStdString(jsonString);
                    ui->terminalTextEdit->append("\n"+display);
                    strncpy(buffer, jsonString.c_str(),255);
                    int n = write(sockfd,buffer,strlen(buffer));
                    ui->applicationLogTextEdit->append("INFO       Sending Json to server");
                    if (n < 0){
                        serverError("ERROR writing to socket");
                    }
                    readBuffer();
                }
            }
        }
    }
    else {ui->pushButton->setDisabled(false);}
}

void MainWindow::cout(string newText){
    QString textDisplay = QString::fromStdString(newText);
    textDisplay.insert(0,' ');
    textDisplay.insert(0,'>');
    ui->applicationLogTextEdit->append(textDisplay);
}



void connectToMServer(int portno){
    int option = 1;
    struct sockaddr_in serv_addr;
    char const *localHost = "localhost";
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&option,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    if (sockfd < 0)
      serverError("ERROR opening socket");
    server = gethostbyname(localHost);
    if (server == NULL){
      fprintf(stderr,"ERROR, no such host");
      exit(0);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    std::cout << "Connecting to mserver..." << std::endl;
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        serverError("ERROR connecting");
    }
}

void MainWindow::on_backButton_clicked() {
    QString text = ui->portTextEdit->toPlainText();
    string strText = text.toStdString();
    int socketNum = std::stoi(strText);
    connectToMServer(socketNum);

    ui->portWidget->hide();
    ui->textEdit->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
}

void MainWindow::on_deleteButton_clicked()
{
    ui->textEdit->clear();
    ui->memoryTextEdit->setText("Memory");
    ui->valueTextEdit->setText("Value");
    ui->nameTextEdit->setText("Name");
    ui->referenceTextEdit->setText("Reference");

}


QStringList MainWindow::getStructName(){return this->structNames;}
void MainWindow::setStructName(QStringList list){this->structNames=list;}
bool MainWindow::getTrueIf(){return this->trueIf;}
void MainWindow::setTrueIf(bool flag){this->trueIf = flag;}
bool MainWindow::getStopFlag(){return this->stopFlag;}
void MainWindow::setStopFlag(bool flag){this->stopFlag = flag;}
void MainWindow::setMainList(std::list<QStringList> newList){this->mainList = newList;}
std::list<QStringList> MainWindow::getMainList(){ return this->mainList;}
int MainWindow::getScopeNum(){return this->scopeNum;}
void MainWindow::setScopeNum(int scopeNum){this->scopeNum = scopeNum;}
std::list<std::list<QStringList>> MainWindow::getWhileList(){return this->whileList;}
void MainWindow::setWhileList(std::list<std::list<QStringList>> list){this->whileList=list;}
std::vector<std::vector<QStringList>> MainWindow::getWhileVector(){return this->whileVector;}
void MainWindow::setWhileVector(std::vector<std::vector<QStringList>> vector){this->whileVector=vector;};
