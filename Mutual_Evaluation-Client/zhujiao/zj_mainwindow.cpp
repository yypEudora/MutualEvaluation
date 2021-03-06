#include "zj_mainwindow.h"
#include "ui_zj_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>

Zj_mainwindow::Zj_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Zj_mainwindow)
{
    ui->setupUi(this);


    // 处理所有信号
    manager_signals();
}

Zj_mainwindow::~Zj_mainwindow()
{
    delete ui;
}

void Zj_mainwindow::show_mainwindow()
{
    this->show();
}


void Zj_mainwindow::manager_signals()
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

void Zj_mainwindow::login_again()
{
    this->hide();
    emit change_user();
}
