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
    void showMainWindow();
    // 处理信号
    void managerSignals();
    // 重新登陆
    void loginAgain();

signals:
    // 切换用户按钮信号
    void changeUser();

private:
    Ui::zj_mainwindow *ui;
};

#endif // ZJ_MAINWINDOW_H
