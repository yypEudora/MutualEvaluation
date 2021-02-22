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

Deal_Tc_Info::Deal_Tc_Info(QTcpSocket *tcpSocket, Tc_Info_MYSQL info_mysql){
    m_tcpSocket = tcpSocket;
    m_info_mysql = info_mysql;
}

Deal_Tc_Info::~Deal_Tc_Info(){
    qDebug()<<"处理教师信息的服务析构掉啦！！！";
}


/**
 * @brief Deal_Tc_Info::acquire_user_data 处理初始化学生用户数据
 * @param current_user, user... 当前用户类型，请求保存的数据
 */
void Deal_Tc_Info::acquire_user_data(QString user, QString pwd, QString name,
                                      QString sex, QString academy, QString email, QString tell,
                                     QString qq, int course_number, bool completed_info)
{
    QByteArray post_data;
    m_info_mysql.init_tc_data(user, pwd,name,sex,academy,email,tell,qq,course_number,completed_info);
    post_data = set_user_data_back_json(pwd,name,sex,academy,email,tell,qq,course_number,completed_info);
    send_service_messages(post_data);
}


/**
 * @brief Deal_Tc_Info::save_personal_info_to_server 保存修改后的个人信息
 * @param current_user, user... 当前用户类型，请求保存的数据
 */
void Deal_Tc_Info::save_personal_info_to_server(QString user, QString name,
                                  QString sex, QString academy, QString email, QString tell, QString qq)
{
    m_info_mysql.save_tc_info(user, name, sex, academy, email, tell, qq);

}


/**
 * @brief Deal_Tc_Info::save_personal_pwd_to_server 保存修改后的密码
 * @param user... 当前用户类型，请求保存的数据
 */
void Deal_Tc_Info::save_personal_pwd_to_server(QString user, QString pwd)
{
    m_info_mysql.save_tc_pwd(user, pwd);
}


/**
 * @brief Deal_Tc_Info::send_service_messages 发送客户端请求服务反馈信息的json数据包
 * @param postData 反馈信息数据包
 */
void Deal_Tc_Info::send_service_messages(QByteArray postData)
{
    if(postData == nullptr)
        return;
    this->m_tcpSocket->write(postData,postData.length());
    qDebug()<<"发送数据包";
}


/**
 * @brief Deal_Tc_Info::get_service_json 设置客户端请求服务的反馈信息的json数据包
 * @param pwd, name, sex... json数据包中设置的内容
 */
QByteArray Deal_Tc_Info::set_user_data_back_json(QString pwd, QString name, QString sex,
                                            QString academy, QString email, QString tell, QString qq,
                                            int course_number, bool completed_info)
{

    QMap<QString, QVariant> user_data;
    user_data.insert("service","acquire_user_data"); //告诉客户端这是进行了请求用户数据的反馈信息
    user_data.insert("pwd", pwd);
    user_data.insert("name", name);
    user_data.insert("sex", sex);
    user_data.insert("academy", academy);
    user_data.insert("email", email);
    user_data.insert("tell", tell);
    user_data.insert("qq", qq);
    user_data.insert("course_number", course_number);
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
