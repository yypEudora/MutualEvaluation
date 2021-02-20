#include "stu_info.h"
#include "ui_stu_info.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>
#include <common/common.h>
#include <QMessageBox>

Stu_Info::Stu_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stu_Info)
{
    ui->setupUi(this);

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    manage_signals();

}

Stu_Info::~Stu_Info()
{
    delete ui;
}

void Stu_Info::show_mainwindow(QString pwd, QString name, QString sex, QString academy, QString grade,
                               QString major, QString clas,QString tell, QString qq)
{
    this->show();
    ui->stu_name_tx->setText(name);
    ui->stu_sex_tx->setText(sex);
    ui->stu_academy_tx->setText(academy);
    ui->stu_grade_tx->setText(grade);
    ui->stu_class_tx->setText(clas);
    //设置专业 ui->stu_major_tx->setText(major);
    ui->stu_tell_tx->setText(tell);
    ui->stu_qq_tx->setText(qq);


    m_password = pwd;
    m_name = name;
    m_sex = sex;
    m_academy = academy;
    m_grade = grade;
    m_major = major;
    m_class = clas;
    m_tell = tell;
    m_qq = qq;
}

void Stu_Info::manage_signals()
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

    //保存按钮
    connect(ui->save_btn, &QToolButton::clicked,[=](){
        //...判断个人信息的输入是否合法

        //判断个人信息是否变化
        if(!check_info_updated()){  //没有变化
            QMessageBox::information(this, tr("提示"),
                                     QString(tr("并没有任何修改！")),
                                     QMessageBox::Yes);
        }
        else{
            int choose;
            choose=QMessageBox::warning(this, tr("提示"),
                                         QString(tr("是否保存修改？")),
                                         QMessageBox::Yes | QMessageBox::No);
            if (choose== QMessageBox::Yes)
            {
              save_changed_info();
              QMessageBox::information(this, tr("提示"),
                                    QString(tr("保存成功！")),
                                    QMessageBox::Yes);
             }
        }
    });

    //返回按钮
    connect(ui->return_btn,&QToolButton::clicked,[=](){
        if(check_info_updated()){
            int choose;
            choose=QMessageBox::warning(this, tr("提示"),
                                         QString(tr("个人信息已修改，是否保存？")),
                                         QMessageBox::Yes | QMessageBox::No);
            if (choose== QMessageBox::Yes)  //保存修改
            {
                save_changed_info();
                QMessageBox::information(this, tr("提示"),
                                        QString(tr("保存成功！")),
                                        QMessageBox::Yes);
             }
            else                            //不保存修改，直接返回
                back_to_stu_mainwindow();

        }
        else
            back_to_stu_mainwindow();
    });
}

void Stu_Info::back_to_stu_mainwindow()
{
    this->hide();
    emit back_to_mainwindow(); //向主界面发送返回主界面的信号
}

void Stu_Info::set_ui()
{
    //缩小按钮
    QIcon icon;
    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton);
    ui->hide_btn->setIcon(icon);
    //关闭按钮
    icon = style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    ui->close_btn->setIcon(icon);
}

bool Stu_Info::check_info_updated()
{

//    if(ui->stu_name_tx->isModified() || ui->stu_sex_tx->isModified() || ui->stu_academy_tx->isModified() || ui->stu_grade_tx->isModified()
//        || ui->stu_class_tx->isModified() || ui->stu_tell_tx->isModified() || ui->stu_qq_tx->isModified())
//        return true;
//    return false;

    //少了|| ui->stu_major_tx->isModified()
    if(m_name != ui->stu_name_tx->text() || m_sex != ui->stu_sex_tx->text() || m_academy != ui->stu_academy_tx->text()
            || m_grade != ui->stu_grade_tx->text() || m_class != ui->stu_class_tx->text() || m_tell != ui->stu_tell_tx->text()
            || m_qq != ui->stu_qq_tx->text())
        return true;

    return false;
}

void Stu_Info::save_changed_info()
{
    //没有major
    m_name = this->ui->stu_name_tx->text();
    m_sex = this->ui->stu_sex_tx->text();
    m_academy = this->ui->stu_academy_tx->text();
    m_grade = this->ui->stu_grade_tx->text();
    m_class = this->ui->stu_class_tx->text();
    m_tell = this->ui->stu_tell_tx->text();
    m_qq = this->ui->stu_qq_tx->text();
    emit save_to_server();
}

void Stu_Info::updated_info_return(QString &name, QString &sex, QString &academy, QString &grade,
                         QString &major, QString &clas,QString &tell, QString &qq)
{
    name = m_name;
    sex = m_sex;
    academy = m_academy;
    grade = m_grade;
    major = m_major;
    clas = m_class;
    tell = m_tell;
    qq = m_qq;
}

void Stu_Info::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Stu_Info::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Stu_Info::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}
