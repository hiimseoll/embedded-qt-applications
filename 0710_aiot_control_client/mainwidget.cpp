#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    p_Tab1DeviceControl = new Tab1DeviceControl(ui->p_tab_1);
    ui->p_tab_1->setLayout(p_Tab1DeviceControl->layout());
}

mainWidget::~mainWidget()
{
    delete ui;
}
