/**
  * @file mainwindow.h
  * @authors Mariana Navarro Jiménez, Jose Andres Arroyo, Jose Antonio Retana Corrales
  * @date 4/23/2020
  * @title Header class for Main Window GUI
  * @brief Clase que permite es la plantilla de la clase que permite visualizar y utilizar un GUI para manejar c!
  * Mediante esta plantilla de la clase MainWindow el usuario puede incertar codigo mediante el GUI que maneja el codigo. El lenguaje que puede leer es c!.
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

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setMainList(std::list<QStringList> newList);
    std::list<QStringList> getMainList();
    int getScopeNum();
    void setScopeNum(int scopeNum);
    bool getWhileFlag();
    void setWhileFlag(bool whileFlag);
    QStringList identifyStart(QString text);
    bool getTrueIf();
    void setTrueIf(bool flag);
    QStringList getStructName();
    void setStructName(QStringList list);
    std::vector<std::vector<QStringList>> getWhileVector();
    void setWhileVector(vector<std::vector<QStringList>> vector);

private slots:

    /**
     * @brief on_pushButton_clicked Metodo que fragmenta y procesa el imput del usuario
     */
    void on_pushButton_clicked();

    /**
     * @brief on_nextButton_clicked Metodo que envia linea una linea del imput del usuario a Json
     */
    void on_nextButton_clicked();

    /**
     * @brief on_backButton_clicked Metodo que establece coneccion entre el servidor y el usuario
     */
    void on_backButton_clicked();

    /**
     * @brief on_deleteButton_clicked Metodo que elimina los elementos del Ram Live View del GUI
     */
    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;
    std::list<QStringList> mainList;/*!< Lista de QStringList que almacena como QString los elementos del input del usuario. */
    QStringList structNames; /*!< QStringList que almacena los nombres creados por el metodo de struct*/
    int scopeNum; /*!< Int que indica la cantidad de scopes presentes en el input del usuario */
    bool whileFlag; /*!< Boolean que indica si en la linea del codigo sucede un while */
    bool trueIf = true; /*!< Boolean que indica si se cumple la condicional del if */
    std::vector<std::vector<QStringList>> whileVector;/*!< vector de vecotres con QStringLists, este vector almacena las funciones del while */

    /**
     * @brief cout Metodo que muestra en la terminal del GUI el parametro pedido.
     * @param newText elemento que se va a mostra en la terminal
     */
    void cout(string newText);

    /**
     * @brief ramView Metodo que muestra en el Ram Live View del GUI los parametros "Nombre", "Valor","Numero Referenciado" y "Espacio de memoria"
     * @param memory direccion de memoria de la variable
     * @param value valor de la variable
     * @param name nombre de la variable
     * @param reference numero de veces que se referencio la vriable
     */
    void ramView(QString memory, QString value, QString name, QString reference);

    /**
     * @brief readBuffer Metodo que hace el parcing de los Jsons enviados por el servidor.
     */
    void readBuffer();

    /**
     * @brief ifAndElse Metodo que le envia al servidor un Json con los parametros de la condicional del if o while
     * @param text parametro condicional del if o while
     * @param isWhile booleano que indica si se realiza un while o if
     * @return
     */
    string ifAndElse(QString text,bool isWhile);

    /**
     * @brief structJson Metodo que le envia los parametros del struct como Json al server
     * @param structList Elementos del struct
     * @param structName Nombre de elementos del struct
     */
    void structJson(std::list<QStringList> structList, string structName);

    /**
     * @brief alignText Metodo que alinea los elementos en el Live Ram View
     */
    void alignText();
};
#endif // MAINWINDOW_H
