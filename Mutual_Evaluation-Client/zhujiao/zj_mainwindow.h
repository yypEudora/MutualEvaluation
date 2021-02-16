#ifndef ZJ_MAINWINDOW_H
#define ZJ_MAINWINDOW_H

#include <QMainWindow>
#include "common/common.h"

namespace Ui {
class Zj_mainwindow;
}

class Zj_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Zj_mainwindow(QWidget *parent = nullptr);
    ~Zj_mainwindow();

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
    Ui::Zj_mainwindow *ui;
};

#endif // ZJ_MAINWINDOW_H
