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


using json = nlohmann::json;

//Server Connection Glstdio::obal Variables:
int sockfd;
char buffer[255];
void serverError(const char *msg);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->scopeNum = 0;
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

QStringList MainWindow::identifyStart(QString text)
{
    QString nameType;
    QString value;
    QString type;
    QString name;
    QString scope;

    //Scope  definition
    if(text.contains("{")){
        this->setScopeNum(this->getScopeNum()+1);
        text.remove("{");
    } else if(text.contains("}")){
        this->setScopeNum(this->getScopeNum()-1);
        text.remove("}");
    } scope = QString::fromStdString(std::to_string(this->getScopeNum()));

    //Division by =
    if(text.contains("=",Qt::CaseSensitive)){
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
    } else if(nameType.contains("reference",Qt::CaseSensitive)){
        type = "reference";
        name = nameType.remove("reference",Qt::CaseSensitive).remove(" ").remove("\n");
    } else if(nameType.contains("float",Qt::CaseSensitive)){
        type = "float";
        name = nameType.remove("float",Qt::CaseSensitive).remove(" ").remove("\n");
    } else {
        type = "NULL";
        name = nameType.remove(" ").remove("\n");
    }

    QStringList package;
    package << type << name << value << scope;;
    return package;

}

void MainWindow::on_pushButton_clicked()
{
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
void MainWindow::setMainList(std::list<QStringList> newList){this->mainList = newList;}
std::list<QStringList> MainWindow::getMainList(){ return this->mainList;}
int MainWindow::getScopeNum(){return this->scopeNum;}
void MainWindow::setScopeNum(int scopeNum){this->scopeNum = scopeNum;}

void MainWindow::on_nextButton_clicked()
{
    if(!this->getMainList().empty()){
        QStringList package = this->getMainList().front();
        std::list<QStringList> temp = this->getMainList();
        temp.pop_front();
        this->setMainList(temp);
        QString type = package.at(0);

        json j;
        j["type"] = type.toStdString();
        j["name"] = package.at(1).toStdString();
        j["value"] = package.at(2).toStdString();
        j["scope"] = package.at(3).toStdString();

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
            string jsonString = j.dump();
            QString display = QString::fromStdString(jsonString);
            ui->terminalTextEdit->append("\n"+display);
        }

        /*
        memset(buffer,0,255);
        string jsonString = j.dump();
        QString display = QString::fromStdString(jsonString);
        ui->terminalTextEdit->append("\n"+display);
        strncpy(buffer, jsonString.c_str(),255);
        int n = write(sockfd,buffer,strlen(buffer));
        if (n < 0){
            serverError("ERROR writing to socket");
        }*/
    }
}
void MainWindow::cout(string newText){
    QString textDisplay = QString::fromStdString(newText);
    textDisplay.insert(0,' ');
    textDisplay.insert(0,'>');
    ui->applicationLogTextEdit->append(textDisplay);
}
void MainWindow::ramView(string memory, string value, string name, string reference){
    QString memoryDisplay = QString::fromStdString(memory);
    QString valueDisplay = QString::fromStdString(value);
    QString nameDisplay = QString::fromStdString(name);
    QString referenceDisplay = QString::fromStdString(reference);
    ui->memoryTextEdit->append(memoryDisplay);
    ui->valueTextEdit->append(valueDisplay);
    ui->nameTextEdit->append(nameDisplay);
    ui->referenceTextEdit->append(referenceDisplay);
}

void serverError(const char *msg)
{
    perror(msg);
    exit(1);
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
    /*
    std::cout << "Connection established." << std::endl;
    /*
    memset(buffer,0,255);
    string testString = "The quick brown fox jumps over the lazy dog";
    strncpy(buffer, testString.c_str(),255);
    int n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){
        serverError("ERROR writing to socket");
    }*/
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
    ui->memoryTextEdit->clear();
    ui->valueTextEdit->clear();
    ui->nameTextEdit->clear();
    ui->referenceTextEdit->clear();

}
