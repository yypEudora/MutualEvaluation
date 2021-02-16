#include "Stu_Mainwindow.h"
#include "ui_stu_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>
#include <QAction>
#include <QMenu>
stu_mainwindow::stu_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stu_mainwindow)
{
    ui->setupUi(this);

    // 处理所有信号
    manager_signals();

    m_stu_info = new Stu_Info;
}



stu_mainwindow::~stu_mainwindow()
{
    delete ui;
}



void stu_mainwindow::show_mainwindow()
{
    this->show();
}


void stu_mainwindow::manager_signals()
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

    //个人信息
    connect(ui->stu_info_list_btn, &QToolButton::clicked, [=](){
        this->hide();
        m_stu_info->show();
    });

}

void stu_mainwindow::login_again()
{
    this->hide();
    emit change_user();

}

void stu_mainwindow::ui_set()
{
    //设置工具栏的下拉列表
    m_stu_info_menu = new QMenu();
    m_change_user_menu = new QMenu();
    m_stu_exit_menu = new QMenu();
    QAction *stu_info = new QAction (m_stu_info_menu);
    QAction *stu_change_user = new QAction (m_change_user_menu);
    QAction *stu_exit = new QAction (m_stu_exit_menu);

    stu_info->setText("个人信息");
    stu_change_user->setText("切换用户");
    stu_exit->setText("退出登录");

    m_stu_info_menu->addAction(stu_info);
    m_change_user_menu->addAction(stu_change_user);
    m_stu_exit_menu->addAction(stu_exit);

    ui->stu_info_list_btn->setMenu(m_stu_info_menu);
    ui->change_user_btn->setMenu(m_change_user_menu);
    ui->stu_exit_btn->setMenu(m_stu_exit_menu);

}
