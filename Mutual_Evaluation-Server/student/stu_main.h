/*
 * stu_main.h
 * 1.解析学生用户在客户端发送过来的想要请求的服务信息json包->然后以不同的任务调用不同的函数
 * 2.被引用：mainwindow.h
 */


#ifndef STU_MAIN_H
#define STU_MAIN_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql/stu_mysql/stu_info_mysql.h"

class Stu_Main
{
public:
    Stu_Main(QTcpSocket *tcpSocket);
    ~Stu_Main();


    //获取客户端请求的服务信息
    void read_service_messages(QByteArray send_buf);
    //解析客户端请求服务的json数据包
    void get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &pwd, QString &name, QString &sex,
                          QString &academy, QString &grade, QString &major, QString &clas,
                          QString &tell, QString &qq, bool &completed_info);

public slots:



private:
    Stu_Info_MYSQL m_info_mysql; //处理学生信息相关
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
};

#endif // STU_MAIN_H
