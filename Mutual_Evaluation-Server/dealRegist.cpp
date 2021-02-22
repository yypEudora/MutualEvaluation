#include "dealRegist.h"
#include <iostream>
#include <QDebug>
#include <QNetworkInterface>
#include <QtSql>
#include <iostream>
using std::cout;


DealRegist::DealRegist(QTcpSocket *tcpSocket, MYSQL_Main mysql){
    m_tcpSocket = tcpSocket;
    m_mysql = mysql;
}

DealRegist::~DealRegist(){
    qDebug()<<"我注册服务析构掉啦！！！";
}


/**
 * @brief DealRegist::read_regist_messages 获取想要注册的用户信息
 * @param send_buf, current_user 客户端传过来的数据，当前请求注册操作的用户类别
 */
void DealRegist::read_regist_messages(QByteArray send_buf, QString current_user)
{
    QByteArray reg_buf = send_buf;
    //获取注册信息
    QString user;
    QString pwd;
    get_regist_info(reg_buf,user, pwd);

    QString msg; //用于判断是否注册成功
    if(!this->m_mysql.user_is_exist(current_user, user)) {
        msg = "true";   //用户不存在,可以进行注册
        m_mysql.regist_success(current_user, user, pwd);//注册个人信息到数据库
    } else {
        msg = "Existed";//用户已经存在，不能进行注册
    }
    qDebug()<<"msg1是："<<msg;

    QByteArray postData = set_regist_back_json(msg);
    this->send_regist_back_messages(postData);

}


/**
 * @brief DealRegist::get_regist_info 解析注册的用户信息json包
 * @param reg_buf, user, nick_name, pwd, tel, email  保存用户注册信息的缓冲区，用以返回的用户注册信息
 */
void DealRegist::get_regist_info(QByteArray reg_buf,QString &user, QString &pwd)
{
        /*json数据如下
        {
            user:xxxx,
            firstPwd:xxx,
        }
        */
        //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(reg_buf, &jsonError);  // 转化为 JSON 文档
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
    qDebug()<<"注册信息包："<<user<<" "<<pwd;
}


/**
 * @brief DealRegist::send_regist_back_messages 给客户端发送回复消息，以说明注册验证是否成功。
 *      如果回复"true"，表示注册成功；回复为"Existed",表示用户已存在；
 * @param postData 发送给客户端的数据包
 */
void DealRegist::send_regist_back_messages(QByteArray postData)
{
    if(postData == nullptr)
        return;
    this->m_tcpSocket->write(postData,postData.length());//发送登录反馈信息数据包
    qDebug()<<"发送数据包";
}


/**
 * @brief DealRegist::set_regist_back_json 设置客户端需要使用注册结果反馈的json数据包
 * @param msg 欲发送给客户端登录操作产生的结果，有'true', 'Existed'
 * @return 设置好的json数据包
 */
QByteArray DealRegist::set_regist_back_json(QString msg){
    QMap<QString, QVariant> login;
    login.insert("sender","register");
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
