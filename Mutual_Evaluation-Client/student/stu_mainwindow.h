#ifndef STU_MAINWINDOW_H
#define STU_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"

namespace Ui {
class stu_mainwindow;
}

class stu_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit stu_mainwindow(QWidget *parent = nullptr);
    ~stu_mainwindow();

    // 显示主窗口
    void show_mainwindow();
    // 处理信号
    void manager_signals();
    // 重新登陆
    void login_again();

signals:
    // 切换用户按钮信号
    void change_user();

private:
    Ui::stu_mainwindow *ui;
};

#endif // STU_MAINWINDOW_H
