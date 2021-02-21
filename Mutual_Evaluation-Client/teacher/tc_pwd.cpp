#include "tc_pwd.h"
#include "ui_tc_pwd.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>

Tc_Pwd::Tc_Pwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tc_Pwd)
{
    ui->setupUi(this);

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    manage_signals();
}

Tc_Pwd::~Tc_Pwd()
{
    delete ui;
}

void Tc_Pwd::manage_signals()
{
    //缩小按钮
    connect(ui->hide_btn,&QToolButton::clicked,[=](){
        if( windowState() != Qt::WindowMinimized ){
            setWindowState( Qt::WindowMinimized );
        }
    });
    //关闭按钮
    connect(ui->close_btn,&QToolButton::clicked,[=](){
        this->close();
    });

}

void Tc_Pwd::set_ui()
{
//    //缩小按钮
//    QIcon icon;
//    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton);
//    ui->hide_btn->setIcon(icon);
//    //关闭按钮
//    icon = style()->standardIcon(QStyle::SP_TitleBarCloseButton);
//    ui->close_btn->setIcon(icon);
}

void Tc_Pwd::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Tc_Pwd::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Tc_Pwd::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}
