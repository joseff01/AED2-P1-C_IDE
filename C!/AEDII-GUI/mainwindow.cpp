#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <list>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //  Caracteristicas de los textEdit
    ui->applicationLogTextEdit->setReadOnly(true);
    ui->terminalTextEdit ->setReadOnly(true);
    ui->viewTextEdit->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
* QStringList identifyStatrt(QString):
* Receives a QString
* Returns a list with the identified type,name and value.
*/
QStringList identifyStart(QString text)
{ 

    QString nameType;
    QString value;
    QString type;
    QString name;

    //Division por =
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
    package << type << name << value;;
    return package;

}

void MainWindow::on_pushButton_clicked()
//int a = 5;
{
    //Gettingg text from editor
    QString text = ui->textEdit->toPlainText();
    QStringList list = text.split(QLatin1Char(';'));
    QStringList package;
    std::list<QStringList> mainList;

    //Making text readable for server
    /*
    string size  = std::to_string(list.size());
    QString qsize =  QString::fromStdString(size);
    QString display;*/

    for(int i =0; i<list.size();i++)
    {
        QString line = list.at(i);
        line = line.remove("\n").remove(" ");

        // display.append(line + "\n").remove(" ");

        if (line != ""){
            package = identifyStart(line);
            mainList.push_back(package);
            /*
            display.append(package.at(0) + ",");
            display.append(package.at(1) + ",");
            display.append(package.at(2) + ",");
            display.append("\n");
            */
        }
    }
    //ui->applicationLogTextEdit->setPlainText(display);
    this->setMainList(mainList);

}
void MainWindow::setMainList(std::list<QStringList> newList){this->mainList = newList;}
std::list<QStringList> MainWindow::getMainList(){ return this->mainList;}

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

        QString display = QString::fromStdString(j.dump());
        ui->applicationLogTextEdit->setText(display);
    } else {ui->applicationLogTextEdit->setPlainText("Execution Done");}
}



void MainWindow::on_backButton_clicked()
{
    ui->portWidget->hide();
}
