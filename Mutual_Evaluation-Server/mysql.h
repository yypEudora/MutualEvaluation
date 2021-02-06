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
    void init_database();      //初始化数据

    bool user_is_exist(QString user);  //判断用户是否存在
    bool password_is_correct(QString user, QString pwd);  //判断密码是否正确
    void regist_success(QString user, QString nick_name, QString pwd, QString tel, QString email);//注册到数据库
    int set_token(const char *user, char *token);
    QString get_str_md5(QString str);

private:
    QSqlDatabase m_database;
};

#endif // MYSQL_H
