#include <QCoreApplication>
#include "gattsinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GATTSInterface gattServer;

    return a.exec();
}
