#include "algorithmvisualisation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlgorithmVisualisation w;
    w.show();
    return a.exec();
}
