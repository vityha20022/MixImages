#include "miximages.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MixImages w;
    w.show();

    return a.exec();
}
