/*
 * tc_info_mysql.h
 * 1.数据库的准备工作：连接数据库
 * 2.提供的服务：处理教师用户数据和信息相关请求(用户登录后的初始化用户数据，保存修改的个人信息，保存修改的密码)
 * 3.被引用：tc_main.h, deal_tc_info.h
 */


#ifndef TC_INFO_MYSQL_H
#define TC_INFO_MYSQL_H

#include <QSqlDatabase>

class Tc_Info_MYSQL
{
public:
    Tc_Info_MYSQL();
    ~Tc_Info_MYSQL();

    void connect_mysql(); //连接数据库

    //用于教师用户登录后初始化用户的数据
    bool init_tc_data(QString user, QString &pwd, QString &name, QString &sex,
                        QString &academy, QString &email, QString &tell, QString &qq,
                        int &course_number, bool &completed_info);

    //将教师用户修改过后的个人信息写到服务器
    bool save_tc_info(QString user, QString name, QString sex, QString academy,
                      QString email, QString tell, QString qq);

    //将修改过后的密码写到服务器
    bool save_tc_pwd(QString user, QString pwd);


private:
    QSqlDatabase m_database;
};

#endif // TC_INFO_MYSQL_H
