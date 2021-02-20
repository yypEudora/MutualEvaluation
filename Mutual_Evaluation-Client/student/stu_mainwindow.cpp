#include "stu_mainwindow.h"
#include "ui_stu_mainwindow.h"
#include <QMessageBox>
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include "student/stu_login_instance.h"
#include "common/common.h"
#include <QDebug>
#include <QLoggingCategory>
#include <string.h>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QJsonObject>

Stu_Mainwindow::Stu_Mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stu_Mainwindow)
{
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);

    ui->setupUi(this);

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    m_stu_info = new Stu_Info;

    // 处理所有信号
    manage_signals();
    ui_set();
}

Stu_Mainwindow::~Stu_Mainwindow()
{
    delete ui;
}



void Stu_Mainwindow::show_mainwindow()
{
    //显示窗口
    this->show();

    Stu_Login_Instance *login_instance = Stu_Login_Instance::getInstance(); //获取单例
    m_user = login_instance->get_user();
    m_ip = login_instance->get_ip();
    m_port = login_instance->get_port();

    acquire_user_data(); //请求用户的数据

    //类型转换
    QByteArray q_user = m_user.toLatin1();
    char *user = q_user.data();
    ui->welcome_string->setText(QObject::tr(user));

    if(!m_completed_info){         //个人信息没有完善
        int choose;
        choose=QMessageBox::warning(this, tr("提示"),
                                     QString(tr("完善个人信息后才可进行其它操作！\n现在是否去完善个人信息？")),
                                     QMessageBox::Yes | QMessageBox::No);
      if (choose== QMessageBox::No)
       {
          emit change_user();
            //什么都不做
      }
      else if (choose== QMessageBox::Yes)
      {
        cout << "去完善个人信息...";
        this->hide();
        m_stu_info->show_mainwindow(m_password, m_name, m_sex, m_academy, m_grade, m_major, m_class, m_tell, m_qq);
      }
    }
}

void Stu_Mainwindow::manage_signals()
{
    //个人信息保存成功，修改back_to_mainwindow的值
    connect(m_stu_info, &Stu_Info::save_to_server, [=]()
    {
       m_completed_info = true;
       save_personal_info_to_server();
    });

    //个人信息界面返回到主界面
    connect(m_stu_info, &Stu_Info::back_to_mainwindow, [=]()
    {
       m_stu_info->hide();
       this->show_mainwindow();
    });



    // 切换用户
    connect(ui->change_user_btn, &QToolButton::clicked, [=]()
    {
        int choose;
        choose=QMessageBox::question(this, tr("退出登录"),
                                     QString(tr("确认退出登录?")),
                                     QMessageBox::Yes | QMessageBox::No);
      if (choose== QMessageBox::No)
       {
            //什么都不做
      }
      else if (choose== QMessageBox::Yes)
      {
        cout << "bye bye...";
        login_again(); //程序退出
      }

    });

    //个人信息
    connect(ui->stu_info_list_btn, &QToolButton::clicked, [=](){
        this->hide();
        m_stu_info->show_mainwindow(m_password, m_name, m_sex, m_academy, m_grade, m_major, m_class, m_tell, m_qq);
    });

    //隐藏按钮
    connect(ui->hide_btn,&QToolButton::clicked,[=](){
        if( windowState() != Qt::WindowMinimized ){
            setWindowState( Qt::WindowMinimized );
        }
    });
    //关闭按钮
    connect(ui->close_btn,&QToolButton::clicked,[=](){
        this->close();
    });

    //缩小放大按钮
    connect(ui->min_max_btn,&QToolButton::clicked,[=](){

        QIcon icon;
        if( m_max_or_normal ){
            icon = style()->standardIcon( QStyle::SP_TitleBarNormalButton );
            ui->min_max_btn->setIcon( icon );
            setWindowState( Qt::WindowMaximized );
        }else {
            icon = style()->standardIcon( QStyle::SP_TitleBarMaxButton );
            ui->min_max_btn->setIcon( icon );
            setWindowState( Qt::WindowNoState );
        }
        m_max_or_normal = !m_max_or_normal;
    });

}

void Stu_Mainwindow::login_again()
{
    this->hide();
    emit change_user();

}

void Stu_Mainwindow::ui_set()
{
    //设置工具栏的下拉列表
//    m_stu_info_menu = new QMenu();
//    m_change_user_menu = new QMenu();
//    m_stu_exit_menu = new QMenu();
//    QAction *stu_info = new QAction (m_stu_info_menu);
//    QAction *stu_change_user = new QAction (m_change_user_menu);
//    QAction *stu_exit = new QAction (m_stu_exit_menu);

//    stu_info->setText(QObject::tr("个人信息"));
//    stu_change_user->setText(QObject::tr("切换用户"));
//    stu_exit->setText(QObject::tr("退出登录"));

//    m_stu_info_menu->addAction(stu_info);
//    m_change_user_menu->addAction(stu_change_user);
//    m_stu_exit_menu->addAction(stu_exit);

//    ui->stu_info_list_btn->setMenu(m_stu_info_menu);
//    ui->change_user_btn->setMenu(m_change_user_menu);
//    ui->stu_exit_btn->setMenu(m_stu_exit_menu);

//    QIcon icon;
//    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton );
//    // true represents Max
//    if( !m_max_or_normal ){
//        icon = style()->standardIcon( QStyle::SP_TitleBarMaxButton );
//        ui->min_max_btn->setIcon( icon );
//    }else{
//        icon = style()->standardIcon( QStyle::SP_TitleBarNormalButton );
//        ui->min_max_btn->setIcon( icon );
//    }
}

void Stu_Mainwindow::acquire_user_data()
{
    QByteArray postData = set_user_data_json();

    this->m_tcpSocket = new QTcpSocket(this);
    this->m_tcpSocket->abort();//中止当前连接并重置套接字。与disconnectFromHost（）不同，
                                //此函数会立即关闭套接字，丢弃写入缓冲区中的任何挂起的数据。
    this->m_tcpSocket->connectToHost(m_ip,8888);
    connect(this->m_tcpSocket,SIGNAL(readyRead()),this,SLOT(read_back_messages()));
    bool suc = this->m_tcpSocket->waitForConnected();
    qDebug()<<suc;
    this->m_tcpSocket->write(postData,postData.length());//发送获取用户数据数据包
    qDebug()<<"发送数据包";
}

QByteArray Stu_Mainwindow::set_user_data_json()
{
    QMap<QString, QVariant> login;
    login.insert("sender","student");            //用户类别
    login.insert("service","acquire_user_data"); //请求的服务
    login.insert("user",m_user);                 //请求服务的用户
    /*json数据如
        {
            sender:xxxx,
            service:xxxx,
            user:xxx
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(login);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}

void Stu_Mainwindow::save_personal_info_to_server()
{
    //修改类成员变量
    m_stu_info->updated_info_return(m_name,m_sex,m_academy,m_grade,m_major,m_class,m_tell,m_qq);
    QByteArray postData = set_personal_info_json();

    this->m_tcpSocket = new QTcpSocket(this);
    this->m_tcpSocket->abort();//中止当前连接并重置套接字。与disconnectFromHost（）不同，
                                //此函数会立即关闭套接字，丢弃写入缓冲区中的任何挂起的数据。
    this->m_tcpSocket->connectToHost(m_ip,8888);
    connect(this->m_tcpSocket,SIGNAL(readyRead()),this,SLOT(read_back_messages()));
    bool suc = this->m_tcpSocket->waitForConnected();
    qDebug()<<suc;
    this->m_tcpSocket->write(postData,postData.length());//发送保存个人信息到服务器数据包
    qDebug()<<"发送保存个人信息数据包";
}

QByteArray Stu_Mainwindow::set_personal_info_json()
{
    QMap<QString, QVariant> info;
    info.insert("sender", "student");            //用户类别
    info.insert("service", "save_personal_info_to_server"); //请求的服务
    info.insert("user", m_user);                 //请求服务的用户
    info.insert("name", m_name);
    info.insert("sex", m_sex);
    info.insert("academy", m_academy);
    info.insert("grade", m_grade);
    info.insert("major", m_major);
    info.insert("class", m_class);
    info.insert("tell", m_tell);
    info.insert("qq", m_qq);
    info.insert("completed_info", m_completed_info);
    /*json数据如
        {
            sender:xxxx,
            service:xxxx,
            user:xxx
            ...
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(info);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}

void Stu_Mainwindow::read_back_messages()
{
    QByteArray alldata = this->m_tcpSocket->readAll();
    QByteArray back_buf = alldata;
    qDebug()<<"back_buf:"<<back_buf;


    //获取事件处理的返回信息
    QString service;
    get_back_json(back_buf,service, m_password, m_name, m_sex, m_academy, m_grade, m_major, m_class, m_tell, m_qq, m_course_number, m_completed_info);
    if(service == "acquire_user_data"){
        qDebug()<<m_name<<""<<m_academy<<""<<m_grade;
    }
}


void Stu_Mainwindow::get_back_json(QByteArray back_buf, QString &service, QString &pwd, QString &name, QString &sex,
                    QString &academy, QString &grade, QString &major, QString &clas,
                                   QString &tell, QString &qq, int &course_number, bool &completed_info)
{
    /*json数据如下
    {
        service:xxxx
        .....
    }
    */
    //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(back_buf, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("service")) {  // 包含指定的 key
                QJsonValue value = object.value("service");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    service = value.toString();  // 将 value 转化为字符串
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

            if (object.contains("clas")) {
                QJsonValue value = object.value("clas");
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
            if (object.contains("course_number")) {
                QJsonValue value = object.value("course_number");
                if (value.isString()) {
                    course_number = value.toInt();
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
    qDebug()<<"返回的信息包："<<service<<" "<<pwd<<" "<<name;
}

void Stu_Mainwindow::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Stu_Mainwindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Stu_Mainwindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}

