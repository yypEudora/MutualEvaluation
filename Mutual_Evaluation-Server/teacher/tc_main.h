#ifndef TC_MAIN_H
#define TC_MAIN_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql.h"

class Tc_Main
{
public:
    Tc_Main(QTcpSocket *tcpSocket, MYSQL mysql);
    ~Tc_Main();


    //获取客户端请求的服务信息
    void read_service_messages(QByteArray send_buf);
    //解析客户端请求服务的json数据包
    void get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &pwd, QString &name, QString &sex,
                          QString &academy, QString &email, QString &tell, QString &qq, bool &completed_info);

private:
    MYSQL m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
};

#endif // TC_MAIN_H
