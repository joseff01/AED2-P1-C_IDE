/**
  * @file mserver.h
  * @authors Mariana Navarro Jiménez, Jose Andres Arroyo, Jose Antonio Retana Corrales
  * @date 4/23/2020
  * @brief plantilla de clase mserver para el manejo de los request de memoria del objeto MainWindow
  * Mediante el envio de mensajes de tipo Json desde el objeto MainWindow, Mserver realiza los requests enviados por este. Estos requests pueden ser : guardado de variables, terminacion de scopes, declaracion de structs, entre otros.
  */
#ifndef MSERVER_H
#define MSERVER_H

#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <json.hpp>
#include <map>
#include <sstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

class Mserver
{
    public:
    /**
     * @brief Mserver constructor de la clase Mserver
     */
    Mserver();
    /**
     * @brief error metodo que para el funcionamiento de Mserver y muestra en consola el error generado
     * @param msg mesaje de error que se mostrara antes de finalizar el programa
     */
    void error(const char *msg);
    /**
     * @brief bindingProcess realiza el proceso de binding del servidor a un numero de socket, iniciando en 5000
     * @param sockfd file descriptor que guarda los valores retornados por el socket system call y el accept system call.
     * @param portno guarda el numero de puerto con el que se aceptan las conecciones
     * @param serv_addr es el struct que contiene el adress del servidor
     */
    void bindingProcess(int* sockfd, int* portno, struct sockaddr_in* serv_addr);
    /**
     * @brief resetMemory reinicia la memoria a su estado incial y libera toda la memoria que de las variables generadas anteriormente por el MainWindow
     */
    void resetMemory();
    /**
     * @brief getVariableValue retorna el valor guardado en la memoria segun el nombre de la variable dada
     * @param variableName nombre de la variable que se quiere buscar en la memoria para retornar su valor guardado
     * @return retorna un string del valor recuperado de la memoria de la variable dada
     */
    string getVariableValue(string variableName);
    /**
     * @brief checkNumericalValueOfExpression metodo que, dada una expresion, devuelve el valor numerico de esta.
     * @param stringExpression expresion a la cual se quiere saber el valor numerico. Esta puede contener el valor de variabels guardadas en la memoria
     * @return retorna un double del valor numerico de la expresion dada. Da error si se incluye el nombre de una variable no guardada en la memoria
     */
    double checkNumericalValueOfExpression(string stringExpression);
    /**
     * @brief checkBooleanValueOfExpression encuentra el valor booleano de una expresion dada
     * @param stringExpression expresion a la cual se quiere saber el valor booleano de esta
     * @return retorna un booleano que corresponde al valor de la expresion dada
     */
    bool checkBooleanValueOfExpression(string stringExpression);
    /**
     * @brief removeScopeMemory metodo llamado al terminar un scope que contenia un struct para que todos los elementos que conforman el struct sean eliminados de la memoria correctamente
     * @return retorna un vector de la lista de variables guardads en el struct que fueron eliminadas de la memoria
     */
    vector<string> removeScopeMemory();
    /**
     * @brief analizeBuffer funcion principal de ejecucion de la clase mserver. Esta se encarga de analizar los Jsons enviados por la clase MainWindow, para que estos sean procesados y que se lleve a cabo el request dado por el cliente
     */
    void analizeBuffer();
    /**
     * @brief readBuffer metodo que analiza el mensaje obtenido del cliente
     */
    void readBuffer();
    /**
     * @brief serverSetup reliza el setup inicial del server para que este pueda conectar con el cliente. Tambien se encarga de empezar el bucle de analisis del request del cliente.
     */
    void serverSetup();


};

#endif // MSERVER_H
