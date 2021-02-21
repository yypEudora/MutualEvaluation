#include "tc_info.h"
#include "ui_tc_info.h"
#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>
#include <common/common.h>
#include <QMessageBox>

Tc_Info::Tc_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tc_Info)
{
    ui->setupUi(this);
    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    // 数据的格式提示
    ui->tc_name_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 1~16");
    ui->tc_email_tx->setToolTip("合法字符:[0-9],字符个数: 13");
    ui->tc_tell_tx->setToolTip("合法字符:[0-9],字符个数: 13");
    ui->tc_qq_tx->setToolTip("合法字符:[0-9],字符个数: 7~15");

    manage_signals();

}

Tc_Info::~Tc_Info()
{
    delete ui;
}


/**
 * @brief Tc_Info::show_mainwindow 显示学生修改个人信息界面
 * @param pwd, name, sex...  当前用户的用户数据
 */
void Tc_Info::show_mainwindow(QString pwd, QString name, QString sex, QString academy, QString email,
                              QString tell, QString qq, bool completed_info)
{
    this->show();
    ui->tc_name_tx->setText(name);
    ui->tc_sex_tx->setText(sex);
    ui->tc_academy_tx->setText(academy);
    ui->tc_email_tx->setText(email);
    ui->tc_tell_tx->setText(tell);
    ui->tc_qq_tx->setText(qq);


    m_password = pwd;
    m_name = name;
    m_sex = sex;
    m_academy = academy;
    m_email = email;
    m_tell = tell;
    m_qq = qq;
    m_completed_info = completed_info;

    //如果已完善个人信息则隐藏注意标语
    if(completed_info)
       this->ui->tc_warning_label->hide();
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

    //保存按钮
    connect(ui->save_btn, &QToolButton::clicked,[=](){
        //判断个人信息的输入格式是否合法
        if(check_info_valid()){      //格式合法
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
                //判断个人信息的输入是否合法
                if(check_info_valid()){
                    save_changed_info();
                    QMessageBox::information(this, tr("提示"),
                                            QString(tr("保存成功！")),
                                            QMessageBox::Yes);
                }
             }
            else                            //不保存修改，直接返回
                back_to_tc_mainwindow();

        }
        else
            back_to_tc_mainwindow();
    });
}


/**
 * @brief Tc_Info::back_to_stu_mainwindow 返回到学生主界面
 */
void Tc_Info::back_to_tc_mainwindow()
{
    this->hide();
    emit back_to_mainwindow(); //向主界面发送返回主界面的信号
}


void Tc_Info::set_ui()
{
//    //缩小按钮
//    QIcon icon;
//    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton);
//    ui->hide_btn->setIcon(icon);
//    //关闭按钮
//    icon = style()->standardIcon(QStyle::SP_TitleBarCloseButton);
//    ui->close_btn->setIcon(icon);
}



/**
 * @brief Tc_Info::check_info_updated 检查个人信息是否被修改
 */
bool Tc_Info::check_info_updated()
{

//    if(ui->stu_name_tx->isModified() || ui->stu_sex_tx->isModified() || ui->stu_academy_tx->isModified() || ui->stu_grade_tx->isModified()
//        || ui->stu_class_tx->isModified() || ui->stu_tell_tx->isModified() || ui->stu_qq_tx->isModified())
//        return true;
//    return false; 这种方法不方便在返回的时候检查个人信息是否被修改

    if(m_name != ui->tc_name_tx->text() || m_sex != ui->tc_sex_tx->text() || m_academy != ui->tc_academy_tx->text()
            || m_email != ui->tc_email_tx->text() || m_tell != ui->tc_tell_tx->text() || m_qq != ui->tc_qq_tx->text())
        return true;

    return false;
}


/**
 * @brief Tc_Info::check_info_valid 检查个人信息的合法性
 */
//数据校验有bug，后面完善
bool Tc_Info::check_info_valid()
{
    QString name = m_name;
    QString tell = m_tell;
    QString qq = m_qq;

    if(ui->tc_name_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "用户名不能为空");
        ui->tc_name_tx->setFocus();
        return false;
    }
    if(ui->tc_email_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "邮箱不能为空");
        ui->tc_email_tx->setFocus();
        return false;
    }
    if(ui->tc_tell_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "电话不能为空");
        ui->tc_tell_tx->setFocus();
        return false;
    }
    if(ui->tc_qq_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "qq不能为空");
        ui->tc_qq_tx->setFocus();
        return false;
    }
    return true;


    // 数据校验
//    QRegExp infoexp(Name);
//    if(!regexp.exactMatch(name))
//    {
//        QMessageBox::warning(this, "警告", "用户名格式不正确");
//        ui->stu_name_tx->clear();
//        ui->stu_name_tx->setFocus();
//        return false;
//    }
//    infoexp.setPattern(Tell);
//    if(!regexp.exactMatch(tell))
//    {
//        QMessageBox::warning(this, "警告", "电话格式不正确");
//        ui->stu_tell_tx->clear();
//        ui->stu_tell_tx->setFocus();
//        return false;
//    }
//    infoexp.setPattern(QQ);
//    if(!regexp.exactMatch(qq))
//    {
//        QMessageBox::warning(this, "警告", "qq格式不正确");
//        ui->stu_qq_tx->clear();
//        ui->stu_qq_tx->setFocus();
//        return false;
//    }
//    return true;
}


/**
 * @brief Tc_Info::save_changed_info 改变类成员变量以及发送给主界面将新的个人信息保存到服务器的信号
 */
void Tc_Info::save_changed_info()
{
    m_name = this->ui->tc_name_tx->text();
    m_sex = this->ui->tc_sex_tx->text();
    m_academy = this->ui->tc_academy_tx->text();
    m_email = this->ui->tc_email_tx->text();
    m_tell = this->ui->tc_tell_tx->text();
    m_qq = this->ui->tc_qq_tx->text();
    m_completed_info = true;
    emit save_updated_info_to_server();
}


/**
 * @brief Tc_Info::updated_info_return 给主界面调用，查询修改过后的个人信息
 * @param name, sex, academy...  当前用户可修改的个人信息
 */
void Tc_Info::updated_info_return(QString &name, QString &sex, QString &academy, QString &email,
                                  QString &tell, QString &qq, bool &completed_info)
{
    name = m_name;
    sex = m_sex;
    academy = m_academy;
    email = m_email;
    tell = m_tell;
    qq = m_qq;
    completed_info = m_completed_info;
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

