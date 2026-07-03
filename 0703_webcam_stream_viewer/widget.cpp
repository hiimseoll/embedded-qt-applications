#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    p_QWebEngineView0_0 = new QWebEngineView(this);
    p_QWebEngineView0_1 = new QWebEngineView(this);
    p_QWebEngineView1_0 = new QWebEngineView(this);
    p_QWebEngineView1_1 = new QWebEngineView(this);

    p_QWebEngineView0_0->load(QUrl("http://127.0.0.1:8080/?action=stream"));
    ui->verticalLayout0_0->addWidget(p_QWebEngineView0_0);

    p_QWebEngineView0_1->load(QUrl("http://127.0.0.1:8080/?action=stream"));
    ui->verticalLayout0_1->addWidget(p_QWebEngineView0_1);

    p_QWebEngineView1_0->load(QUrl("http://127.0.0.1:8080/?action=stream"));
    ui->verticalLayout1_0->addWidget(p_QWebEngineView1_0);

    p_QWebEngineView1_1->load(QUrl("http://127.0.0.1:8080/?action=stream"));
    ui->verticalLayout1_1->addWidget(p_QWebEngineView1_1);
}


Widget::~Widget()
{
    delete ui;
}
