#include <QApplication>
//#include "mydialogwindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *wnd = new MainWindow();

    wnd->show();

    return a.exec();
}
