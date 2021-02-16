#ifndef TC_MAINWINDOW_H
#define TC_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"
#include "teacher/tc_info.h"
namespace Ui {
class Tc_Mainwindow;
}

class Tc_Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tc_Mainwindow(QWidget *parent = nullptr);
    ~Tc_Mainwindow();

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
    Ui::Tc_Mainwindow *ui;
    QMenu *m_tc_info_menu;
    QMenu *m_change_user_menu;
    QMenu *m_tc_exit_menu;

    Tc_Info* m_tc_info;

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
    bool m_max_or_normal = true; //界面放大缩小
};

#endif // TC_MAINWINDOW_H
