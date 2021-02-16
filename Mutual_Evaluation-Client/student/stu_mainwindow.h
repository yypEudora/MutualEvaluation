#ifndef STU_MAINWINDOW_H
#define STU_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"
#include "student/stu_info.h"

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
    void manager_signals();
    // 重新登陆
    void login_again();
    // 界面设置
    void ui_set();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    // 切换用户按钮信号
    void change_user();

private:
    Ui::Stu_Mainwindow *ui;
    QMenu *m_stu_info_menu;
    QMenu *m_change_user_menu;
    QMenu *m_stu_exit_menu;

    Stu_Info* m_stu_info;

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
    bool m_max_or_normal = true; //界面放大缩小
};

#endif // STU_MAINWINDOW_H
