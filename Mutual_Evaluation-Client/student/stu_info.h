#ifndef STU_INFO_H
#define STU_INFO_H

#include <QWidget>

namespace Ui {
class Stu_Info;
}

class Stu_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Stu_Info(QWidget *parent = nullptr);
    ~Stu_Info();
    void manage_signals(); //管理信号槽连接
    void set_ui();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:

private:
    Ui::Stu_Info *ui;

    QPoint m_pt;
    bool m_bPressed = true; //响应鼠标点击
};

#endif // STU_INFO_H
