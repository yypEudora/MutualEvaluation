#ifndef DEALREGIST_H
#define DEALREGIST_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql.h"

class DealRegist
{
public:
    DealRegist(QTcpSocket *tcpSocket, MYSQL mysql);
    ~DealRegist();

    void read_regist_messages(QByteArray send_buf);
    void send_regist_back_messages(QByteArray postData);
    QByteArray set_regist_back_json(QString msg);

public slots:


private:
    MYSQL m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字

    //解析用户注册信息的json包
    void get_regist_info(QByteArray reg_buf,QString &user, QString &nick_name, QString &pwd, QString &tel, QString &email);

};

#endif // DEALREGIST_H
