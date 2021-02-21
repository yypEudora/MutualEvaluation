#ifndef STU_PWD_H
#define STU_PWD_H

#include <QWidget>

namespace Ui {
class Stu_Pwd;
}

class Stu_Pwd : public QWidget
{
    Q_OBJECT

public:
    explicit Stu_Pwd(QWidget *parent = nullptr);
    ~Stu_Pwd();
    void show_mainwindow(QString pwd);
    void manage_signals(); //管理信号槽连接
    bool check_pwd_valid(); //检查输入密码的合法性
    bool check_before_pwd_correct(); //验证旧密码是否正确
    //给学生主界面调用，查询修改过后的密码
    void updated_pwd_return(QString &pwd);
    //给login界面调用，将输入的原始密码进行加密
    void return_input_pwd(QString &pwd);
    //给login界面调用，将输入的新密码进行加密
    void return_input_new_pwd(QString &new_pwd);

    void set_ui();

    QString pwd_str_md5;    //原始密码加密，login.cpp处理
    QString new_pwd_str_md5;//新密码加密，login.cpp处理

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void back_to_mainwindow();
    void save_updated_pwd_to_server(); //修改过的个人信息写到服务器的信号，由学生主界面转发到login.cpp进行加密处理
    void check_before_pwd();            //修改过的密码写到服务器的信号，由学生主界面转发到login.cpp进行加密处理

private:
    Ui::Stu_Pwd *ui;

    QString m_password;

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
};

#endif // STU_PWD_H
