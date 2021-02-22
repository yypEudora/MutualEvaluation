#include "teacher/tc_main.h"
#include "teacher/deal_tc_info.h"
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

Tc_Main::Tc_Main(QTcpSocket *tcpSocket){
    m_tcpSocket = tcpSocket;
}

Tc_Main::~Tc_Main(){
    qDebug()<<"教师请求的服务析构掉啦！！！";
}


/**
 * @brief Tc_Main::read_service_messages 获取客户端请求的服务信息
 * @param send_buf客户端传过来的数据，当前请求登录操作的用户类别
 */
void Tc_Main::read_service_messages(QByteArray send_buf)
{
    QByteArray temp_buf = send_buf;
    //获取登录信息
    QString service;
    QString current_user = "teacher";
    QString user;
    QString pwd;
    QString name;
    QString sex;
    QString academy;
    QString email;
    QString tell;
    QString qq;
    int course_number;
    bool completed_info; //是否完善个人信息

    Deal_Tc_Info deal_tc_info{m_tcpSocket, m_info_mysql};//学生信息相关处理对象



    //获取客户端传来的请求服务信息json包
    get_service_json(temp_buf,service,user, pwd, name, sex, academy, email, tell, qq, completed_info);

    QByteArray post_data;

    /*请求用户数据和个人信息相关*/
    if(service == "acquire_user_data")                          //处理初始化用户数据
        deal_tc_info.acquire_user_data(user, pwd,name,sex,academy,email,tell,qq, course_number,completed_info);
    else if(service == "save_personal_info_to_server"){         //保存修改过的个人信息
        deal_tc_info.save_personal_info_to_server(user, name, sex, academy, email, tell, qq);
        completed_info = true;  //为防止后续请求服务中，completed_info仍为false
    }
    else if(service == "save_personal_pwd_to_server")          //保存修改过的密码
        deal_tc_info.save_personal_pwd_to_server(user,pwd);

}


/**
 * @brief Tc_Main::get_service_json 解析客户端请求服务的json数据包
 * @param temp_buf, service... 缓冲区，json数据包中想解析的内容
 */
void Tc_Main::get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &pwd, QString &name, QString &sex,
                                QString &academy, QString &email, QString &tell, QString &qq, bool &completed_info)
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
            if (object.contains("pwd")) {
                QJsonValue value = object.value("pwd");
                if (value.isString()) {
                    pwd = value.toString();
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
            if (object.contains("email")) {
                QJsonValue value = object.value("email");
                if (value.isString()) {
                    email = value.toString();
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
            }

        }
    }
    qDebug()<<"服务信息json包"<<service<<" "<<user<<" ";

}

