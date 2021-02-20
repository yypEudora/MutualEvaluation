#include "stu_main.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QNetworkInterface>
#include <QtSql>
#include <iostream>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHostAddress>

using std::cout;

Stu_Main::Stu_Main(QTcpSocket *tcpSocket, MYSQL mysql){
    m_tcpSocket = tcpSocket;
    m_mysql = mysql;
}

Stu_Main::~Stu_Main(){
    qDebug()<<"学生请求的服务析构掉啦！！！";
}


/**
 * @brief Stu_Main::read_service_messages 获取想要登录的用户信息
 * @param send_buf客户端传过来的数据，当前请求登录操作的用户类别
 */
void Stu_Main::read_service_messages(QByteArray send_buf)
{
    QByteArray temp_buf = send_buf;
    //获取登录信息
    QString service;
    QString user;
    QString password;
    QString name;
    QString sex;
    QString academy;
    QString grade;
    QString major;
    QString clas;
    QString tell;
    QString qq;
    int course_number;
    bool completed_info; //是否完善个人信息

    //获取客户端传来的请求服务信息json包
    get_service_json(temp_buf,service,user,name,sex,academy, grade, major, clas,tell, qq, completed_info);

    QByteArray post_data;
    if(service == "acquire_user_data"){
        //处理初始化用户数据
        m_mysql.init_user_data("student", user, password,name,sex,academy,grade,major, clas,tell,qq,course_number,completed_info);
        post_data = set_user_data_back_json(password,name,sex,academy,grade,major, clas,tell,qq,course_number,completed_info);
    }
    else if(service == "save_personal_info_to_server"){
        m_mysql.save_user_info("student", user, name, sex, academy, grade, major, clas, tell, qq, completed_info);
    }

    send_service_messages(post_data);
}


void Stu_Main::send_service_messages(QByteArray postData)
{
    if(postData == nullptr)
        return;
    this->m_tcpSocket->write(postData,postData.length());//发送客户端请求服务的反馈信息数据包
    qDebug()<<"发送数据包";
}

void Stu_Main::get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &name, QString &sex,
                                QString &academy, QString &grade, QString &major, QString &clas,
                                QString &tell, QString &qq, bool &completed_info)
{        /*json数据如下
        {
            sender:xxxx,
            service:xxx,
            user:xxx
            ...
        }
        */
        //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(temp_buf, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("service")) {  // 包含指定的 key
                QJsonValue value = object.value("service");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    service = value.toString();  // 将 value 转化为字符串

                }
            }
            if (object.contains("user")) {
                QJsonValue value = object.value("user");
                if (value.isString()) {
                    user = value.toString();
                }
            }
            if (object.contains("name")) {
                QJsonValue value = object.value("name");
                if (value.isString()) {
                    name = value.toString();
                }
            }
            if (object.contains("sex")) {
                QJsonValue value = object.value("sex");
                if (value.isString()) {
                    sex = value.toString();
                }
            }
            if (object.contains("academy")) {
                QJsonValue value = object.value("academy");
                if (value.isString()) {
                    academy = value.toString();
                }
            }
            if (object.contains("grade")) {
                QJsonValue value = object.value("grade");
                if (value.isString()) {
                    grade = value.toString();
                }
            }
            if (object.contains("major")) {
                QJsonValue value = object.value("major");
                if (value.isString()) {
                    major = value.toString();
                }
            }
            if (object.contains("class")) {
                QJsonValue value = object.value("class");
                if (value.isString()) {
                    clas = value.toString();
                }
            }
            if (object.contains("tell")) {
                QJsonValue value = object.value("tell");
                if (value.isString()) {
                    tell = value.toString();
                }
            }
            if (object.contains("qq")) {
                QJsonValue value = object.value("qq");
                if (value.isString()) {
                    qq = value.toString();
                }
            }
            if (object.contains("completed_info")) {
                QJsonValue value = object.value("completed_info");
                if (value.isString()) {
                    completed_info = value.toBool();
                }
                qDebug()<<"completed_info"<<completed_info;
            }

        }
    }
    qDebug()<<"服务信息json包"<<service<<" "<<user<<" ";

}


QByteArray Stu_Main::set_user_data_back_json(QString pwd, QString name, QString sex,
                                        QString academy, QString grade, QString major, QString clas,
                                        QString tell, QString qq, int course_number, bool completed_info)
{

    QMap<QString, QVariant> user_data;
    user_data.insert("service","acquire_user_data"); //告诉客户端这是进行了请求用户数据的反馈信息
    user_data.insert("pwd",pwd);
    user_data.insert("name",name);
    user_data.insert("sex", sex);
    user_data.insert("academy",academy);
    user_data.insert("grade",grade);
    user_data.insert("major",major);
    user_data.insert("clas",clas);
    user_data.insert("tell",tell);
    user_data.insert("qq",qq);
    user_data.insert("course_number",course_number);
    user_data.insert("completed_info", completed_info);
    /*json数据如
        {
            service:xxxx,
            .....
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(user_data);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}
