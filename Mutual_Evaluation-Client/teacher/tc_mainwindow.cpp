#include "tc_mainwindow.h"
#include "ui_tc_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
Tc_Mainwindow::Tc_Mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tc_Mainwindow)
{
    ui->setupUi(this);


    // 处理所有信号
    manager_signals();
    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    m_tc_info = new Tc_Info;

    ui_set();
}

Tc_Mainwindow::~Tc_Mainwindow()
{
    delete ui;
}

void Tc_Mainwindow::show_mainwindow()
{
    this->show();
}


void Tc_Mainwindow::manager_signals()
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
    connect(ui->tc_info_list_btn, &QToolButton::clicked, [=](){
        this->hide();
        m_tc_info->show();
    });

    //隐藏按钮
    connect(ui->hide_btn,&QToolButton::clicked,[=](){
        if( windowState() != Qt::WindowMinimized ){
            setWindowState( Qt::WindowMinimized );
        }
    });
    //关闭按钮
    connect(ui->close_btn,&QToolButton::clicked,[=](){
        this->close();
    });

    //缩小放大按钮
    connect(ui->min_max_btn,&QToolButton::clicked,[=](){

        QIcon icon;
        if( m_max_or_normal ){
            icon = style()->standardIcon( QStyle::SP_TitleBarNormalButton );
            ui->min_max_btn->setIcon( icon );
            setWindowState( Qt::WindowMaximized );
        }else {
            icon = style()->standardIcon( QStyle::SP_TitleBarMaxButton );
            ui->min_max_btn->setIcon( icon );
            setWindowState( Qt::WindowNoState );
        }
        m_max_or_normal = !m_max_or_normal;
    });


}

void Tc_Mainwindow::ui_set()
{
    //设置工具栏的下拉列表
    m_tc_info_menu = new QMenu();
    m_change_user_menu = new QMenu();
    m_tc_exit_menu = new QMenu();
    QAction *stu_info = new QAction (m_tc_info_menu);
    QAction *stu_change_user = new QAction (m_change_user_menu);
    QAction *stu_exit = new QAction (m_tc_exit_menu);

    stu_info->setText(QObject::tr("个人信息"));
    stu_change_user->setText(QObject::tr("切换用户"));
    stu_exit->setText(QObject::tr("退出登录"));

    m_tc_info_menu->addAction(stu_info);
    m_change_user_menu->addAction(stu_change_user);
    m_tc_exit_menu->addAction(stu_exit);

    ui->tc_info_list_btn->setMenu(m_tc_info_menu);
    ui->change_user_btn->setMenu(m_change_user_menu);
    ui->tc_exit_btn->setMenu(m_tc_exit_menu);

//    QIcon icon;
//    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton );
//    // true represents Max
//    if( !m_max_or_normal ){
//        icon = style()->standardIcon( QStyle::SP_TitleBarMaxButton );
//        ui->min_max_btn->setIcon( icon );
//    }else{
//        icon = style()->standardIcon( QStyle::SP_TitleBarNormalButton );
//        ui->min_max_btn->setIcon( icon );
//    }
}

void Tc_Mainwindow::login_again()
{
    this->hide();
    emit change_user();
}

void Tc_Mainwindow::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Tc_Mainwindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Tc_Mainwindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}

