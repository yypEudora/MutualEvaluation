#include "login.h"
#include "ui_login.h"
#include <QMouseEvent>
#include <QString>
#include <QToolButton>
#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>
#include <QLoggingCategory>
#include <QDebug>
#include <QFileInfo>
#include "student/stu_login_instance.h"
#include "teacher/tc_login_instance.h"
#include "common/common.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);


    m_parent = parent;
    // 此处无需指定父窗口
    m_stu_mainwindow = new Stu_Mainwindow;
    m_tc_mainwindow = new Tc_Mainwindow;
    m_zj_mainwindow = new Zj_mainwindow;

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    ui->stackedWidget->setCurrentWidget(ui->login_page);
    setAutoFillBackground(true);

    //居中显示
    m_cm.move_to_center(this);

    //初始设置选择身份为学生
    ui->student_btn->setChecked(true);

    //密码
    ui->pwd_tx->setEchoMode(QLineEdit::Password);

    //数据格式管理
    manage_format();
    //信号槽管理
    manage_signals();

    //设置软件图标
    this->setWindowIconText("ME");

    //读取socket_server信息
    read_socket();
}

Login::~Login()
{
    delete ui;
}


/**
 * @brief Login::manage_format 设置需要输入的数据的正确格式
 */
void Login::manage_format(){
    // 数据的格式提示
    ui->stu_id_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->tc_id_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->zj_id_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->name_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->stu_pwd_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->tc_pwd_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->zj_pwd_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->pwd_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");

}


/**
 * @brief Login::manage_signals 设置信号槽连接
 */
void Login::manage_signals()
{
    //登录界面点击设置，进入服务器IP和端口界面
    connect(ui->set_btn,&QToolButton::clicked,[=](){
        ui->ip_tx->setFocus();
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });
    //登录界面点击注册，进入注册界面
    connect(ui->register_btn,&QToolButton::clicked,[=](){
        ui->student_box->setChecked(true);
        ui->stackedWidget->setCurrentWidget(ui->register_page);
        ui->idStackedWidget->setCurrentWidget(ui->student_page);
    });
    //登录界面点击退出，退出程序
    connect(ui->exit_btn,&QToolButton::clicked,[=](){
        exit(1);
    });
    //注册界面点击返回，返回登录界面
    connect(ui->ret_btn,&QToolButton::clicked,[=](){
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    });
    //注册界面点击设置，进入设置界面
    connect(ui->set3_btn,&QToolButton::clicked,[=](){
        ui->ip_tx->setFocus();
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });
    //设置界面点击返回，返回登录界面
    connect(ui->return_btn,&QToolButton::clicked,[=](){
        ui->stackedWidget->setCurrentWidget(ui->login_page);
    });
    //注册界面点击学生，显示学生注册信息
    connect(ui->student_box,&QToolButton::clicked,[=](){
        ui->idStackedWidget->setCurrentWidget(ui->student_page);
    });
    //注册界面点击教师，显示教师注册信息
    connect(ui->teacher_box,&QToolButton::clicked,[=](){
        ui->idStackedWidget->setCurrentWidget(ui->teacher_page);
    });
    //注册界面点击助教，显示助教注册信息
    connect(ui->zhujiao_box,&QToolButton::clicked,[=](){
        ui->idStackedWidget->setCurrentWidget(ui->zhujiao_page);
    });

    // 学生主界面切换用户 - 重新登录
    connect(m_stu_mainwindow, &Stu_Mainwindow::change_user, [=]()
    {
        m_stu_mainwindow->hide();
        this->show();
    });
    // 教师主界面切换用户 - 重新登录
    connect(m_tc_mainwindow, &Tc_Mainwindow::change_user, [=]()
    {
        m_tc_mainwindow->hide();
        this->show();
    });
    // 助教主界面切换用户 - 重新登录
    connect(m_zj_mainwindow, &Zj_mainwindow::change_user, [=]()
    {
        m_zj_mainwindow->hide();
        this->show();
    });


    //为学生修改密码界面输入的原始密码进行加密
    connect(m_stu_mainwindow, &Stu_Mainwindow::check_before_pwd, [=]()
    {
        Stu_Pwd* stu_pwd = m_stu_mainwindow->return_stu_pwd_window();
        QString pwd;
        stu_pwd->return_input_pwd(pwd);
        stu_pwd->pwd_str_md5 = m_cm.get_str_md5(pwd);//加密
    });

    //为学生修改密码界面输入的新密码进行加密
    connect(m_stu_mainwindow, &Stu_Mainwindow::save_updated_pwd_to_server, [=]()
    {
        Stu_Pwd* stu_pwd = m_stu_mainwindow->return_stu_pwd_window();
        QString new_pwd;
        stu_pwd->return_input_new_pwd(new_pwd);
        stu_pwd->new_pwd_str_md5 = m_cm.get_str_md5(new_pwd);//加密
    });

    //为教师修改密码界面输入的原始密码进行加密
    connect(m_tc_mainwindow, &Tc_Mainwindow::check_before_pwd, [=]()
    {
        Tc_Pwd* tc_pwd = m_tc_mainwindow->return_tc_pwd_window();
        QString pwd;
        tc_pwd->return_input_pwd(pwd);
        tc_pwd->pwd_str_md5 = m_cm.get_str_md5(pwd);//加密
    });

    //为教师修改密码界面输入的新密码进行加密
    connect(m_tc_mainwindow, &Tc_Mainwindow::save_updated_pwd_to_server, [=]()
    {
        Tc_Pwd* tc_pwd = m_tc_mainwindow->return_tc_pwd_window();
        QString new_pwd;
        tc_pwd->return_input_new_pwd(new_pwd);
        tc_pwd->new_pwd_str_md5 = m_cm.get_str_md5(new_pwd);//加密
    });






    //记住密码的用户名输入后，自动访问json获取密码
    connect(ui->name_tx,&QLineEdit::editingFinished,[=](){
        ui->name_tx->setText(ui->name_tx->text());
        QString path = "conf/" + this->ui->name_tx->text() + ".json";
        QFileInfo file_info(path);
        if(file_info.isFile())
            read_login_info();
        else{
            qDebug()<<"该文件不存在";
            ui->pwd_tx->clear();
            ui->remp_btn->setChecked(false);
        }
    });
}

/**
 * @brief Login::mousePressEvent  Login::mousePressEvent Login::mouseMoveEvent
 * 窗口移动
 */
void Login::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}
void Login::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}
void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}


/**
 * @brief Login::get_login_current_user 得到当前请求登录的用户类别
 */
QString Login::get_login_current_user()
{
    QString current_user;
    if(ui->student_btn->isChecked())
        current_user = "student";
    else if(ui->teacher_btn->isChecked())
        current_user = "teacher";
    else if(ui->zhujiao_btn->isChecked())
        current_user = "zhujiao";
    return current_user;
}


/**
 * @brief Login::get_regist_current_user 得到当前请求注册的用户类别
 */
QString Login::get_regist_current_user()
{
    QString current_user;
    if(ui->student_box->isChecked())
        current_user = "student";
    else if(ui->teacher_box->isChecked())
        current_user = "teacher";
    else if(ui->zhujiao_box->isChecked())
        current_user = "zhujiao";
    return current_user;
}


/**
 * @brief Login::set_login_json 登陆用户需要使用的json数据包，发送给服务端
 * @param current_user, user, pwd  当前用户类别，用户，密码
 * @return 设置好的json数据包
 */
QByteArray Login::set_login_json(QString current_user, QString user, QString pwd)
{
    QMap<QString, QVariant> login;
    login.insert("sender","login");

    if(current_user == "student")
        login.insert("current_user", "student");
    else if(current_user == "teacher")
        login.insert("current_user", "teacher");
    else if(current_user == "zhujiao")
        login.insert("current_user", "zhujiao");

    login.insert("user",user);
    login.insert("pwd",pwd);
    /*json数据如
        {
            user:xxxx,
            pwd:xxx
        }
    */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(login);
    if(jsonDocument.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDocument.toJson();
}


/**
 * @brief Login::set_register_json 注册用户需要使用的json数据包，发送给服务端
 * @param current_user, user, pwd  当前用户类别，用户，密码
 * @return 设置好的json数据包
 */
QByteArray Login::set_register_json(QString current_user, QString user, QString pwd)
{
    QMap<QString, QVariant> reg;
    reg.insert("sender","register");

    if(current_user == "student")
        reg.insert("current_user", "student");
    else if(current_user == "teacher")
        reg.insert("current_user", "teacher");
    else if(current_user == "zhujiao")
        reg.insert("current_user", "zhujiao");

    reg.insert("user", user);
    reg.insert("pwd", pwd);
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(reg);
    if(jsonDoc.isNull()){
        cout << " jsonDocument.isNull() ";
        return "";
    }
    return jsonDoc.toJson();
}


/**
 * @brief Login::on_login_btn_clicked 确认登录
 */
void Login::on_login_btn_clicked()
{

    QString current_user = get_login_current_user();
    cout<<current_user<<"请求登录啦";
    // 从控件中取出用户输入的数据
    QString user = ui->name_tx->text();
    QString pwd = ui->pwd_tx->text();

    QString ip = ui->ip_tx->text();
    QString port = ui->port_tx->text();


    QString path = "conf/" + this->ui->name_tx->text() + ".json";

    //登录信息写入配置文件
    m_cm.write_login_info(user,pwd,ui->remp_btn->isChecked(),path);
    qDebug()<<"write:::"<<user.toStdString().data()<<pwd.toStdString().data();

    QByteArray postData = set_login_json(current_user, user, m_cm.get_str_md5(pwd));

    this->tcpSocket = new QTcpSocket(this);
    this->tcpSocket->abort();//中止当前连接并重置套接字。与disconnectFromHost（）不同，
                                //此函数会立即关闭套接字，丢弃写入缓冲区中的任何挂起的数据。
    this->tcpSocket->connectToHost(ip,8888);
    connect(this->tcpSocket,SIGNAL(readyRead()),this,SLOT(read_back_message()));
    bool suc = this->tcpSocket->waitForConnected();
    qDebug()<<suc;
    this->tcpSocket->write(postData,postData.length());//发送登录数据包
    qDebug()<<"发送数据包";

}


/**
 * @brief Login::on_reg_btn_clicked 确认注册
 */
void Login::on_reg_btn_clicked()
{
    QString current_user = get_regist_current_user();

    QString user;
    QString firstPwd;
    QString surePwd;

     // 从控件中取出用户输入的数据
    if(current_user == "student"){
        user = ui->stu_id_tx->text();
        firstPwd = ui->stu_pwd_tx->text();
        surePwd = ui->stu_pwd_again_tx->text();
    }
    else if(current_user == "teacher"){
        user = ui->tc_id_tx->text();
        firstPwd = ui->tc_pwd_tx->text();
        surePwd = ui->tc_pwd_again_tx->text();
    }
    else if(current_user == "zhujiao"){
        user = ui->zj_id_tx->text();
        firstPwd = ui->zj_pwd_tx->text();
        surePwd = ui->zj_pwd_again_tx->text();
    }


    QString ip = ui->ip_tx->text();
    QString port = ui->port_tx->text();

    //数据校验
    QRegExp regexp(USER_REG);
    if(!regexp.exactMatch(user)){
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        if(current_user == "student"){
            ui->stu_id_tx->clear();
            ui->stu_id_tx->setFocus();
        }
        else if(current_user == "teacher"){
            ui->tc_id_tx->clear();
            ui->tc_id_tx->setFocus();
        }
        else if(current_user == "zhujiao"){
            ui->zj_id_tx->clear();
            ui->zj_id_tx->clear();
        }
        return;
    }
    regexp.setPattern(PASSWD_REG);
    if(!regexp.exactMatch(firstPwd))
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        if(current_user == "student"){
            ui->stu_pwd_tx->clear();
            ui->stu_pwd_tx->setFocus();
        }
        else if(current_user == "teacher"){
            ui->tc_pwd_tx->clear();
            ui->tc_pwd_tx->setFocus();
        }
        else if(current_user == "zhujiao"){
            ui->zj_pwd_tx->clear();
            ui->zj_pwd_tx->clear();
        }
        return;
    }
    if(surePwd != firstPwd)
    {
        QMessageBox::warning(this, "警告", "两次输入的密码不匹配, 请重新输入");
        if(current_user == "student"){
            ui->stu_pwd_again_tx->clear();
            ui->stu_pwd_again_tx->setFocus();
        }
        else if(current_user == "teacher"){
            ui->tc_pwd_again_tx->clear();
            ui->tc_pwd_again_tx->setFocus();
        }
        else if(current_user == "zhujiao"){
            ui->zj_pwd_again_tx->clear();
            ui->zj_pwd_again_tx->clear();
        }
        return;
    }


    // 将用户输入的注册信息 -> json对象
    QByteArray postData = set_register_json(current_user,user, m_cm.get_str_md5(firstPwd));

    this->tcpSocket = new QTcpSocket(this);
    this->tcpSocket->abort();//中止当前连接并重置套接字。与disconnectFromHost（）不同，
                                //此函数会立即关闭套接字，丢弃写入缓冲区中的任何挂起的数据。
    this->tcpSocket->connectToHost(ip,8888);
    connect(this->tcpSocket,SIGNAL(readyRead()),this,SLOT(read_back_message()));
    bool suc = this->tcpSocket->waitForConnected();
    qDebug()<<suc;
    this->tcpSocket->write(postData,postData.length());//发送登录数据包
    qDebug()<<"发送数据包: "<<postData;
}


/**
 * @brief Login::on_set2_btn_clicked 确认设置
 */

void Login::on_set2_btn_clicked()
{
    QString path = SOCKET_CONF;
    QString ip = ui->ip_tx->text();
    QString port = ui->port_tx->text();
    QRegExp regexp(IP_REG);
    if(!regexp.exactMatch(ip))
    {
        QMessageBox::warning(this, "警告", "您输入的IP格式不正确, 请重新输入!");
        return;
    }
    // 端口号
    regexp.setPattern(PORT_REG);
    if(!regexp.exactMatch(port))
    {
        QMessageBox::warning(this, "警告", "您输入的端口格式不正确, 请重新输入!");
        return;
    }
    m_cm.write_socket_info(ip,port,path);
    // 跳转到登陆界面
    ui->stackedWidget->setCurrentWidget(ui->login_page);

}


/**
 * @brief Login::read_back_message 读取服务端登录/注册结果反馈信息
 */
void Login::read_back_message()
{
    QByteArray alldata = this->tcpSocket->readAll();
    QByteArray back_buf = alldata;
    qDebug()<<"back_buf:"<<back_buf;


    //获取事件处理的返回信息
    QString sender;
    QString msg;
    get_back_json(back_buf,sender,msg);

    if(sender == "login"){
        read_login_back_messages(msg);
    }
    else if(sender == "register"){
        read_regist_back_messages(msg);
    }
}


/**
 * @brief Login::read_login_back_messages 读取服务端登录结果的反馈信息
 * @param msg 登录结果的反馈信息
 */
void Login::read_login_back_messages(QString msg)
{
    qDebug()<<"msg2是："<<msg;
    if(msg == "true") {
        // 从控件中取出用户输入的数据
        QString user = ui->name_tx->text();
        QString pwd = ui->pwd_tx->text();

        QString ip = ui->ip_tx->text();
        QString port = ui->port_tx->text();


        QString path = "conf/" + this->ui->name_tx->text() + ".json";

        //登录信息写入配置文件
        m_cm.write_login_info(user,pwd,ui->remp_btn->isChecked(),path);
        qDebug()<<"write:::"<<user.toStdString().data()<<pwd.toStdString().data();

        this->hide();
        QString current_user = get_login_current_user();

         //判断登录成功后显示哪种类别的主界面
        if(current_user == "student"){
            cout<<"学生正在登录...";
            //获取单例
            Stu_Login_Instance *stu_login_instance = Stu_Login_Instance::getInstance();
            stu_login_instance->set_login_info(ui->name_tx->text(), ui->ip_tx->text(), ui->port_tx->text());

            m_stu_mainwindow->show_mainwindow(); //显示学生主界面
        }
        else if(current_user == "teacher"){
            cout<<"教师正在登录...";
            Tc_Login_Instance *tc_login_instance = Tc_Login_Instance::getInstance();
            tc_login_instance->set_login_info(ui->name_tx->text(), ui->ip_tx->text(), ui->port_tx->text());

            m_tc_mainwindow->show_mainwindow(); //显示教师主界面
        }
        else if(current_user == "zhujiao"){
            cout<<"助教正在登录...";
            m_zj_mainwindow->show_mainwindow(); //显示助教主界面
        }

    }
    else if(msg == "NotFound"){
        QMessageBox::information(NULL, "信息提示",
                                 "登录失败！该用户不存在.",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
    } else if(msg == "PasswordWrong"){
        QMessageBox::information(NULL, "信息提示",
                                 "登录失败！密码错误.",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
    }
}


/**
 * @brief Login::read_regist_back_messages 读取服务端注册结果的反馈信息
 * @param msg 注册结果的反馈信息
 */
void Login::read_regist_back_messages(QString msg)
{

    qDebug()<<"msg2是："<<msg;
    if(msg == "true") {     //注册成功
        QMessageBox::information(NULL, "信息提示",
                                 "注册成功！",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
        //跳转到登录界面
        ui->stackedWidget->setCurrentWidget(ui->login_page);

    } else if(msg == "Existed"){
        QMessageBox::information(NULL, "信息提示",
                                 "注册失败！该用户已存在.",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
    }
}


/**
 * @brief Login::get_back_json 解析注册/登录反馈信息json包
 * @param back_buf, sender, msg, user_info_has_completed 整个反馈信息的缓冲区，服务端的发送者，服务成功与否的反馈信息
 *                                                       用户是否完善了个人信息
 */
void Login::get_back_json(QByteArray &back_buf, QString &sender,QString &msg)
{
        /*json数据如下
        {
            sender:xxxx
            msg:xxxx
        }
        */
        //解析json包
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(back_buf, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("sender")) {  // 包含指定的 key
                QJsonValue value = object.value("sender");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    sender = value.toString();  // 将 value 转化为字符串
                }
            }
            if (object.contains("msg")) {
                QJsonValue value = object.value("msg");
                if (value.isString()) {
                    msg = value.toString();
                }
            }

        }
    }
    qDebug()<<"返回的信息包："<<sender<<" "<<msg<<" ";
}

/**
 * @brief Login::read_login_info 曾经登录选择记住密码的用户输入用户名后，密码自动弹出
 *
 */
void Login::read_login_info()
{
    QString path = "conf/" + this->ui->name_tx->text() + ".json";
    QFile file(path);
    if( false == file.open(QIODevice::ReadWrite) )
    {
        cout << "file open err";
        return;
    }

    QString user_name = ui->name_tx->text();
    QString user_pwd = m_cm.get_cfg_value("login","pwd",path);
    QString remeber = m_cm.get_cfg_value("login", "remember",path);

    qDebug() << "密码" << user_pwd;

    if(remeber == "yes")//记住密码
    {
        QByteArray pwd = user_pwd.toLatin1();
        QString password = m_cm.info_decode(pwd);
        ui->pwd_tx->setText(password);
        ui->remp_btn->setChecked(true);

    }
    else //没有记住密码
    {
        ui->pwd_tx->clear();
        ui->remp_btn->setChecked(false);
    }

    ui->name_tx->setText(user_name);

}

/**
 * @brief Login::read_read_socket 服务器信息自动导入
 *
 */
void Login::read_socket()
{
    QString path = SOCKET_CONF;
    QString server_ip = m_cm.get_cfg_value("socket_server","ip",path);
    QString server_port = m_cm.get_cfg_value("socket_server","port",path);
    ui->ip_tx->setText(server_ip);
    ui->port_tx->setText(server_port);
}
