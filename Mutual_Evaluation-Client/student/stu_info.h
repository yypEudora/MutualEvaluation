#ifndef STU_INFO_H
#define STU_INFO_H

#include <QWidget>

namespace Ui {
class Stu_Info;
}

class Stu_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Stu_Info(QWidget *parent = nullptr);
    ~Stu_Info();
    void show_mainwindow(QString pwd,QString name, QString sex, QString academy, QString grade,
                         QString major, QString clas,QString tell, QString qq);
    void manage_signals(); //管理信号槽连接
    void set_ui();
    void back_to_stu_mainwindow(); //返回到学生主界面
    bool check_info_updated(); //检查个人信息是否修改
    void save_changed_info();  //改变类成员变量以及发送给主界面将新的个人信息
                                //保存到服务器的信号
    //给主界面调用，查询修改过后的个人信息
    void updated_info_return(QString &name, QString &sex, QString &academy, QString &grade,
                             QString &major, QString &clas,QString &tell, QString &qq);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void back_to_mainwindow();
    void save_to_server(); //修改过的个人信息写到服务器的信号
    void save_success();    //保存成功的信号

private slots:

private:
    Ui::Stu_Info *ui;

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击

    QString m_password;
    QString m_name;
    QString m_sex;
    QString m_academy;
    QString m_grade;
    QString m_major;
    QString m_class;
    QString m_tell;
    QString m_qq;

};

#endif // STU_INFO_H
