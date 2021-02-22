/*
 * dealLogin.h
 * 1.解析客户端发送过来的想要登录的用户信息json包，设置用户登录的反馈信息json包->将设置好的反馈信息json包发送给客户端
 * 2.被引用：mainwindow.h
 */


#ifndef DEALLOGIN_H
#define DEALLOGIN_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql/mysql_main.h"

class DealLogin
{
public:
    DealLogin(QTcpSocket *tcpSocket, MYSQL_Main mysql);
    ~DealLogin();

    void read_login_messages(QByteArray send_buf, QString current_user);    //解析客户端发送过来的想要登录的用户信息json包
    void send_login_back_messages(QByteArray postData);                     //将设置好的反馈信息json包发送给客户端
    QByteArray set_login_back_json(QString msg);                            //设置用户登录的反馈信息json包

public slots:



private:
    MYSQL_Main m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
    void get_login_info(QByteArray log_buf,QString &user, QString &pwd);

};

#endif // DEALLOGIN_H
