#ifndef DEALLOGIN_H
#define DEALLOGIN_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql.h"

class DealLogin
{
public:
    DealLogin(QTcpSocket *tcpSocket, MYSQL mysql);
    ~DealLogin();

    void read_login_messages(QByteArray send_buf, QString current_user);
    void send_login_back_messages(QByteArray postData);
    QByteArray set_login_back_json(QString msg);

public slots:



private:
    MYSQL m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
    void get_login_info(QByteArray log_buf,QString &user, QString &pwd);

};

#endif // DEALLOGIN_H
