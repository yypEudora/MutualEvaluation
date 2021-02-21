#ifndef TC_PWD_H
#define TC_PWD_H

#include <QWidget>

namespace Ui {
class Tc_Pwd;
}

class Tc_Pwd : public QWidget
{
    Q_OBJECT

public:
    explicit Tc_Pwd(QWidget *parent = nullptr);
    ~Tc_Pwd();
    void manage_signals(); //管理信号槽连接
    void set_ui();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Ui::Tc_Pwd *ui;

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
};

#endif // TC_PWD_H
