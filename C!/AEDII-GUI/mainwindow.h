/**
  * @file mainwindow.h
  * @authors Mariana Navarro Jiménez, Jose Andres Arroyo, Jose Antonio Retana Corrales
  * @date 4/23/2020
  * @title Clase header para la clase Main Window GUI.
  * @brief Plantilla de clase que permite visualizar y utilizar un GUI para manejar c!.
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
    /**
     * @brief setMainList cambia la informacion almacenada en mainList por la nueva lista de QstringLists ingresada
     * @param newList lista de QstringLists
     */
    void setMainList(std::list<QStringList> newList);
    /**
     * @brief getMainList metodo que regresa el valor de la mainList actual
     * @return retorna Mainlist actual
     */
    std::list<QStringList> getMainList();
    /**
     * @brief getScopeNum metodo que obtiene el valor del numero de scopes actual
     * @return retorna el numero de scopes actual
     */
    int getScopeNum();
    /**
     * @brief setScopeNum metodo que cambia el scopeNum por el dato inresado al llamar la funcion
     * @param scopeNum nuevo numero de scope
     */
    void setScopeNum(int scopeNum);
    /**
     * @brief getWhileFlag metodo que analiza si se esta analizado un while en la linea de codigo
     * @return retorna un booleando que indica si se analiza un while o no en la linea de codigo
     */
    bool getWhileFlag();
    /**
     * @brief setWhileFlag metodo que cambia el booleando del Whileflag
     * @param whileFlag booleano que se ingresa al llamar la funcion
     */
    void setWhileFlag(bool whileFlag);
    /**
     * @brief identifyStart metodo que identifica si hay un if o while al inicio de la linea de codigo se esta analizando
     * @param text Qstring de la linea de codigo que se esta analizando
     * @return retorna un QStringList con la linea de codigo analizada
     */
    QStringList identifyStart(QString text);
    /**
     * @brief getTrueIf metodo que obtiene el valor del booleano trueIf
     * @return retorna el booleano de true if
     */
    bool getTrueIf();
    /**
     * @brief setTrueIf Metodo que cambia el valor del booleano trueIf
     * @param flag valor booleano ingresado al llamar la funcion
     */
    void setTrueIf(bool flag);
    /**
     * @brief getStructName Metodo que obtiene el QstringList de los nombres dentro del struct
     * @return retorna un QStringist de los nombres de las vriabels del struct creado
     */
    QStringList getStructName();
    /**
     * @brief setStructName Cambia los nombres del QStringList StructName por la nueva Qstringlist ingresada
     * @param list nueva QstringList con los nombres de las variables
     */
    void setStructName(QStringList list);
    /**
     * @brief getWhileVector vetodo que obtiene el valor de whileVector
     * @return retorna un vector de vectores de QstringList
     */
    std::vector<std::vector<QStringList>> getWhileVector();
    /**
     * @brief setWhileVector cambia el valor del whileVector
     * @param vector nuevo vector ingresado al llamar la funcion
     */
    void setWhileVector(vector<std::vector<QStringList>> vector);

private slots:

    /**
     * @brief on_pushButton_clicked Metodo que fragmenta y procesa el imput del usuario.
     */
    void on_pushButton_clicked();

    /**
     * @brief on_nextButton_clicked Metodo que envia linea una linea del imput del usuario a Json.
     */
    void on_nextButton_clicked();

    /**
     * @brief on_backButton_clicked Metodo que establece coneccion entre el servidor y el usuario.
     */
    void on_backButton_clicked();

    /**
     * @brief on_deleteButton_clicked Metodo que elimina los elementos del Ram Live View del GUI.
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
     * @return un string con el caso retornado por el servidor
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
