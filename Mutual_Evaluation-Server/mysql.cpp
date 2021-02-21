#include "mysql.h"
#include <QNetworkInterface>
#include <QtSql>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include <hiredis/hiredis.h>
#include <hiredis/read.h>
#include <hiredis/sds.h>
#include <iostream>


using std::cout;

MYSQL::MYSQL()
{
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
//    connect_mysql();
//    create_database();
//    init_database();
//    QSqlQuery query(m_database);
//    query.exec("select * from Student_Info");
//    while(query.next())
//    {
//        qDebug()<<query.value(0).toString()<<query.value(1).toString()<<query.value(2).toString()
//                <<query.value(3).toString()<<query.value(4).toString();
//    }
}

MYSQL::~MYSQL()
{
    m_database.close();
}

/**
 * @brief MYSQL::connect_mysql 连接到数据库
 */
void MYSQL::connect_mysql()
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

//创建数据库、数据表
/**
 * @brief MYSQL::create_database 如果没有指定数据库和表则创建
 */
void MYSQL::create_database()
{
    QString querystring;
    //创建数据库
    m_database = QSqlDatabase::database("testConnect");
    querystring = "CREATE DATABASE IF NOT EXISTS Mutual_Evaluation";
    m_database.exec(querystring);
    if (m_database.lastError().isValid())
    {
        qDebug()<<"Create [Mutual_Evaluation] database failed."<<m_database.lastError();
        return;
    }



    m_database.setDatabaseName("Mutual_Evaluation");
    if(!m_database.open())
    {
        qDebug()<<"[Mutual_Evaluation] database open failed";
        return;
    }else{
        qDebug()<<"[Mutual_Evaluation] database open success";
    }



    create_student_info(querystring); //创建数据表Student_Info
    create_teacher_info(querystring); //创建数据表Teacher_Info
    create_zhujiao_info(querystring); //创建数据表Zhujiao_Info

}

/**
 * @brief MYSQL::init_database 初始化数据库（添加原始值）
 */
void MYSQL::init_database()
{
    QSqlQuery query(m_database);
    query.exec("insert into Student_Info values('2017051604056', '123123', '小洋芋', '女','计算机与信息科学学院','2017级','软件工程','2班','13101291635','791188918', 0, 'false')");
    query.exec("insert into Student_Info values('2017051604057', '123123', '小洋芋2号', '女','计算机与信息科学学院','2017级','软件工程','3班','13101291635','791188918', 0, 'false')");
    query.exec("insert into Student_Info values('2017051604059', '123123', '小洋芋3号', '女','计算机与信息科学学院','2017级','软件工程','4班','13101291635','791188918', 0, 'false')");
    if(!query.exec())
    {
        qDebug() << "Error: Fail to insert in [Mutual_Evaluation].[Student_Info]." << query.lastError();

    }
}


/**
 * @brief MYSQL::user_is_exist 查询用户是否存在
 * @param current_user,user 查询哪张用户表，欲查询的用户
 * @return 用户存在，返回true；否则，返回false。
 */
bool MYSQL::user_is_exist(QString current_user, QString user)
{
    connect_mysql();
    //createDB();
    QSqlQuery query(m_database);
    QString cmd;

    if(current_user == "student")
        cmd ="select * from Student_Info where Id='"+user+"'";
    else if(current_user == "teacher")
        cmd = "select * from Teacher_Info where Id='"+user+"'";
    else if(current_user == "zhujiao")
        cmd = "select * from Zhujiao_Info where Id='"+user+"'";

    qDebug()<<"命令是" << cmd;
    query.exec(cmd);

    char token[128] = {0};


    //服务器端的数据同样采取加密操作，比较的是加密后的密文是否相同
    QCryptographicHash md5(QCryptographicHash::Md5);
    //遍历数据库信息，查询判断是否有该用户
    while(query.next()) {
        QString tmp = query.value(0).toString();
        if(tmp == user) {
            std::string name = query.value(0).toString().toStdString();
            const char* user = name.c_str();
            //qDebug() << "user: "<< user;
            //产生token码,QString转char*类型
            set_token(user,token);
            qDebug()<<"有这个"<<current_user<<" "<<user;
            return true;
        }
        qDebug()<<"没有这个"<<current_user<<" "<<user;
    }

    return false;
}



/**
 * @brief MYSQL::password_is_correct 查询用户密码是否正确
 * @param current_user, user，pwd 查询哪张用户表，欲查询的用户,用户密码
 * @return 密码正确，返回true；否则，返回false。
 */
bool MYSQL::password_is_correct(QString current_user, QString user, QString pwd){
    connect_mysql();
    //createDB();
    QSqlQuery query(m_database);
    QString cmd;

    if(current_user == "student")
        cmd ="select * from Student_Info where Id='"+user+"'";
    else if(current_user == "teacher")
        cmd = "select * from Teacher_Info where Id='"+user+"'";
    else if(current_user == "zhujiao")
        cmd = "select * from Zhujiao_Info where Id='"+user+"'";

    qDebug()<<"命令是" << cmd;
    query.exec(cmd);

    char token[128] = {0};

    //服务器端的数据同样采取加密操作，比较的是加密后的密文是否相同


    //遍历数据库信息，查询判断是否有该用户
    while(query.next()) {
        QString tmp_user = query.value(0).toString();
        if(tmp_user == user) {
            QString tmp_pwd = query.value(1).toString();
            if(tmp_pwd == pwd){
                std::string password = query.value(1).toString().toStdString();
                const char* pwd = password.c_str();
                //qDebug() << "user: "<< user;
                //产生token码,QString转char*类型
                set_token(pwd,token);
                qDebug()<<"用户存在----"<<"\n正确密码是："<<get_str_md5(tmp_pwd)<<"\n输入密码是"<<get_str_md5(pwd);
                return true;    //密码正确
            }
            qDebug()<<"用户存在----"<<"\n正确密码是："<<tmp_pwd<<"\n输入密码是"<<pwd;

           return false;        //密码错误
        }
    }
    return false;               //用户不存在
}


/**
 * @brief MYSQL::regist_success 将注册成功的用户写到数据库
 * @param current_user, user，nick_name, pwd, tel, email 注册到哪张用户表，注册的用户信息
 */
void MYSQL::regist_success(QString current_user, QString user, QString pwd)
{
    QSqlQuery query(m_database);
    QString cmd;


    if(current_user == "student")   //学生注册
        cmd ="insert into Student_Info values('"+user+"','"+pwd+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"',"+"0"+",'"+"false"+"')";
    else if(current_user == "teacher") //教师注册
        cmd ="insert into Teacher_Info values('"+user+"','"+pwd+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"',"+"0"+",'"+"false"+"')";
    else if(current_user == "zhujiao")  //助教注册
        cmd ="insert into Zhujiao_Info values('"+user+"','"+pwd+"','"+""+"','"+"" +"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+""+"','"+"false"+"')";

    qDebug()<<"命令是" << cmd;
    query.exec(cmd);

    //此处有bug
    if(!query.exec())
    {
        qDebug()<<"增加错误！！";
        qDebug() << "Error: Fail to insert in [Mutual_Evaluation].[xxxx_Info]." << query.lastError();

    }
}


/**
 * @brief MYSQL::init_stu_data 用于学生用户登录后初始化用户的数据
 * @param user... 欲查询的用户的个人数据
 */
void MYSQL::init_stu_data(QString user, QString &pwd, QString &name, QString &sex,
                    QString &academy, QString &grade, QString &major, QString &clas,
                    QString &tell, QString &qq, int &course_number, bool &completed_info)
{
    connect_mysql();
    QSqlQuery query(m_database);
    QString cmd;

    cmd ="select * from Student_Info where Id='"+user+"'";


    qDebug()<<"命令是" << cmd;
    query.exec(cmd);
    QString if_completed; //数据表中此字段是vchar类型
    while(query.next()) {
        pwd = query.value(1).toString();
        name = query.value(2).toString();
        sex = query.value(3).toString();
        academy = query.value(4).toString();
        grade = query.value(5).toString();
        major = query.value(6).toString();
        clas = query.value(7).toString();
        tell = query.value(8).toString();
        qq = query.value(9).toString();
        course_number = query.value(10).toInt();
        if_completed = query.value(11).toString();
        if(if_completed == "true")
            completed_info = true;
        else
            completed_info = false;
    }
}


/**
 * @brief MYSQL::save_stu_info 将学生用户修改过后的个人信息写到服务器
 * @param user... 欲保存的用户的个人数据
 */
//测试使用，数据库命令需要改进
void MYSQL::save_stu_info(QString user, QString name, QString sex,
                           QString academy, QString grade, QString major, QString clas,
                           QString tell, QString qq)
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
    QString cmd7;
    QString cmd8;
    QString if_completed="true";//数据表中是以vchar类型存储的

    cmd = "update Student_Info set Name='"+name+"' where Id='"+user+"'";
    cmd1 = "update Student_Info set Sex='"+sex+"' where Id='"+user+"'";
    cmd2 = "update Student_Info set Academy='"+academy+"' where Id='"+user+"'";
    cmd3 = "update Student_Info set Grade='"+grade+"' where Id='"+user+"'";
    cmd4 = "update Student_Info set Major='"+major+"' where Id='"+user+"'";
    cmd5 = "update Student_Info set Class='"+clas+"' where Id='"+user+"'";
    cmd6 = "update Student_Info set Tell='"+tell+"' where Id='"+user+"'";
    cmd7 = "update Student_Info set QQ='"+qq+"' where Id='"+user+"'";
    cmd8 = "update Student_Info set IfCompleteInfo='"+if_completed+"' where Id='"+user+"'";

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
    query.exec(cmd7);
    query.exec(cmd8);
}


/**
 * @brief MYSQL::init_tc_data 用于教师用户登录后初始化用户的数据
 * @param user... 欲查询的用户的个人数据
 */
void MYSQL::init_tc_data(QString user, QString &pwd, QString &name, QString &sex,
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
 * @brief MYSQL::save_tc_info 将教师用户修改过后的个人信息写到服务器
 * @param user... 欲保存的用户的个人数据
 */
//测试使用，数据库命令需要改进
void MYSQL::save_tc_info(QString user, QString name, QString sex, QString academy,
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
 * @brief MYSQL::save_user_pwd 将修改过后的密码写到服务器
 * @param current_user, user, pwd 保存到哪张用户表，欲保存的用户的密码
 */
void MYSQL::save_user_pwd(QString current_user, QString user, QString pwd)
{
    connect_mysql();
    QSqlQuery query(m_database);
    QString cmd;

    if(current_user == "student")
        cmd = "update Student_Info set Password='"+pwd+"' where Id='"+user+"'";
    else if(current_user == "teacher")
        cmd = "update Teacher_Info set Password='"+pwd+"' where Id='"+user+"'";

    else if(current_user == "zhujiao")
        cmd = "update Zhujiao_Info set Password='"+pwd+"' where Id='"+user+"'";

    qDebug()<<"命令是" << cmd;
    query.exec(cmd);
}



/**
 * @brief MYSQL::set_token 产生token码,QString转char*类型
 * @param user，token 用户
 * @return redis连接成功，返回true；否则，返回false。
 */
int MYSQL::set_token(const char* user, char *token)
{
    //链接redis
    redisContext* pRedisContext=(redisContext*)redisConnect("127.0.0.1",6379);
    if(pRedisContext==nullptr)
        {
            printf("Error:连接redis失败\n");
            return false;
        }
    if(pRedisContext->err!=0)
        {
            printf("Error:%s\n",pRedisContext->errstr);
            redisFree(pRedisContext);
        }

    int rand_num[4]={0};
    srand((int)time(nullptr));
    for(int i =0; i< 4;i++){
        rand_num[i] = rand()%1000;
    }
    char tmp[1024] = {0};

    sprintf(tmp, "%s%d%d%d%d", user, rand_num[0], rand_num[1], rand_num[2], rand_num[3]);
    qDebug() << "sprintf tmp::"<<tmp;

    char str[100] = { 0 };
    for (int i = 0; i < 16; i++)
    {
         sprintf(str, "%02x", rand_num[i]);
         strcat(token, str);
        }
    // redis保存此字符串，用户名，token,
    redisReply * reply = (redisReply*)redisCommand(pRedisContext,"SET %s %s",user,token);
    qDebug()<< "user and token: " << user<<"    " << token;
    if (nullptr != reply)
        {
            freeReplyObject(reply);
        }
    return true;
}


/**
 * @brief MYSQL::get_str_md5 将某个字符串加密成md5码
 * @param str 欲想加密的字符串
 * @return 加密后的字符串
 */
QString MYSQL::get_str_md5(QString str)
{
    QByteArray array;
    //md5加密
    array = QCryptographicHash::hash ( str.toLocal8Bit(), QCryptographicHash::Md5 );

    return array.toHex();
}


//创建各种表
/**
 * @brief MYSQL::create_student_info 创建Student_Info表
 */
void MYSQL::create_student_info(QString querystring){
    querystring =
            "CREATE TABLE IF NOT EXISTS Mutual_Evaluation.Student_Info\
            (\
                Id varchar(20) primary key,\
                Password varchar(100),\
                Name varchar(20),\
                Sex varchar(2),\
                Academy varchar(50), \
                Grade varchar(20),\
                Major varchar(20),\
                Class varchar(20),\
                Tell varchar(11),\
                QQ varchar(15),\
                Ncourse int(3),\
                IfCompleteInfo varchar(5)\
                )";
            m_database.exec(querystring);//执行创建数据表语句
    if (m_database.lastError().isValid())
    {
        qDebug()<<"Student_Info table creat failed:" << m_database.lastError();
        return;
    }

    //支持中文
    m_database.exec("alter table Student_Info convert to character set utf8");
}


/**
 * @brief MYSQL::create_teacher_info 创建Teacher_Info表
 */
void MYSQL::create_teacher_info(QString querystring){
    querystring =
            "CREATE TABLE IF NOT EXISTS Mutual_Evaluation.Teacher_Info\
            (\
                Id varchar(20) primary key,\
                Password varchar(100),\
                Name varchar(20),\
                Sex varchar(2),\
                Academy varchar(50), \
                Email varchar(30), \
                Tell varchar(11),\
                QQ varchar(15),\
                Ncourse int(3),\
                IfCompleteInfo varchar(5)\
                )";
            m_database.exec(querystring);//执行创建数据表语句
    if (m_database.lastError().isValid())
    {
        qDebug()<<"Teacher_Info table creat failed:" << m_database.lastError();
        return;
    }

    //支持中文
    m_database.exec("alter table Teacher_Info convert to character set utf8");
}


/**
 * @brief MYSQL::create_zhujiao_info 创建Zhujiao_Info表
 */
void MYSQL::create_zhujiao_info(QString querystring){
    querystring =
            "CREATE TABLE IF NOT EXISTS Mutual_Evaluation.Zhujiao_Info\
            (\
                Id varchar(20) primary key,\
                Password varchar(100),\
                Name varchar(20),\
                Sex varchar(2),\
                Academy varchar(50), \
                CourseId varchar(20),\
                Tell varchar(11),\
                权限1 varchar(15),\
                权限2 varchar(15),\
                权限3 varchar(15),\
                权限4 varchar(15),\
                权限5 varchar(15),\
                权限6 varchar(15),\
                IfCompleteInfo varchar(5)\
                )";
            m_database.exec(querystring);//执行创建数据表语句
    if (m_database.lastError().isValid())
    {
        qDebug()<<"Zhujiao_Info table creat failed:" << m_database.lastError();
        return;
    }

    //支持中文
    m_database.exec("alter table Zhujiao_Info convert to character set utf8");
}

