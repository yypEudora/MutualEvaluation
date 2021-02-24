/*
 * tc_pwd.h
 * 1.简述：教师用户修改密码界面
 * 2.主要功能：修改密码并保存
 * 3.被引用：tc_mainwindow.h
 */


#ifndef TC_PWD_H
#define TC_PWD_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Tc_Pwd;
}

class Tc_Pwd : public QWidget
{
    Q_OBJECT

public:
    explicit Tc_Pwd(QWidget *parent = nullptr);
    ~Tc_Pwd();
    void show_mainwindow(QString pwd);
    void manage_signals(); //管理信号槽连接
    bool check_pwd_valid(); //检查输入密码的合法性
    bool check_before_pwd_correct(); //验证旧密码是否正确

    // 连接服务器保存修改后的密码
    void save_personal_pwd_to_server();
    //设置发送给服务器的用户修改后的密码
    QByteArray set_personal_pwd_json();
    //解析服务器发送过来的json数据包
    void get_back_json(QByteArray back_buf, QString &service, QString &msg);


    //给教师主界面调用，查询修改过后的密码
    void updated_pwd_return(QString &pwd);
    //给login界面调用，将输入的原始密码进行加密
    void return_input_pwd(QString &pwd);
    //给login界面调用，将输入的新密码进行加密
    void return_input_new_pwd(QString &new_pwd);

    void set_ui();

    QString pwd_str_md5;    //原始密码加密，login.cpp处理
    QString new_pwd_str_md5;//新密码加密，login.cpp处理

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void back_to_mainwindow();
    void save_updated_pwd_to_server(); //修改过的个人信息写到服务器的信号，由学生主界面转发到login.cpp进行加密处理
    void check_before_pwd();            //修改过的密码写到服务器的信号，由学生主界面转发到login.cpp进行加密处理

private slots:
    // 读取服务器返回的用户数据json包
    void read_back_messages();

private:
    Ui::Tc_Pwd *ui;

    QString m_user;
    QString m_password;
    QString m_ip;
    QString m_port;

    QTcpSocket *m_tcpSocket;//socket

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
};

#endif // TC_PWD_H
