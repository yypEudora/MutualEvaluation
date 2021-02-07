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
    void showMainWindow();
    // 处理信号
    void managerSignals();
    // 重新登陆
    void loginAgain();

signals:
    // 切换用户按钮信号
    void changeUser();
private:
    Ui::tc_mainwindow *ui;
};

#endif // TC_MAINWINDOW_H
