#include "MyApp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyApp w;
    w.showFullScreen();
    return a.exec();
}
