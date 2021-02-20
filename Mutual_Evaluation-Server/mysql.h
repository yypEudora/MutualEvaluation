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
    void create_student_info(QString querystring); //创建Student_Info表
    void create_teacher_info(QString querystring); //创建Teacher_Info表
    void create_zhujiao_info(QString querystring); //创建Zhujiao_Info表
    void init_database();      //初始化数据

    bool user_is_exist(QString current_user, QString user);  //判断用户是否存在
    bool password_is_correct(QString current_user, QString user, QString pwd);  //判断密码是否正确
    void regist_success(QString current_user, QString user, QString pwd);//注册到数据库

    //用于用户登录后初始化用户的数据
    void init_user_data(QString current_user, QString user, QString &pwd, QString &name, QString &sex,
                        QString &academy, QString &grade, QString &major, QString &clas,
                        QString &tell, QString &qq, int &course_number, bool &completed_info);
    //将修改过后的个人信息写到服务器
    void save_user_info(QString current_user, QString user, QString name, QString sex,
                        QString academy, QString grade, QString major, QString clas,
                        QString tell, QString qq, bool completed_info);


    int set_token(const char *user, char *token);
    QString get_str_md5(QString str);

private:
    QSqlDatabase m_database;
};

#endif // MYSQL_H
