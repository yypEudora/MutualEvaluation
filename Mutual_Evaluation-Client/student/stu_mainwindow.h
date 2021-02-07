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
    void showMainWindow();
    // 处理信号
    void managerSignals();
    // 重新登陆
    void loginAgain();

signals:
    // 切换用户按钮信号
    void changeUser();

private:
    Ui::stu_mainwindow *ui;
};

#endif // STU_MAINWINDOW_H
