/*
 * stu_mainwindow.h
 * 1.简述：学生用户主界面窗口
 * 2.主要功能：初始化用户数据，
 * 3.被引用：login.cpp
 */


#ifndef STU_MAINWINDOW_H
#define STU_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"
#include "student/stu_info.h"
#include "student/stu_pwd.h"

namespace Ui {
class Stu_Mainwindow;
}

class Stu_Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Stu_Mainwindow(QWidget *parent = nullptr);
    ~Stu_Mainwindow();
    // 显示主窗口
    void show_mainwindow();
    // 处理信号
    void manage_signals();
    // 切换用户，重新登陆
    void login_again();
    // 界面设置
    void ui_set();

    // 连接服务器获取用户数据
    void acquire_user_data();
    //设置发送给服务器的用户以及请求获取用户数据
    QByteArray set_user_data_json();



    //解析服务器发送过来的json数据包
    void get_user_data_back_json(QByteArray back_buf, QString &msg, QString &service, QString &pwd, QString &name, QString &sex,
                        QString &academy, QString &grade, QString &major, QString &clas,
                        QString &tell, QString &qq, int &course_number, bool &completed_info); //解析返回的json数据包



    //为了返回给login.cpp的修改密码界面对象（因为需要在login.cpp对密码进行加密）
    Stu_Pwd* return_stu_pwd_window();



protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    // 切换用户按钮信号
    void change_user();

    //为了使加密的方式相同，应转交给login.cpp处理
    void check_before_pwd();
    void save_updated_pwd_to_server();

private slots:
    // 读取服务器返回的用户数据json包
    void read_back_messages();

private:
    Ui::Stu_Mainwindow *ui;
    QMenu *m_stu_info_menu;
    QMenu *m_change_user_menu;
    QMenu *m_stu_exit_menu;

    Stu_Info* m_stu_info;//修改个人信息界面
    Stu_Pwd* m_stu_pwd;//修改密码界面

    QString m_user; //当前登录用户
    QString m_password;
    QString m_name;
    QString m_sex;
    QString m_academy;
    QString m_grade;
    QString m_major;
    QString m_class;
    QString m_tell;
    QString m_qq;
    int m_course_number;
    bool m_completed_info; //是否完善个人信息

    QString m_ip;
    QString m_port;
    QTcpSocket *m_tcpSocket;//socket

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
    bool m_max_or_normal = true; //界面放大缩小

};

#endif // STU_MAINWINDOW_H
