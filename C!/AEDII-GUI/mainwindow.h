/**
  @file mainwindow.h
  @authors Mariana Navarro Jiménez, Jose Andres Arroyo, Jose Antonio Retana Corrales
  @date 4/23/2020
  @title Header class for Main Window GUI
  @brief Clase que permite visualizar y utilizar un GUI para manejar c!
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow, crea el mainWindow gui
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    /**
      @brief MainWindow, it's the desctructor class
        */
    ~MainWindow();

    void processBuffer();
    void setMainList(std::list<QStringList> newList);
    /**
     * @brief getMainList, method used to get a list of qstringlists
     * @return returns a list of QStringLists
     */
    std::list<QStringList> getMainList();
    int getScopeNum();
    void setScopeNum(int scopeNum);
    QStringList identifyStart(QString text);
    bool getTrueIf();
    void setTrueIf(bool flag);
    bool getStopFlag();
    void setStopFlag(bool flag);

private slots:
    /**
     * @brief on_pushButton_clicked, gets input from the user and turns it into a list, sets the Main list
     */
    void on_pushButton_clicked();
    /**
     * @brief on_nextButton_clicked, when clicked it reads line by line the code and send it to the server to be managed.
     */
    void on_nextButton_clicked();
    /**
     * @brief on_backButton_clicked, establishes connection with the server
     */
    void on_backButton_clicked();
    /**
     * @brief on_deleteButton_clicked, deletes the previous input made by the user
     */
    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;
    std::list<QStringList> mainList;
    bool trueIf = true;
    bool stopFlag = false;
    /**
     * @brief cout, shows in the terminal the instruction
     * @param newText; text introudced by the user
     */
    int scopeNum;
    void cout(string newText);
    /**
     * @brief ramView, shows the memoru, value, name and quatity of references in the GUI
     * @param memory
     * @param value
     * @param name
     * @param reference
     */
    void ramView(QString memory, QString value, QString name, QString reference);
    void readBuffer();
    void ifAndElse(QString text);
};
#endif // MAINWINDOW_H
