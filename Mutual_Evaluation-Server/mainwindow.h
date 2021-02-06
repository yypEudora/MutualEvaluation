#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QSqlDatabase>
#include "mysql.h"
#include "dealRegist.h"
#include "dealLogin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString get_local_ip_address(); //获取本机ip地址
    void init_server_connect();   //连接/关闭服务器
    void get_sender_info(QByteArray send_buf,QString &sender);//确认请求处理的事件

private slots:
    void start_tcp_server();  //开启服务器
    void close_tcp_server();  //关闭服务器

    void new_connect();
    void read_messages();
    void read_login_messages(QByteArray send_buf);
    void read_regist_messages(QByteArray send_buf);


private:
    Ui::MainWindow *ui;
    QTcpServer *m_tcpServer;//服务器套接字
    QTcpSocket *m_tcpSocket;//连接进服务器的套接字

    int m_port = 8888;  //服务器端口号

    bool m_server_status;   //当前服务器状态，为0表示关闭，为1时表示开启

    MYSQL m_mysql;
//    DealRegist *m_deal_regist;  //处理注册服务
//    DealLogin *m_deal_login;    //处理登录服务

};
#endif // MAINWINDOW_H
