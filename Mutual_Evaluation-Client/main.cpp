#include "login.h"
#include <QApplication>
#include "student/stu_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    //stu_mainwindow w;
    w.show();
    return a.exec();
}
