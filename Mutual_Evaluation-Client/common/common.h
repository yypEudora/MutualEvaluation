/*
 * common.h
 * 1.简述：项目所需要的共同功能
 * 2.主要功能：文本数据输入格式要求，登录时所需要的配置信息，信息加密
 * 3.被引用：login.h
 */


#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <QWidget>
#include <QString>
#include <QListWidgetItem>
#include <QtNetwork/QNetworkAccessManager>

#define cout qDebug() << "[ " << __FILE__ << ":"  << __LINE__ << " ] "


#define SOCKET_CONF        "conf/socket.json"     // 配置文件

// 正则表达式
#define USER_REG        "^[a-zA-Z\\d_@#-\*]\{3,16\}$"
#define PASSWD_REG      "^[a-zA-Z\\d_@#-\*]\{6,18\}$"
#define Name            "^[a-zA-Z\\d_@#-\*]\{2,16\}$"
#define QQ              "^[a-zA-Z\\d_@#-\*]\{7,15\}$"
#define Tell            "1\\d\{10\}"
#define EMAIL_REG       "^[a-zA-Z\\d\._-]\+@[a-zA-Z\\d_\.-]\+(\.[a-zA-Z0-9_-]\+)+$"
#define IP_REG          "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"
#define PORT_REG        "^[1-9]$|(^[1-9][0-9]$)|(^[1-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9]$)|(^[1-6][0-5][0-5][0-3][0-5]$)"

class Common : public QObject
{
    Q_OBJECT

public:
    Common(QObject* parent = nullptr);
    ~Common();

    // 窗口在屏幕中央显示
    void move_to_center(QWidget *tmp);

    // 从配置文件中得到相对应的参数
    QString get_cfg_value(QString title, QString key, QString path);

    // 登录信息，写入配置文件
    void write_login_info(QString user, QString pwd, bool is_remeber, QString path);

    // 服务器信息，写入配置文件
    void write_socket_info(QString ip, QString port, QString path=SOCKET_CONF);

    // 获取某个文件的md5码
    QString get_file_md5(QString file_path);

    // 将某个字符串加密成md5码
    QString get_str_md5(QString str = "");

    // 产生分隔线
    QString get_boundary();

    //信息加密
    QByteArray info_encrypt(QString str);

    //信息解密
    QString info_decode(QByteArray by);
};

#endif // COMMON_H

