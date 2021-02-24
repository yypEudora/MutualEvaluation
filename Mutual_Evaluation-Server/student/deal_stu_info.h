/*
 * deal_stu_info.h
 * 1.处理学生用户数据和个人信息相关：登录时初始化用户数据，保存修改的个人信息，保存修改的密码
 * 2.解析学生用户在客户端发送过来的想要请求的服务信息json包->然后以不同的任务调用不同的函数，设置客户端请求服务的反馈信息的json数据包，发送客户端请求服务反馈信息的json数据包
 * 3.被引用：stu_main.h
 */


#ifndef DEAL_STU_INFO_H
#define DEAL_STU_INFO_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql/stu_mysql/stu_info_mysql.h"

class Deal_Stu_Info
{
public:
    Deal_Stu_Info(QTcpSocket *tcpSocket, Stu_Info_MYSQL info_mysql);
    ~Deal_Stu_Info();

    //处理初始化用户数据
    void acquire_user_data(QString user, QString pwd, QString name,
                           QString sex, QString academy, QString grade, QString major,
                           QString clas, QString tell, QString qq, int course_number,
                           bool completed_info);

    //保存修改过的个人信息
    void save_personal_info_to_server(QString user, QString name,
                                      QString sex, QString academy, QString grade, QString major,
                                      QString clas, QString tell, QString qq);

    //保存修改过的密码
    void save_personal_pwd_to_server(QString user, QString pwd);


    //获取客户端请求的服务信息
    void read_service_messages(QByteArray send_buf);
    //解析客户端请求服务的json数据包
    void get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &pwd, QString &name, QString &sex,
                          QString &academy, QString &grade, QString &major, QString &clas,
                          QString &tell, QString &qq, bool &completed_info);


    //发送客户端请求服务反馈信息的json数据包
    void send_service_messages(QByteArray postData);


    //设置客户端请求服务的反馈信息的json数据包
    QByteArray set_user_data_back_json(QString msg, QString pwd, QString name, QString sex,
                                       QString academy, QString grade, QString major, QString clas,
                                       QString tell, QString qq, int course_number, bool completed_info);
    QByteArray set_user_save_info_back_json(QString msg);
    QByteArray set_user_save_pwd_back_json(QString msg);



public slots:



private:
    Stu_Info_MYSQL m_info_mysql; //处理学生信息相关
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
};

#endif // DEAL_STU_INFO_H
