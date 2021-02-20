#ifndef STU_MAIN_H
#define STU_MAIN_H

#include <QTcpSocket>
#include <QSqlDatabase>
#include "mysql.h"

class Stu_Main
{
public:
    Stu_Main(QTcpSocket *tcpSocket, MYSQL mysql);
    ~Stu_Main();

    void read_service_messages(QByteArray send_buf);
    void send_service_messages(QByteArray postData);
    void get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &name, QString &sex,
                          QString &academy, QString &grade, QString &major, QString &clas,
                          QString &tell, QString &qq, bool &completed_info);
    QByteArray set_user_data_back_json(QString pwd, QString name, QString sex,
                                       QString academy, QString grade, QString major, QString clas,
                                       QString tell, QString qq, int course_number, bool completed_info);

public slots:



private:
    MYSQL m_mysql; //数据库
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字

    void get_login_info(QByteArray log_buf,QString &service, QString &user);
};

#endif // STU_MAIN_H
