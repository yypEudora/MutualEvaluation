/*
 * tc_main.h
 * 1.解析教师用户在客户端发送过来的想要请求的服务信息json包->然后以不同的任务调用不同的函数
 * 2.被引用：mainwindow.h
 */


#ifndef TC_MAIN_H
#define TC_MAIN_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql/tc_mysql/tc_info_mysql.h"

class Tc_Main
{
public:
    Tc_Main(QTcpSocket *tcpSocket);
    ~Tc_Main();


    //获取客户端请求的服务信息
    void read_module_messages(QByteArray send_buf);
    //解析客户端请求服务的json数据包
    void get_module_json(QByteArray temp_buf, QString &module);

private:
    Tc_Info_MYSQL m_info_mysql; //处理教师信息相关
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
};

#endif // TC_MAIN_H
