#ifndef ZJ_MAINWINDOW_H
#define ZJ_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"

namespace Ui {
class zj_mainwindow;
}

class zj_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zj_mainwindow(QWidget *parent = nullptr);
    ~zj_mainwindow();

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
    Ui::zj_mainwindow *ui;
};

#endif // ZJ_MAINWINDOW_H
