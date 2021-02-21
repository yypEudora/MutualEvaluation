#ifndef DEAL_STU_INFO_H
#define DEAL_STU_INFO_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql.h"

class Deal_Stu_Info
{
public:
    Deal_Stu_Info(QTcpSocket *tcpSocket, MYSQL mysql);
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
    void save_personal_pwd_to_server(QString current_user, QString user, QString pwd);



    void send_service_messages(QByteArray postData); //发送客户端请求服务反馈信息的json数据包
    //设置客户端请求服务的反馈信息的json数据包
    QByteArray set_user_data_back_json(QString pwd, QString name, QString sex,
                                       QString academy, QString grade, QString major, QString clas,
                                       QString tell, QString qq, int course_number, bool completed_info);



public slots:



private:
    MYSQL m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字
};

#endif // DEAL_STU_INFO_H
