#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include "common/common.h"
#include "common/des.h"
#include "student/stu_mainwindow.h"
#include "teacher/tc_mainwindow.h"
#include "zhujiao/zj_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE


class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    void get_ip_port();
    void manage_format(); //管理数据格式
    void manage_signals(); //管理信号槽连接
    ~Login();

    // 设置登陆用户信息的json包
    QByteArray setLoginJson(QString current_user, QString user, QString pwd);

    // 设置注册用户信息的json包
    QByteArray setRegisterJson(QString current_user, QString user,QString pwd);

    //得到服务器处理登录后的返回码
    QStringList getLoginStatus(QByteArray json);

    void GetBackJson(QByteArray &back_buf, QString &sender,QString &msg); //解析返回的json数据包

    QString getLoginCurrentUser();  //得到当前请求登录的用户类别
    QString getRegistCurrentUser(); //得到当前请求注册的用户类别


protected: 
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    // 读取配置信息，设置默认登录状态，默认设置信息
    //void readCfg();


private slots:
    void on_login_btn_clicked();
    void on_reg_btn_clicked();
    void on_set2_btn_clicked();

    void readBackMessage(); //读取返回信息
    void readLoginBackMessages(QString msg);//读取返回登录信息槽函数；
    void readRegistBackMessages(QString msg);//读取注册返回信息

private:
    Ui::Login *ui;
    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击

    QNetworkAccessManager *m_manager;//http
    QTcpSocket *tcpSocket;//socket

    Common m_cm;
    QWidget *m_parent;  // 父窗口指针
    stu_mainwindow* m_stu_mainwindow; //学生主界面指针
    tc_mainwindow* m_tc_mainwindow; //教师主界面指针
    zj_mainwindow* m_zj_mainwindow; //助教主界面指针


};
#endif // LOGIN_H
