#include "student/deal_stu_info.h"
#include "student/stu_main.h"
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

Stu_Main::Stu_Main(QTcpSocket *tcpSocket){
    m_tcpSocket = tcpSocket;
}

Stu_Main::~Stu_Main(){

}


/**
 * @brief Stu_Main::read_module_messages 获取客户端发送的请求的服务信息
 * @param send_buf 客户端传过来的数据
 */
void Stu_Main::read_module_messages(QByteArray send_buf)
{
    QByteArray temp_buf = send_buf;
    //获取想要处理的有关模块信息
    QString module;
    //获取客户端传来的请求服务信息json包
    get_module_json(temp_buf,module);

    /*请求哪个模块的处理*/
    if(module == "deal_stu_info"){                              //学生信息处理模块
        Deal_Stu_Info deal_stu_info{m_tcpSocket, m_info_mysql}; //学生信息相关处理对象
        deal_stu_info.read_service_messages(send_buf);
    }

}


/**
 * @brief Stu_Main::get_module_json 解析客户端发送的请求服务的json数据包
 * @param temp_buf, module 缓冲区，应该是处理学生请求的哪个模块
 */
void Stu_Main::get_module_json(QByteArray temp_buf, QString &module)
{        /*json数据如下
        {
            sender:xxxx,
            module:xxx,
            service:xxx
            ...
        }
        */
        //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(temp_buf, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("module")) {  // 包含指定的 key
                QJsonValue value = object.value("module");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    module = value.toString();  // 将 value 转化为字符串

                }
            }
        }
    }
    qDebug()<<"【需要进行处理的module是】"<<module;

}

