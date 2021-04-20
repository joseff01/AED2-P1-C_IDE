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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setMainList(std::list<QStringList> newList);
    std::list<QStringList> getMainList();

private slots:
    void on_pushButton_clicked();
    void on_nextButton_clicked();
    void on_backButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;
    std::list<QStringList> mainList;
    void cout(string newText);
    void ramView(string memory, string value, string name, string reference);
};
#endif // MAINWINDOW_H
