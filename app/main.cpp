// QtStaticExample.cpp : Defines the entry point for the application.
//

#include "MainWindow.h"
#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
