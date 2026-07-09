#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    p_tab1_socket = new tab1_socket(ui->p_tab_socket);
    ui->p_tab_socket->setLayout(p_tab1_socket->layout());

    p_tab2_sensor = new tab2_sensor(ui->p_tab_sensor);
    ui->p_tab_sensor->setLayout(p_tab2_sensor->layout());

    p_tab3_sqlite = new tab3_sqlite(ui->p_tab_sqlite);
    ui->p_tab_sqlite->setLayout(p_tab3_sqlite->layout());

    connect(p_tab1_socket, SIGNAL(sensor_recv_data_sig(QStringList&)), p_tab2_sensor, SLOT(tab2_sensor_recv_data_slot(QStringList&)));
    connect(p_tab1_socket, SIGNAL(sqlite_recv_data_sig(QStringList&)), p_tab3_sqlite, SLOT(tab3_sqlite_recv_data_slot(QStringList&)));
}

MainWidget::~MainWidget()
{
    delete ui;
}
