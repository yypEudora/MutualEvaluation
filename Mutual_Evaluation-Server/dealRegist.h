/*
 * dealRegist.h
 * 1.解析客户端发送过来的想要注册的用户信息json包->调用数据库注册函数，设置用户注册的反馈信息json包->将设置好的反馈信息json包发送给客户端
 * 2.被引用：mainwindow.h
 */



#ifndef DEALREGIST_H
#define DEALREGIST_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql/mysql_main.h"

class DealRegist
{
public:
    DealRegist(QTcpSocket *tcpSocket, MYSQL_Main mysql);
    ~DealRegist();

    void read_regist_messages(QByteArray send_buf, QString current_user);   //解析客户端发送过来的想要注册的用户信息json包
    void send_regist_back_messages(QByteArray postData);                    //将设置好的反馈信息json包发送给客户端
    QByteArray set_regist_back_json(QString msg);                           //设置用户注册的反馈信息json包

public slots:


private:
    MYSQL_Main m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字

    //解析用户注册信息的json包
    void get_regist_info(QByteArray reg_buf,QString &user, QString &pwd);

};

#endif // DEALREGIST_H
