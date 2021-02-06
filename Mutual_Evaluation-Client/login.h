#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    void get_ip_port();
    void manage_signals();
    ~Login();
protected: 
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

//    void mouse_move_event(QMouseEvent *event);
//    void mouse_press_event(QMouseEvent *event);
//    void mouse_release_event(QMouseEvent *event);

private slots:
//    void on_student_box_stateChanged(int arg1);

//    void on_teacher_box_stateChanged(int arg1);

//    void on_zhujiao_box_stateChanged(int arg1);

    void on_login_btn_clicked();

    void on_reg_btn_clicked();

    void on_set2_btn_clicked();

private:
    Ui::Login *ui;
    QPoint m_pt;
    bool m_bPressed = true;
    QWidget *m_parent;  // 父窗口指针

};
#endif // LOGIN_H
