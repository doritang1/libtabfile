#include <QCoreApplication>
#include "libtabfile.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    convert_tabfile("inputfile");

    return a.exec();
}

