#include "tc_info_mysql.h"
#include <QNetworkInterface>
#include <QtSql>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include <iostream>


using std::cout;

Tc_Info_MYSQL::Tc_Info_MYSQL()
{
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
}

Tc_Info_MYSQL::~Tc_Info_MYSQL()
{
    m_database.close();
}

/**
 * @brief MYSQL::connect_mysql 连接到数据库
 */
void Tc_Info_MYSQL::connect_mysql()
{
    if (QSqlDatabase::contains("testConnect"))//判断testConnect连接是否存在并连接
    {
        m_database = QSqlDatabase::database("testConnect");
    }
    else //未连接则新建数据库连接
    {
        m_database=QSqlDatabase::addDatabase("QMYSQL","testConnect");//创建数据库连接，并为其命名testConnect
        m_database.setHostName("127.0.0.1");      //连接数据库主机名，这里需要注意（若填的为”127.0.0.1“，出现不能连接，则改为localhost)
        m_database.setPort(3306);                 //连接数据库端口号，与设置一致
        m_database.setUserName("root");          //数据库用户名，与设置一致
        m_database.setPassword("root");    //数据库密码，与设置一致
    }
    if(m_database.open()){
        qDebug()<<"connect success!";
    }
    if(!m_database.open())
    {
        qDebug()<<"fail to connect mysql:"<<m_database.lastError().text();
        return;
    }
}


/**
 * @brief Tc_Info_MYSQL::init_tc_data 用于教师用户登录后初始化用户的数据
 * @param user... 欲查询的用户的个人数据
 */
void Tc_Info_MYSQL::init_tc_data(QString user, QString &pwd, QString &name, QString &sex,
                         QString &academy, QString &email, QString &tell, QString &qq,
                         int &course_number, bool &completed_info)
{
    connect_mysql();
    QSqlQuery query(m_database);
    QString cmd;

    cmd ="select * from Teacher_Info where Id='"+user+"'";


    qDebug()<<"命令是" << cmd;
    query.exec(cmd);
    QString if_completed; //数据表中此字段是vchar类型
    while(query.next()) {
        pwd = query.value(1).toString();
        name = query.value(2).toString();
        sex = query.value(3).toString();
        academy = query.value(4).toString();
        email = query.value(5).toString();
        tell = query.value(6).toString();
        qq = query.value(7).toString();
        course_number = query.value(8).toInt();
        if_completed = query.value(9).toString();
        if(if_completed == "true")
            completed_info = true;
        else
            completed_info = false;
    }
}


/**
 * @brief Tc_Info_MYSQL::save_tc_info 将教师用户修改过后的个人信息写到服务器
 * @param user... 欲保存的用户的个人数据
 */
//测试使用，数据库命令需要改进
void Tc_Info_MYSQL::save_tc_info(QString user, QString name, QString sex, QString academy,
                         QString email, QString tell, QString qq)
{
    connect_mysql();
    QSqlQuery query(m_database);
    QString cmd;
    QString cmd1;
    QString cmd2;
    QString cmd3;
    QString cmd4;
    QString cmd5;
    QString cmd6;

    QString if_completed="true";//数据表中是以vchar类型存储的

    cmd = "update Teacher_Info set Name='"+name+"' where Id='"+user+"'";
    cmd1 = "update Teacher_Info set Sex='"+sex+"' where Id='"+user+"'";
    cmd2 = "update Teacher_Info set Academy='"+academy+"' where Id='"+user+"'";
    cmd3 = "update Teacher_Info set Email='"+email+"' where Id='"+user+"'";
    cmd4 = "update Teacher_Info set Tell='"+tell+"' where Id='"+user+"'";
    cmd5 = "update Teacher_Info set QQ='"+qq+"' where Id='"+user+"'";
    cmd6 = "update Teacher_Info set IfCompleteInfo='"+if_completed+"' where Id='"+user+"'";

        //cmd = "update Student_Info set Name='"+name+"',Sex='"+sex+"',Academy='"+academy+"',Grade='"+grade+"',Major='"+major+"',Class='"+clas+"',Tell='"+tell+"',QQ='"+qq+"',IfCompletedInfo='"+if_complete+"'"+" where Id='"+user+"'";
        //cmd = "update Student_Info set 'Name'='"+name+"','Sex'='"+sex+"','Academy'='"+academy+"','Grade'='"+grade+"','Major'='"+major+"','Class'='"+clas+"','Tell'='"+tell+"','QQ'='"+qq+"','IfCompletedInfo'='"+if_complete+"'"+" where Id='"+user+"'";
        //cmd="update Student_Info set 'Name'='"+name;

    qDebug()<<"命令是" << cmd;
    query.exec(cmd);
    query.exec(cmd1);
    query.exec(cmd2);
    query.exec(cmd3);
    query.exec(cmd4);
    query.exec(cmd5);
    query.exec(cmd6);
}



/**
 * @brief Tc_Info_MYSQL::save_tc_pwd 将教师用户修改过后的密码写到服务器
 * @param user, pwd 欲保存的用户的密码
 */
void Tc_Info_MYSQL::save_tc_pwd(QString user, QString pwd)
{
    connect_mysql();
    QSqlQuery query(m_database);
    QString cmd;

    cmd = "update Teacher_Info set Password='"+pwd+"' where Id='"+user+"'";

    qDebug()<<"命令是" << cmd;
    query.exec(cmd);
}
