#include "stu_info.h"
#include "ui_stu_info.h"

#include <QMouseEvent>
#include <QToolButton>
#include <QStyle>
#include <common/common.h>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include "stu_login_instance.h"

Stu_Info::Stu_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stu_Info)
{
    ui->setupUi(this);

    //去掉创建的边框
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    // 数据的格式提示
    ui->stu_name_tx->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 1~16");
    ui->stu_tell_tx->setToolTip("合法字符:[0-9],字符个数: 13");
    ui->stu_qq_tx->setToolTip("合法字符:[0-9],字符个数: 7~15");

    manage_signals();

}

Stu_Info::~Stu_Info()
{
    delete ui;
}

/**
 * @brief Stu_Info::show_mainwindow 显示学生修改个人信息界面
 * @param pwd, name, sex...  当前用户的用户数据
 */
void Stu_Info::show_mainwindow(QString pwd, QString name, QString sex, QString academy, QString grade,
                               QString major, QString clas,QString tell, QString qq, bool completed_info)
{
    this->show();

    Stu_Login_Instance *login_instance = Stu_Login_Instance::getInstance(); //获取单例
    m_user = login_instance->get_user();
    m_ip = login_instance->get_ip();
    m_port = login_instance->get_port();


    ui->stu_name_tx->setText(name);
    ui->stu_sex_tx->setText(sex);
    ui->stu_academy_tx->setText(academy);
    ui->stu_grade_tx->setText(grade);
    ui->stu_class_tx->setText(clas);
    ui->stu_major_tx->setText(major);
    ui->stu_tell_tx->setText(tell);
    ui->stu_qq_tx->setText(qq);


    m_password = pwd;
    m_name = name;
    m_sex = sex;
    m_academy = academy;
    m_grade = grade;
    m_major = major;
    m_class = clas;
    m_tell = tell;
    m_qq = qq;
    m_completed_info = completed_info;

    //如果已完善个人信息则隐藏注意标语
    if(completed_info)
       this->ui->stu_warning_label->hide();
}


/**
 * @brief Stu_Info::manage_signals 管理信号槽连接
 */
void Stu_Info::manage_signals()
{
    //缩小按钮
    connect(ui->hide_btn,&QToolButton::clicked,[=](){
        if( windowState() != Qt::WindowMinimized ){
            setWindowState( Qt::WindowMinimized );
        }
    });
    //关闭按钮
    connect(ui->close_btn,&QToolButton::clicked,[=](){
        this->close();
    });

    //保存按钮
    connect(ui->save_btn, &QToolButton::clicked,[=](){
        //判断个人信息的输入格式是否合法
        if(check_info_valid()){      //格式合法
            //判断个人信息是否变化
            if(!check_info_updated()){  //没有变化
                QMessageBox::information(this, tr("提示"),
                                         QString(tr("并没有任何修改！")),
                                         QMessageBox::Yes);
            }
            else{
                int choose;
                choose=QMessageBox::warning(this, tr("提示"),
                                             QString(tr("是否保存修改？")),
                                             QMessageBox::Yes | QMessageBox::No);
                if (choose== QMessageBox::Yes)
                {
                    save_changed_info();
                }
            }
        }
    });

    //返回按钮
    connect(ui->return_btn,&QToolButton::clicked,[=](){
        if(check_info_updated()){
            int choose;
            choose=QMessageBox::warning(this, tr("提示"),
                                         QString(tr("个人信息已修改，是否保存？")),
                                         QMessageBox::Yes | QMessageBox::No);
            if (choose== QMessageBox::Yes)  //保存修改
            {
                //判断个人信息的输入是否合法
                if(check_info_valid()){
                    save_changed_info();
                }
             }
            else                            //不保存修改，直接返回
                back_to_stu_mainwindow();

        }
        else
            back_to_stu_mainwindow();
    });
}


/**
 * @brief Stu_Info::back_to_stu_mainwindow 返回到学生主界面
 */
void Stu_Info::back_to_stu_mainwindow()
{
    this->hide();
    emit back_to_mainwindow(); //向主界面发送返回主界面的信号
}


void Stu_Info::set_ui()
{
//    //缩小按钮
//    QIcon icon;
//    icon = style()->standardIcon( QStyle::SP_TitleBarMinButton);
//    ui->hide_btn->setIcon(icon);
//    //关闭按钮
//    icon = style()->standardIcon(QStyle::SP_TitleBarCloseButton);
//    ui->close_btn->setIcon(icon);
}


/**
 * @brief Stu_Info::check_info_updated 检查个人信息是否被修改
 */
bool Stu_Info::check_info_updated()
{

//    if(ui->stu_name_tx->isModified() || ui->stu_sex_tx->isModified() || ui->stu_academy_tx->isModified() || ui->stu_grade_tx->isModified()
//        || ui->stu_class_tx->isModified() || ui->stu_tell_tx->isModified() || ui->stu_qq_tx->isModified())
//        return true;
//    return false; 这种方法不方便在返回的时候检查个人信息是否被修改

    if(m_name != ui->stu_name_tx->text() || m_sex != ui->stu_sex_tx->text() || m_academy != ui->stu_academy_tx->text()
            || m_grade != ui->stu_grade_tx->text() || m_major != ui->stu_major_tx->text() || m_class != ui->stu_class_tx->text() || m_tell != ui->stu_tell_tx->text()
            || m_qq != ui->stu_qq_tx->text())
        return true;

    return false;
}


/**
 * @brief Stu_Info::check_info_valid 检查个人信息的合法性
 */
//数据校验有bug，后面完善
bool Stu_Info::check_info_valid()
{
    QString name = m_name;
    QString tell = m_tell;
    QString qq = m_qq;

    if(ui->stu_name_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "用户名不能为空");
        ui->stu_name_tx->setFocus();
        return false;
    }
    if(ui->stu_tell_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "电话不能为空");
        ui->stu_tell_tx->setFocus();
        return false;
    }
    if(ui->stu_qq_tx->text()=="")
    {
        QMessageBox::warning(this, "警告", "qq不能为空");
        ui->stu_qq_tx->setFocus();
        return false;
    }
    return true;


    // 数据校验
    QRegExp infoexp(Name);
//    if(!regexp.exactMatch(name))
//    {
//        QMessageBox::warning(this, "警告", "用户名格式不正确");
//        ui->stu_name_tx->clear();
//        ui->stu_name_tx->setFocus();
//        return false;
//    }
//    infoexp.setPattern(Tell);
//    if(!regexp.exactMatch(tell))
//    {
//        QMessageBox::warning(this, "警告", "电话格式不正确");
//        ui->stu_tell_tx->clear();
//        ui->stu_tell_tx->setFocus();
//        return false;
//    }
//    infoexp.setPattern(QQ);
//    if(!regexp.exactMatch(qq))
//    {
//        QMessageBox::warning(this, "警告", "qq格式不正确");
//        ui->stu_qq_tx->clear();
//        ui->stu_qq_tx->setFocus();
//        return false;
//    }
//    return true;
}


/**
 * @brief Stu_Info::save_changed_info 改变类成员变量以及发送给主界面将新的个人信息保存到服务器的信号
 */
void Stu_Info::save_changed_info()
{
    m_name = this->ui->stu_name_tx->text();
    m_sex = this->ui->stu_sex_tx->text();
    m_academy = this->ui->stu_academy_tx->text();
    m_grade = this->ui->stu_grade_tx->text();
    m_major = this->ui->stu_major_tx->text();
    m_class = this->ui->stu_class_tx->text();
    m_tell = this->ui->stu_tell_tx->text();
    m_qq = this->ui->stu_qq_tx->text();
    m_completed_info = true;

    save_personal_info_to_server();
}


/**
 * @brief Stu_Info::save_personal_info_to_server 连接服务器保存修改后的个人信息
 */
void Stu_Info::save_personal_info_to_server()
{
    //修改类成员变量
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

    emit save_updated_info_to_server(); //信号发送给stu_mainwindow.cpp
}


/**
 * @brief Stu_Info::set_personal_info_json 设置发送给服务器的用户个人信息数据
 * return 要发送给服务器的json数据包
 */
QByteArray Stu_Info::set_personal_info_json()
{
    QMap<QString, QVariant> info;
    info.insert("sender", "student");            //用户类别
    info.insert("module", "deal_stu_info");     //请求的相关处理模块
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



/**
 * @brief Stu_Info::read_back_messages 读取服务器返回的用户数据json包
 */
void Stu_Info::read_back_messages()
{
    QByteArray alldata = this->m_tcpSocket->readAll();
    QByteArray back_buf = alldata;
    qDebug()<<"back_buf:"<<back_buf;

    //获取事件处理的返回信息
    QString service;
    QString msg;
    get_back_json(back_buf,service, msg);
    if(service == "save_personal_info_to_server"){
        if(msg=="true"){
            QMessageBox::information(this, tr("提示"),
                                         QString(tr("保存成功！")),
                                         QMessageBox::Yes);
        }
        else{
            QMessageBox::warning(this, tr("提示"),
                                         QString(tr("保存失败！")),
                                         QMessageBox::Yes);
        }
    }
}



/**
 * @brief Stu_Info::get_back_json 解析服务器返回的用户数据json包
 * @param back_buf, service... 缓冲区以及需要解析出来的json包里的内容
 */
void Stu_Info::get_back_json(QByteArray back_buf, QString &service, QString &msg)
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
            if (object.contains("msg")) {
                QJsonValue value = object.value("msg");
                if (value.isString()) {
                    msg = value.toString();
                }
            }
        }
    }
    qDebug()<<"保存个人信息返回的信息包："<<service<<" "<<msg;
}



/**
 * @brief Stu_Info::updated_info_return 给主界面调用，查询修改过后的个人信息
 * @param name, sex, academy...  当前用户可修改的个人信息
 */
void Stu_Info::updated_info_return(QString &name, QString &sex, QString &academy, QString &grade,
                         QString &major, QString &clas,QString &tell, QString &qq, bool &completed_info)
{
    name = m_name;
    sex = m_sex;
    academy = m_academy;
    grade = m_grade;
    major = m_major;
    clas = m_class;
    tell = m_tell;
    qq = m_qq;
    completed_info = m_completed_info;
}


void Stu_Info::mousePressEvent(QMouseEvent *event)
{
//     如果是左键, 计算窗口左上角, 和当前按钮位置的距离
    if(event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_pt = event->pos();
    }
}

void Stu_Info::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void Stu_Info::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bPressed)
        move(event->pos() - m_pt + pos());
}
