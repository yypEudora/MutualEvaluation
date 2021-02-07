#ifndef TC_MAINWINDOW_H
#define TC_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"

namespace Ui {
class tc_mainwindow;
}

class tc_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit tc_mainwindow(QWidget *parent = nullptr);
    ~tc_mainwindow();

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
    Ui::tc_mainwindow *ui;
};

#endif // TC_MAINWINDOW_H
