#include "dealLogin.h"
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

DealLogin::DealLogin(QTcpSocket *tcpSocket, MYSQL_Main mysql){
    m_tcpSocket = tcpSocket;
    m_mysql = mysql;
}

DealLogin::~DealLogin(){
    qDebug()<<"我登录服务析构掉啦！！！";
}

/**
 * @brief DealLogin::read_login_messages 获取想要登录的用户信息
 * @param send_buf,current_user 客户端传过来的数据，当前请求登录操作的用户类别
 */
void DealLogin::read_login_messages(QByteArray send_buf, QString current_user)
{
    QByteArray log_buf = send_buf;
    //获取登录信息
    QString user;
    QString pwd;
    get_login_info(log_buf,user,pwd);

    QString msg; //用于判断是否登录成功

    //this->setWindowTitle(msg);
    if(this->m_mysql.user_is_exist(current_user, user)) { //用户存在
        if(m_mysql.password_is_correct(current_user, user, pwd)){  //密码正确
            msg = "true";                  //可以进行登录
        }
        else msg = "PasswordWrong";        //密码错误
    } else {
        msg = "NotFound";                  //用户不存在
    }
    qDebug()<<"msg1是："<<msg;

    QByteArray postData = set_login_back_json(msg);

    this->send_login_back_messages(postData);
}


/**
 * @brief DealLogin::get_login_info 解析登录的用户信息json包
 * @param log_buf, user, pwd  保存用户登录信息的缓冲区，用以返回的用户名和密码
 */
void DealLogin::get_login_info(QByteArray log_buf,QString &user, QString &pwd)
{
        /*json数据如下
        {
            user:xxxx,
            pwd:xxx,
        }
        */
        //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(log_buf, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("user")) {  // 包含指定的 key
                QJsonValue value = object.value("user");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    user = value.toString();  // 将 value 转化为字符串

                }
            }
            if (object.contains("pwd")) {
                QJsonValue value = object.value("pwd");
                if (value.isString()) {
                    pwd = value.toString();
                }
            }
        }
    }
    qDebug()<<"登录信息包："<<user<<" "<<pwd<<" ";
}



/**
 * @brief DealLogin::send_login_back_messages 给客户端发送回复消息，以说明登录是否成功。
 *      如果回复"true"，表示登录成功；回复为"PasswordWrong",表示密码错误；
 *      回复"NotFound"表示用户不存在
 * @param postData 发送给客户端的数据包
 */
void DealLogin::send_login_back_messages(QByteArray postData)
{
    if(postData == nullptr)
        return;
    this->m_tcpSocket->write(postData,postData.length());//发送登录反馈信息数据包
    qDebug()<<"发送数据包";
}


/**
 * @brief DealLogin::set_login_back_json 设置客户端需要使用登录结果反馈的json数据包
 * @param msg,user_info_has_completed 欲发送给客户端登录操作产生的结果，msg有'true', 'NotFound', 'PasswordWrong'
 *                                    以及用户是否完善了信息,user_info_has_completed有'true'和'false'
 * @return 设置好的json数据包
 */
QByteArray DealLogin::set_login_back_json(QString msg){
    QMap<QString, QVariant> login;
    login.insert("sender","login"); //告诉客户端这是进行了登录操作的反馈信息
    login.insert("msg",msg);
    /*json数据如
        {
            sender:xxxx,
            msg:xxx
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(login);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}
