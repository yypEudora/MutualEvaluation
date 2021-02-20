#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QtSql>
#include "dealRegist.h"
#include "dealLogin.h"
#include "student/stu_main.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_mysql.connect_mysql(); //连接数据库
    m_mysql.create_database();     //打开数据库，不存在时则创建
    m_mysql.init_database();       //初始化数据库
    m_server_status = 0;    //初始服务器状态为关闭
    this->m_tcpServer = nullptr;
    this->m_tcpSocket = nullptr;
    this->ui->ip_tx->setText(this->get_local_ip_address()); //获取本机ip
    init_server_connect();    //开启/关闭服务器
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::init_server_connect 连接开启/关闭服务器的槽函数
 * @caption 初始化QObject::connect()函数
 */
void MainWindow::init_server_connect()
{
    connect(this->ui->open_server_btn,SIGNAL(clicked()),this,SLOT(start_tcp_server()));
    connect(this->ui->close_server_btn,SIGNAL(clicked()),this,SLOT(close_tcp_server()));
}


/**
 * @brief MainWindow::start_tcp_server 开启服务器
 */
void MainWindow::start_tcp_server()
{
    if(m_server_status == 1){
        qDebug()<<"服务器已经为开启状态";
    }
    else{
            this->m_tcpServer = new QTcpServer(this);
            int port = this->ui->port_tx->text().toInt();
            m_port = port;//端口
            if(this->m_tcpServer->listen(QHostAddress::Any,m_port))
                connect(this->m_tcpServer,SIGNAL(newConnection()),this,SLOT(new_connect()));  //有新的连接
            qDebug()<<"服务器开启成功，"<<"端口号："<<m_port;
            this->ui->server_status_tx->setText("Running...");
            m_server_status = 1;
    }
}

/**
 * @brief MainWindow::close_tcp_server 关闭服务器
 */
void MainWindow::close_tcp_server()
{
    if(m_server_status == 0) {
        qDebug()<<"服务器已经为关闭状态";
    }
    else{
        if(this->m_tcpSocket != nullptr) {
            this->m_tcpSocket->close();
        }
        if(this->m_tcpServer != nullptr) {
            this->m_tcpServer->close();
        }

        this->ui->server_status_tx->setText("Stop.");
        qDebug()<<"服务器成功关闭";
        m_server_status = 0;
    }
}


/**
 * @brief MainWindow::new_connect 新的Socket连接
 */
void MainWindow::new_connect(){
    this->m_tcpSocket = this->m_tcpServer->nextPendingConnection();//获取连接进来的socket
    connect(this->m_tcpSocket,SIGNAL(readyRead()),this,SLOT(read_messages()));
}


/**
 * @brief MainWindow::read_messages 读入客户端发送的消息，再判断该进行什么服务
 */
void MainWindow::read_messages(){
    QByteArray allData = this->m_tcpSocket->readAll();
    QByteArray send_buf = allData;


    qDebug()<<"send_buf:"<<send_buf;


    QString sender;
    QString current_user;
    QJsonDocument doucment;
    get_sender_info(send_buf,sender, current_user);//获取发送者信息

    qDebug()<<"sender是："<<sender;

    //判断需要处理的事件
    if(sender=="login"){
        read_login_messages(send_buf, current_user);
        qDebug()<<current_user<<"处理登录...";
    }
    else if(sender=="register"){
        read_regist_messages(send_buf, current_user);
        qDebug()<<current_user<<"处理注册...";
    }
    else if(sender == "student"){
        read_student_messages(send_buf);
        qDebug() <<"处理学生请求的服务";

    }
}


/**
 * @brief MainWindow::read_login_messages 此时已经知道进行登录服务，调用登录服务的操作
 * @param send_buf，current_user 客户端发送过来的数据，当前哪种用户请求登录
 */
void MainWindow::read_login_messages(QByteArray send_buf, QString current_user)
{
    DealLogin deal_login{m_tcpSocket, m_mysql};
    deal_login.read_login_messages(send_buf, current_user);
}


/**
 * @brief MainWindow::read_regist_messages 此时已经知道进行注册服务，调用注册服务的操作
 * @param send_buf，current_user 客户端发送过来的数据，当前哪种用户请求注册
 */
void MainWindow::read_regist_messages(QByteArray send_buf, QString current_user){
    DealRegist deal_regist{m_tcpSocket, m_mysql};
    deal_regist.read_regist_messages(send_buf, current_user);
}

//响应学生用户类别请求的服务
void MainWindow::read_student_messages(QByteArray send_buf){
    Stu_Main stu_main{m_tcpSocket, m_mysql};
    stu_main.read_service_messages(send_buf);
}



/**
 * @brief MainWindow::get_sender_info 解析客户端发送过来的json包
 * @param send_buf, sender, current_user 用以保存客户端发送数据的缓冲区，发送者(请求什么服务), 当前用户(学生/教师/助教)
 */
void MainWindow::get_sender_info(QByteArray send_buf,QString &sender, QString &current_user)
{
        /*json数据如下
        {
            sender:xxxx;
            ....
        }
        */
        //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(send_buf, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("sender")) {  // 包含指定的 key
                QJsonValue value = object.value("sender");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    sender = value.toString();  // 将 value 转化为字符串
                }
            }
            if (object.contains("current_user")) {  // 包含指定的 key
                QJsonValue value = object.value("current_user");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    current_user = value.toString();  // 将 value 转化为字符串
                }
            }
        }
    }
}



/**
 * @brief MainWindow::get_local_ip_address QT获取本机IP地址
 * @return 本机ip地址
 */
QString MainWindow::get_local_ip_address()
{
    QString vAddress;
#ifdef _WIN32
    QHostInfo vHostInfo = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> vAddressList = vHostInfo.addresses();
#else
    QList<QHostAddress> vAddressList = QNetworkInterface::allAddresses();
#endif
    for(int i = 0; i < vAddressList.size(); i++) {
        if(!vAddressList.at(i).isNull() &&
                vAddressList.at(i) != QHostAddress::LocalHost &&
                vAddressList.at(i).protocol() ==  QAbstractSocket::IPv4Protocol)
        {
            vAddress = vAddressList.at(i).toString();
            break;
        }
    }
    return vAddress;
}


