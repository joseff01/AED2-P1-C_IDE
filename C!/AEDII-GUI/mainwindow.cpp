#include "mainwindow.h"
#include "ui_mainwindow.h"

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

QStringList identifyName(QString text, bool flag)
{
    QChar cEquals('='); QChar cSpace(' '); QChar cSpecial(';');
    QString nameString; QString str; QString valueString;
    QStringList list;

    for(int i = 0; i<text.size();i++)
    {
        if ((text.at(i))== cEquals && flag)
        {
            nameString = str;
            str.clear();
            flag = false;
        }
        if ((text.at(i))== cSpecial)
        {
            valueString = str;
        }
        else
        {
           if(text.at(i) != cSpace)
           {
               str.append(text.at(i));
           }
        }

    }
    list.append(nameString);
    list.append(valueString);
    return list;

}

QStringList identifyStart(QString text)
{
    QChar cI('i'); QChar cN('n'); QChar cT('t'); QChar cSpecial(';'); QChar cL('l'); QChar cO('o'); QChar cG('g'); QChar cSpace(' ');QChar cC('c');QChar cA('a');QChar cR('r');QChar cH('h');QChar cF('f');QChar cD('d');QChar cS('s');QChar cU('u');QChar cB('b');QChar cE('e');
    QString typeString;
    QStringList packageList; QStringList temp;

    for(int i = 0; i<text.size();i++)
    {
        if((text.at(i))== cI && (text.at(i+1))== cN && ((text.at(i+2))== cT)&& ((text.at(i+3))== cSpace))
        {
            typeString = "int";
            //text.remove(0,2);
            //temp = identifyName(text,true);
            //i=+4;

        }
        if((text.at(i))== cL && (text.at(i+1))== cO && ((text.at(i+2))== cN) && ((text.at(i+3))== cG) && ((text.at(i+4))== cSpace))
        {
            typeString  = "long";
            text.remove(0,3);
            temp = identifyName(text,true);
            i=+5;

        }
        if((text.at(i))== cC && (text.at(i+1))== cH && ((text.at(i+2))== cA) && ((text.at(i+3))== cR) && ((text.at(i+4))== cSpace))
        {
            typeString  = "char";
            text.remove(0,3);
            temp = identifyName(text,true);
            i=+5;

        }
        if((text.at(i))== cF && (text.at(i+1))== cL && ((text.at(i+2))== cO) && ((text.at(i+3))== cA) && ((text.at(i+4))== cT) && ((text.at(i+5))== cSpace))
        {
            typeString  = "float";
            text.remove(0,4);
            temp = identifyName(text,true);
            i=+6;

        }
        if((text.at(i))== cD && (text.at(i+1))== cO  && ((text.at(i+2))== cU)&& ((text.at(i+3))== cB) && ((text.at(i+4))== cL) && ((text.at(i+5))== cE) && ((text.at(i+6))== cSpace))
        {
            typeString ="double";
            text.remove(0,4);
            temp = identifyName(text,true);
            i=+7;
        }
        else
        {
            typeString  = "NULL";
            temp = identifyName(text,true);
        }
    }

    packageList.append(typeString);
    //packageList.append(temp.at(0));
    //packageList.append(temp.at(1));
    return packageList;

}

void MainWindow::on_pushButton_clicked()
//int a = 5;
{
    QString text = ui->textEdit->toPlainText();
    QStringList list = text.split(QLatin1Char(';'));
    QStringList package; QString prueba;

    for(int i =0; i<list.size()-1;i++)
    {
        package = identifyStart(list.at(i));
        for(int i =0; i<package.size();i++)
        {
           prueba.append(package.at(i));
           ui->applicationLogTextEdit->setPlainText(prueba);
        }
    }
}





    /**
        for(int i =0; i<package.size();i++)
        {
           prueba.append(package.at(i));
           ui->applicationLogTextEdit->setPlainText(prueba);
        }

      for (inicio string, inicio = largo string, i++);

           1. particion del Qstring y lo colocamos en una lista. (lista = [int a = 1, adaw = awda,])
           2. irse al inicio fijarse que es el tipo y lo ponemos en un string, si no tiene tipo enviarlo como null.
           3. Despues de identificar el tipo ver lo que esta a la derecha de = y cuardarlo como nombre en otro string.
           4. Guardar


        if((text.at(i))== cSpecial){
            ui->applicationLogTextEdit->setPlainText(appendText.append(str));
        }
        else{ str.append(text.at(i));}
        */

    /**
    QString s = QString::number(text.length());
    ui->applicationLogTextEdit->setPlainText(s);
     */

