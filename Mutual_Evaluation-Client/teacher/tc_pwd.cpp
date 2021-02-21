#include "tc_pwd.h"
#include "ui_tc_pwd.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>
#include <QMessageBox>
#include "common/common.h"

Tc_Pwd::Tc_Pwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tc_Pwd)
{
    ui->setupUi(this);

    // 密码样式
    ui->tc_before_pwd_tx->setEchoMode(QLineEdit::Password);
    ui->tc_new_pwd_tx->setEchoMode(QLineEdit::Password);
    ui->tc_pwd_again_tx->setEchoMode(QLineEdit::Password);

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    manage_signals();
}

Tc_Pwd::~Tc_Pwd()
{
    delete ui;
}


/**
 * @brief Tc_Pwd::manage_signals 管理信号槽连接
 */
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

    //保存按钮
    connect(ui->save_btn, &QToolButton::clicked,[=](){
        //检查密码是否合法
        if(check_pwd_valid()){      //密码合法
            int choose;
            choose=QMessageBox::warning(this, tr("提示"),
                                         QString(tr("是否确认修改？")),
                                         QMessageBox::Yes | QMessageBox::No);
            if (choose== QMessageBox::Yes)
            {
                m_password = ui->tc_new_pwd_tx->text();
                emit save_updated_pwd_to_server();
                ui->tc_before_pwd_tx->clear();
                ui->tc_new_pwd_tx->clear();
                ui->tc_pwd_again_tx->clear();
                QMessageBox::information(this, tr("提示"),
                                    QString(tr("修改成功！")),
                                    QMessageBox::Yes);
             }
        }

    });

    //返回按钮
    connect(ui->return_btn,&QToolButton::clicked,[=](){
        int choose;
        choose=QMessageBox::warning(this, tr("提示"),
                                     QString(tr("是否确认离开？")),
                                     QMessageBox::Yes | QMessageBox::No);
        if (choose== QMessageBox::Yes)
        {
            emit back_to_mainwindow();
            ui->tc_before_pwd_tx->clear();
            ui->tc_new_pwd_tx->clear();
            ui->tc_pwd_again_tx->clear();
        }
    });


}


/**
 * @brief Tc_Pwd::show_mainwindow 显示学生修改密码主界面
 */
void Tc_Pwd::show_mainwindow(QString pwd)
{
    this->show();

    m_password = pwd;
}


/**
 * @brief Tc_Pwd::check_pwd_valid 检查输入密码的合法性
 * @return 合法返回true,否则返回false
 */
bool Tc_Pwd::check_pwd_valid()
{
    QString before_pwd = ui->tc_before_pwd_tx->text();
    QString new_pwd = ui->tc_new_pwd_tx->text();
    QString pwd_again = ui->tc_pwd_again_tx->text();



    if(ui->tc_before_pwd->text()=="")  //原始密码为空
    {
        QMessageBox::warning(this, "警告", "原始密码不能为空！");
        ui->tc_before_pwd_tx->setFocus();
        return false;
    }
    else if(!check_before_pwd_correct()) //原始密码验证错误
    {
        QMessageBox::warning(this, "警告", "原始密码错误！请重新输入");
        ui->tc_before_pwd_tx->clear();
        ui->tc_before_pwd_tx->setFocus();
        return false;
    }

    // 数据校验
    QRegExp pwdexp(PASSWD_REG);
    if(ui->tc_new_pwd_tx->text()=="")  //新密码为空
    {
        QMessageBox::warning(this, "警告", "新密码不能为空！");
        ui->tc_new_pwd_tx->setFocus();
        return false;
    }
    else if(!pwdexp.exactMatch(new_pwd)) //新密码格式错误
    {
        QMessageBox::warning(this, "警告", "新密码格式错误！");
        ui->tc_new_pwd_tx->clear();
        ui->tc_new_pwd_tx->setFocus();
        return false;
    }
    else if(new_pwd == before_pwd){
        QMessageBox::warning(this, "警告", "新密码不能与原始密码相同！");
        ui->tc_new_pwd_tx->clear();
        ui->tc_pwd_again_tx->clear();
        ui->tc_new_pwd_tx->setFocus();

        return false;
    }

    if(ui->tc_pwd_again_tx->text()=="") //确认密码为空
    {
        QMessageBox::warning(this, "警告", "请确认新密码！");
        ui->tc_pwd_again_tx->setFocus();
        return false;
    }
    else if(pwd_again != new_pwd){
        QMessageBox::warning(this, "警告", "确认密码错误！请重新输入");
        ui->tc_pwd_again_tx->clear();
        ui->tc_pwd_again_tx->setFocus();
        return false;
    }
    return true;
}


/**
 * @brief Tc_Pwd::check_before_pwd_correct 验证旧密码是否正确
 * @return 合法返回true,否则返回false
 */
bool Tc_Pwd::check_before_pwd_correct()
{
    QString pwd = ui->tc_before_pwd->text();
    emit check_before_pwd();
    if(pwd_str_md5 !=m_password)
        return false;
    return true;
}


/**
 * @brief Tc_Pwd::updated_pwd_return 给学生主界面调用，查询修改过后的密码
 */
void Tc_Pwd::updated_pwd_return(QString &pwd)
{
    m_password = new_pwd_str_md5;
    pwd = new_pwd_str_md5;
    qDebug()<<"pwd_str_md5"<<pwd;
}


/**
 * @brief Tc_Pwd::return_input_pwd 给login主界面调用，查询输入的原始密码
 */
void Tc_Pwd::return_input_pwd(QString &pwd)
{
    pwd = this->ui->tc_before_pwd_tx->text();
}


/**
 * @brief Tc_Pwd::return_input_new_pwd 给login主界面调用，查询输入的新密码
 */
void Tc_Pwd::return_input_new_pwd(QString &new_pwd)
{
    new_pwd = this->ui->tc_new_pwd_tx->text();
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
