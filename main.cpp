#include "CuttingProblem.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CuttingProblem w;
    w.showMaximized();
    return a.exec();
}
