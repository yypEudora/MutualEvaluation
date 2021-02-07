#include "stu_mainwindow.h"
#include "ui_stu_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>

stu_mainwindow::stu_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stu_mainwindow)
{
    ui->setupUi(this);

    // 处理所有信号
    managerSignals();
}

stu_mainwindow::~stu_mainwindow()
{
    delete ui;
}



void stu_mainwindow::showMainWindow()
{
    this->show();
}


void stu_mainwindow::managerSignals()
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

void stu_mainwindow::loginAgain()
{
    this->hide();
    emit changeUser();

}
