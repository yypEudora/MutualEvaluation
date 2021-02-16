#include "tc_info.h"
#include "ui_tc_info.h"
#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>
Tc_Info::Tc_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tc_Info)
{
    ui->setupUi(this);
    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    manage_signals();

}

Tc_Info::~Tc_Info()
{
    delete ui;
}

void Tc_Info::manage_signals()
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
    //返回按钮
    connect(ui->return_btn,&QToolButton::clicked,[=](){
       this->close();
    });
}

void Tc_Info::set_ui()
{
    //缩小按钮
    QIcon icon;
    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton);
    ui->hide_btn->setIcon(icon);
    //关闭按钮
    icon = style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    ui->close_btn->setIcon(icon);
}

void Tc_Info::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Tc_Info::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Tc_Info::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}

