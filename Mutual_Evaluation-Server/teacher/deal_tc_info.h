/*
 * deal_tc_info.h
 * 1.处理教师用户数据和个人信息相关：登录时初始化用户数据，保存修改的个人信息，保存修改的密码
 * 2.设置客户端请求服务的反馈信息的json数据包->然后以不同的任务调用不同的函数，设置客户端请求服务的反馈信息的json数据包，发送客户端请求服务反馈信息的json数据包
 * 3.被引用：tc_main.h
 */


#ifndef DEAL_TC_INFO_H
#define DEAL_TC_INFO_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql/tc_mysql/tc_info_mysql.h"

class Deal_Tc_Info
{
public:
    Deal_Tc_Info(QTcpSocket *tcpSocket, Tc_Info_MYSQL mysql);
    ~Deal_Tc_Info();

    //处理初始化用户数据
    void acquire_user_data(QString user, QString pwd, QString name,
                           QString sex, QString academy, QString email, QString tell, QString qq,
                           int course_number,bool completed_info);

    //保存修改过的个人信息
    void save_personal_info_to_server(QString user, QString name,
                                      QString sex, QString academy, QString email, QString tell, QString qq);

    //保存修改过的密码
    void save_personal_pwd_to_server(QString user, QString pwd);


    //获取客户端请求的服务信息
    void read_service_messages(QByteArray send_buf);
    //解析客户端请求服务的json数据包
    void get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &pwd, QString &name, QString &sex,
                          QString &academy, QString &email, QString &tell, QString &qq, bool &completed_info);

    void send_service_messages(QByteArray postData); //发送客户端请求服务反馈信息的json数据包

    //设置客户端请求服务的反馈信息的json数据包
    QByteArray set_user_data_back_json(QString msg, QString pwd, QString name, QString sex,
                                       QString academy, QString email, QString tell,
                                       QString qq, int course_number, bool completed_info);

    QByteArray set_user_save_info_back_json(QString msg);
    QByteArray set_user_save_pwd_back_json(QString msg);

public slots:



private:
    Tc_Info_MYSQL m_info_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
};

#endif // DEAL_TC_INFO_H
