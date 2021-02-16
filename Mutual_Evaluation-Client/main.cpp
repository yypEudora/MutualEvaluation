#include "login.h"
#include <QApplication>
#include <QDesktopWidget>
#include "student/stu_mainwindow.h"
#include "student/stu_info.h"
#include "teacher/tc_mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
//    Stu_Mainwindow* stu_mainwindow = new Stu_Mainwindow;
//    stu_mainwindow->show();

//    QDesktopWidget *desktop=QApplication::desktop();
//    stu_mainwindow->resize(desktop->width()*3/4,desktop->height()*3/4);
//    stu_mainwindow->move((desktop->width()-stu_mainwindow->width())/2,(desktop->height()-stu_mainwindow->height())/4);
//    stu_mainwindow->showMaximized();
//    stu_mainwindow->move ((QApplication::desktop()->width() - stu_mainwindow.width())/2,(QApplication::desktop()->height() - stu_mainwindow.height())/2);
//    Stu_Info *stu_info = new Stu_Info ();
//    stu_info->show();

//    Tc_Mainwindow *w =new Tc_Mainwindow ();
//    w->show();
    return a.exec();
}
