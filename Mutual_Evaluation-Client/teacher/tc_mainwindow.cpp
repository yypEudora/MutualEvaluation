#include "tc_mainwindow.h"
#include "ui_tc_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>

tc_mainwindow::tc_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tc_mainwindow)
{
    ui->setupUi(this);


    // 处理所有信号
    manager_signals();
}

tc_mainwindow::~tc_mainwindow()
{
    delete ui;
}

void tc_mainwindow::show_mainwindow()
{
    this->show();
}


void tc_mainwindow::manager_signals()
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
        login_again(); //程序退出
      }

    });




}

void tc_mainwindow::login_again()
{
    this->hide();
    emit change_user();
}
