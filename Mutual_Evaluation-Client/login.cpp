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
#include "student/stu_login_instance.h"


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
}


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

    //登录信息写入配置文件
    //m_cm.writeLoginInfo(user,pwd,ui->rem_pwd->isChecked());
    //qDebug()<<"write:::"<<user.toStdString().data()<<pwd.toStdString().data();

//    //设置登录信息json包，MD5加密
//    QByteArray array = setLoginJson(user,m_cm.getStrMd5(pwd));


    // 将用户输入的注册信息 -> json对象
        QByteArray postData = set_login_json(current_user, user, m_cm.getStrMd5(pwd));

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
        QByteArray postData = set_register_json(current_user,user, m_cm.getStrMd5(firstPwd));

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
    // 跳转到登陆界面
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    //m_cm.writeWebInfo(ip,port);
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
        this->hide();
        QString current_user = get_login_current_user();

         //判断登录成功后显示哪种类别的主界面
        if(current_user == "student"){
            cout<<"学生正在登录...";
            Stu_Login_Instance *login_instance = Stu_Login_Instance::getInstance(); //获取单例
            login_instance->set_login_info(ui->name_tx->text(), ui->ip_tx->text(), ui->port_tx->text());
            m_stu_mainwindow->show_mainwindow(); //显示学生主界面
        }
        else if(current_user == "teacher"){
            cout<<"教师正在登录...";
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


//获取登录返回码
QStringList Login::get_login_status(QByteArray json)
{
    QJsonParseError error;
    QStringList list;

    // 将来源数据json转化为JsonDocument
    // 由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error == QJsonParseError::NoError)
    {
        if (doc.isNull() || doc.isEmpty())
        {
            qDebug() << "doc.isNull() || doc.isEmpty()";
            return list;
        }

        if( doc.isObject() )
        {
            //取得最外层这个大对象
            QJsonObject obj = doc.object();
            qDebug() << "服务器返回的数据";
            //状态码
            list.append( obj.value( "code" ).toString() );
            //登陆token
            list.append( obj.value( "token" ).toString() );
        }
    }
    else
    {
        qDebug() << "err = " << error.errorString().toStdString().data();
    }

    return list;

}


