#include "taskmgr.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    taskmgr w;
    w.setWindowTitle("Курсовая работа");
    w.show();

    return a.exec();
}
