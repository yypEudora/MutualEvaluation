#ifndef TC_INFO_H
#define TC_INFO_H

#include <QWidget>

namespace Ui {
class Tc_Info;
}

class Tc_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Tc_Info(QWidget *parent = nullptr);
    ~Tc_Info();
    void manage_signals(); //管理信号槽连接
    void set_ui();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Tc_Info *ui;
    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
};

#endif // TC_INFO_H
