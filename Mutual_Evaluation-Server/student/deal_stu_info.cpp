#include "student/deal_stu_info.h"
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

Deal_Stu_Info::Deal_Stu_Info(QTcpSocket *tcpSocket, Stu_Info_MYSQL info_mysql){
    m_tcpSocket = tcpSocket;
    m_info_mysql = info_mysql;
}

Deal_Stu_Info::~Deal_Stu_Info(){

}


/**
 * @brief Deal_Stu_Info::acquire_user_data 处理初始化学生用户数据
 * @param current_user, user... 当前用户类型，请求保存的数据
 */
void Deal_Stu_Info::acquire_user_data(QString user, QString pwd, QString name,
                                      QString sex, QString academy, QString grade, QString major,
                                      QString clas, QString tell, QString qq, int course_number,
                                      bool completed_info)
{
    QByteArray post_data;
    QString msg;
    if(m_info_mysql.init_stu_data(user, pwd,name,sex,academy,grade,major, clas,tell,qq,course_number,completed_info))
        msg = "true";
    else
        msg = "false";
    post_data = set_user_data_back_json(msg,pwd,name,sex,academy,grade,major, clas,tell,qq,course_number,completed_info);
    send_service_messages(post_data);
}


/**
 * @brief Deal_Stu_Info::save_personal_info_to_server 保存修改后的个人信息
 * @param current_user, user... 当前用户类型，请求保存的数据
 */
void Deal_Stu_Info::save_personal_info_to_server(QString user, QString name,
                                  QString sex, QString academy, QString grade, QString major,
                                  QString clas, QString tell, QString qq)
{
    QByteArray post_data;
    QString msg;
    if(m_info_mysql.save_stu_info(user, name, sex, academy, grade, major, clas, tell, qq))
        msg = "true";
    else
        msg = "false";
    post_data = set_user_save_info_back_json(msg);
    send_service_messages(post_data);

}


/**
 * @brief Deal_Stu_Info::save_personal_pwd_to_server 保存修改后的密码
 * @param current_user, user... 当前用户类型，请求保存的数据
 */
void Deal_Stu_Info::save_personal_pwd_to_server(QString user, QString pwd)
{
    QByteArray post_data;
    QString msg;
    if(m_info_mysql.save_stu_pwd(user, pwd))
        msg = "true";
    else
        msg = "false";
    post_data = set_user_save_pwd_back_json(msg);
    send_service_messages(post_data);
}


/**
 * @brief Deal_Stu_Info::read_service_messages 获取客户端请求的服务信息
 * @param send_buf客户端传过来的数据
 */
void Deal_Stu_Info::read_service_messages(QByteArray send_buf)
{
    QByteArray temp_buf = send_buf;
    //获取登录信息
    QString service;
    QString current_user = "student";
    QString user;
    QString pwd;
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
    get_service_json(temp_buf,service,user, pwd, name,sex,academy, grade, major, clas,tell, qq, completed_info);
    qDebug()<<"【进行的服务是】"<<service;
    /*请求用户数据和个人信息相关*/
    if(service == "acquire_user_data"){                          //处理初始化用户数据
        acquire_user_data(user, pwd,name,sex,academy,grade,major, clas,tell,qq,course_number,completed_info);
    }
    else if(service == "save_personal_info_to_server"){         //保存修改过的个人信息
        save_personal_info_to_server(user, name, sex, academy, grade, major, clas, tell, qq);
        completed_info = true;  //为防止后续请求服务中，completed_info仍为false

    }
    else if(service == "save_personal_pwd_to_server"){          //保存修改过的密码
        save_personal_pwd_to_server(user,pwd);

    }
}



/**
 * @brief Deal_Stu_Info::get_service_json 解析客户端请求服务的json数据包
 * @param temp_buf, service... 缓冲区，json数据包中想解析的内容
 */
void Deal_Stu_Info::get_service_json(QByteArray temp_buf, QString &service, QString &user, QString &pwd, QString &name, QString &sex,
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
            }

        }
    }
}



/**
 * @brief Deal_Stu_Info::send_service_messages 发送客户端请求服务反馈信息的json数据包
 * @param postData 反馈信息数据包
 */
void Deal_Stu_Info::send_service_messages(QByteArray postData)
{
    if(postData == nullptr)
        return;
    this->m_tcpSocket->write(postData,postData.length());
    qDebug()<<"发送数据包";
}


/**
 * @brief Deal_Stu_Info::set_user_data_back_json 设置客户端请求服务的反馈信息的json数据包
 * @param msg, pwd, name, sex... json数据包中设置的内容
 */
QByteArray Deal_Stu_Info::set_user_data_back_json(QString msg, QString pwd, QString name, QString sex,
                                        QString academy, QString grade, QString major, QString clas,
                                        QString tell, QString qq, int course_number, bool completed_info)
{

    QMap<QString, QVariant> user_data;
    user_data.insert("service","acquire_user_data"); //告诉客户端这是进行了请求用户数据的反馈信息
    user_data.insert("msg", msg);
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



/**
 * @brief Deal_Stu_Info::set_user_save_info_back_json 设置客户端请求服务的反馈信息的json数据包
 * @param msg 是否成功
 */
QByteArray Deal_Stu_Info::set_user_save_info_back_json(QString msg)
{
    QMap<QString, QVariant> back_msg;
    back_msg.insert("service","save_personal_info_to_server"); //告诉客户端这是进行了请求保存/修改个人信息的反馈信息
    back_msg.insert("msg",msg);
    /*json数据如
        {
            service:xxxx,
            .....
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(back_msg);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}


/**
 * @brief Deal_Stu_Info::set_user_save_pwd_back_json 设置客户端请求服务的反馈信息的json数据包
 * @param msg 是否成功
 */
QByteArray Deal_Stu_Info::set_user_save_pwd_back_json(QString msg)
{
    QMap<QString, QVariant> back_msg;
    back_msg.insert("service","save_personal_pwd_to_server"); //告诉客户端这是进行了请求修改密码的反馈信息
    back_msg.insert("msg",msg);
    /*json数据如
        {
            service:xxxx,
            .....
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(back_msg);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();

}
