#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setMainList(QStringList newList);
    QStringList getMainList();


private slots:
    QStringList mainList;
    int positionInt;

    void on_pushButton_clicked();

    void on_nextButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
