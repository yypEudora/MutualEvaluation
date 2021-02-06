#include "titlewg.h"
#include "ui_titlewg.h"

TitleWg::TitleWg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleWg)
{
    ui->setupUi(this);
    setAttribute(Qt::WidgetAttribute::WA_StyledBackground);
    setAutoFillBackground(true);
    setProperty("type", QVariant("video_thumb"));
}

TitleWg::~TitleWg()
{
    delete ui;
}
