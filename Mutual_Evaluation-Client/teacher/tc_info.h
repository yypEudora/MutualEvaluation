/*
 * tc_info.h
 * 1.简述：教师用户个人信息界面
 * 2.主要功能：修改个人信息并保存
 * 3.被引用：tc_mainwindow.h
 */


#ifndef TC_INFO_H
#define TC_INFO_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Tc_Info;
}

class Tc_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Tc_Info(QWidget *parent = nullptr);
    ~Tc_Info();
    void show_mainwindow(QString pwd,QString name, QString sex, QString academy, QString email,
                         QString tell, QString qq, bool completed_info);


    void manage_signals(); //管理信号槽连接
    void set_ui();
    void back_to_tc_mainwindow(); //返回到教师主界面
    bool check_info_updated(); //检查个人信息是否修改
    bool check_info_valid(); //检查个人信息的合法性
    void save_changed_info();  //改变类成员变量以及发送给主界面将新的个人信息
                                //保存到服务器的信号

    // 连接服务器保存修改后的个人信息
    void save_personal_info_to_server();
    //设置发送给服务器的用户个人信息数据
    QByteArray set_personal_info_json();

    //解析服务器发送过来的json数据包
    void get_back_json(QByteArray back_buf, QString &service, QString &msg);


    //给主界面调用，查询修改过后的个人信息
    void updated_info_return(QString &name, QString &sex, QString &academy, QString &email,
                             QString &tell, QString &qq, bool &completed_info);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


signals:
    void back_to_mainwindow();
    void save_updated_info_to_server(); //修改过的个人信息写到服务器的信号
    void save_success();    //保存成功的信号

private slots:
    // 读取服务器返回的用户数据json包
    void read_back_messages();

private:
    Ui::Tc_Info *ui;
    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击

    QString m_user;
    QString m_password;
    QString m_name;
    QString m_sex;
    QString m_academy;
    QString m_email;
    QString m_tell;
    QString m_qq;
    bool m_completed_info;

    QString m_ip;
    QString m_port;
    QTcpSocket *m_tcpSocket;//socket
};

#endif // TC_INFO_H
