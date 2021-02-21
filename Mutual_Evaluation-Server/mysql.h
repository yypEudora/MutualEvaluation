#ifndef MYSQL_H
#define MYSQL_H

#include <QSqlDatabase>

class MYSQL
{
public:
    MYSQL();
    ~MYSQL();

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

    /*统一处理不同用户类别的服务，如：修改密码*/
    //将修改过后的密码写到服务器
    void save_user_pwd(QString current_user, QString user, QString pwd);


    /*不统一处理不同用户类别的服务，如：初始化用户数据，修改个人信息*/
    //用于学生用户登录后初始化用户的数据
    void init_stu_data(QString user, QString &pwd, QString &name, QString &sex,
                        QString &academy, QString &grade, QString &major, QString &clas,
                        QString &tell, QString &qq, int &course_number, bool &completed_info);
    //将学生用户修改过后的个人信息写到服务器
    void save_stu_info(QString user, QString name, QString sex,
                        QString academy, QString grade, QString major, QString clas,
                        QString tell, QString qq);
    //用于教师用户登录后初始化用户的数据
    void init_tc_data(QString user, QString &pwd, QString &name, QString &sex,
                        QString &academy, QString &email, QString &tell, QString &qq,
                        int &course_number, bool &completed_info);

    //将教师用户修改过后的个人信息写到服务器
    void save_tc_info(QString user, QString name, QString sex, QString academy,
                      QString email, QString tell, QString qq);


    int set_token(const char *user, char *token);
    QString get_str_md5(QString str);

private:
    QSqlDatabase m_database;
};

#endif // MYSQL_H
