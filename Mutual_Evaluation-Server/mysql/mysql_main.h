/*
 * mysql_main.h
 * 1.数据库的准备工作：用于连接数据库，创建/打开数据库，创建/打开各种数据表, 初始化数据表里的数据
 * 2.提供的服务：处理登录注册相关请求(查询用户是否存在，查询用户密码是否正确，注册用户)
 * 3.被引用：mainwindow.h
 */


#ifndef MYSQL_MAIN_H
#define MYSQL_MAIN_H

#include <QSqlDatabase>

class MYSQL_Main
{
public:
    MYSQL_Main();
    ~MYSQL_Main();

    void connect_mysql(); //连接数据库
    void create_database();     //数据库不存在则创建

    /*创建各种表*/
    void create_student_info(QString querystring); //创建Student_Info表
    void create_teacher_info(QString querystring); //创建Teacher_Info表
    void create_zhujiao_info(QString querystring); //创建Zhujiao_Info表
    void init_database();      //初始化数据

    /*处理登录注册相关*/
    bool user_is_exist(QString current_user, QString user);  //判断用户是否存在
    bool password_is_correct(QString current_user, QString user, QString pwd);  //判断密码是否正确
    void regist_success(QString current_user, QString user, QString pwd);//注册到数据库


    int set_token(const char *user, char *token);

private:
    QSqlDatabase m_database;
};

#endif // MYSQL_MAIN_H
