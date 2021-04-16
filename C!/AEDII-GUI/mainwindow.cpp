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

