#include "mserver.h"

#include <QCoreApplication>
#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Mserver* mserver = new Mserver();
    mserver->serverSetup();
}
