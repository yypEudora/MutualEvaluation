#include "student_mainwindow.h"
#include "ui_student_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>

student_mainwindow::student_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::student_mainwindow)
{
    ui->setupUi(this);

    // 处理所有信号
    managerSignals();

}

student_mainwindow::~student_mainwindow()
{
    delete ui;
}



void student_mainwindow::showMainWindow()
{
    this->show();
}


void student_mainwindow::managerSignals()
{
    // 切换用户
    connect(ui->change_user_btn, &QToolButton::clicked, [=]()
    {
        int choose;
        choose=QMessageBox::question(this, tr("退出登录"),
                                     QString(tr("确认退出登录?")),
                                     QMessageBox::Yes | QMessageBox::No);

      if (choose== QMessageBox::No)
       {
            //什么都不做
      }
      else if (choose== QMessageBox::Yes)
      {
        cout << "bye bye...";
        loginAgain(); //程序退出
      }

    });
}

void student_mainwindow::loginAgain()
{
    emit changeUser();
}
