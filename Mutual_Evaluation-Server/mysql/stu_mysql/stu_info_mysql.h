/*
 * stu_info_mysql.h
 * 1.数据库的准备工作：连接数据库
 * 2.提供的服务：处理学生用户数据和信息相关请求(用户登录后的初始化用户数据，保存修改的个人信息，保存修改的密码)
 * 3.被引用：stu_main.h, deal_stu_info.h
 */


#ifndef STU_INFO_MYSQL_H
#define STU_INFO_MYSQL_H

#include <QSqlDatabase>

class Stu_Info_MYSQL
{
public:
    Stu_Info_MYSQL();
    ~Stu_Info_MYSQL();

    void connect_mysql(); //连接数据库

    //用于学生用户登录后初始化用户的数据
    bool init_stu_data(QString user, QString &pwd, QString &name, QString &sex,
                        QString &academy, QString &grade, QString &major, QString &clas,
                        QString &tell, QString &qq, int &course_number, bool &completed_info);
    //将学生用户修改过后的个人信息写到服务器
    bool save_stu_info(QString user, QString name, QString sex,
                        QString academy, QString grade, QString major, QString clas,
                        QString tell, QString qq);
    //将学生用户修改过后的密码写到服务器
    bool save_stu_pwd(QString user, QString pwd);

private:
    QSqlDatabase m_database;
};

#endif // STU_INFO_MYSQL_H
