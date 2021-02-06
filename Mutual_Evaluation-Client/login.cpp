#include "login.h"
#include "ui_login.h"

#include <QMouseEvent>
#include <QString>
#include <QToolButton>
Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    m_parent = parent;

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    setAutoFillBackground(true);

    //初始设置选择身份为学生
    ui->student_btn->setChecked(true);

    //密码
    ui->pwd_tx->setEchoMode(QLineEdit::Password);

    //信号槽管理
    manage_signals();
}

void Login::manage_signals()
{
    //登录界面点击设置，进入服务器IP和端口界面
    connect(ui->set_btn,&QToolButton::clicked,[=](){
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });
    //登录界面点击注册，进入注册界面
    connect(ui->register_btn,&QToolButton::clicked,[=](){
        ui->student_box->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->register_page);
        ui->idStackedWidget->setCurrentWidget(ui->student_page);
    });
    //登录界面点击退出，退出程序
    connect(ui->exit_btn,&QToolButton::clicked,[=](){
        exit(1);
    });
    //注册界面点击返回，返回登录界面
    connect(ui->ret_btn,&QToolButton::clicked,[=](){
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    });
    //注册界面点击设置，进入设置界面
    connect(ui->set3_btn,&QToolButton::clicked,[=](){
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });
    //设置界面点击返回，返回登录界面
    connect(ui->return_btn,&QToolButton::clicked,[=](){
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    });
    //注册界面点击学生，显示学生注册信息
    connect(ui->student_box,&QToolButton::clicked,[=](){
        ui->idStackedWidget->setCurrentWidget(ui->student_page);
    });
    //注册界面点击教师，显示教师注册信息
    connect(ui->teacher_box,&QToolButton::clicked,[=](){
        ui->idStackedWidget->setCurrentWidget(ui->teacher_page);
    });
    //注册界面点击助教，显示助教注册信息
    connect(ui->zhujiao_box,&QToolButton::clicked,[=](){
        ui->idStackedWidget->setCurrentWidget(ui->zhujiao_page);
    });
}

Login::~Login()
{
    delete ui;
}

void Login::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}

//确认登录
void Login::on_login_btn_clicked()
{

}
//确认注册
void Login::on_reg_btn_clicked()
{

}
//确认设置
void Login::on_set2_btn_clicked()
{

}
