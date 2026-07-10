#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    p_Tab1DeviceControl = new Tab1DeviceControl(ui->p_tab_1);
    ui->p_tab_1->setLayout(p_Tab1DeviceControl->layout());

    p_Tab2SocketClient = new Tab2SocketClient(ui->p_tab_2);
    ui->p_tab_2->setLayout(p_Tab2SocketClient->layout());

    p_Tab3ControlPanel = new Tab3ControlPanel(ui->p_tab_3);
    ui->p_tab_3->setLayout(p_Tab3ControlPanel->layout());

    p_Tab4SensorChart = new Tab4SensorChart(ui->p_tab_4);
    ui->p_tab_4->setLayout(p_Tab4SensorChart->layout());

    p_Tab5SensorDatabase = new Tab5SensorDatabase(ui->p_tab_5);
    ui->p_tab_5->setLayout(p_Tab5SensorDatabase->layout());

    p_Tab6WebCamera = new Tab6WebCamera(ui->p_tab_6);
    ui->p_tab_6->setLayout(p_Tab6WebCamera->layout());

    p_Tab7CamOpencv = new Tab7CamOpencv(ui->p_tab_7);
    ui->p_tab_7->setLayout(p_Tab7CamOpencv->layout());

    ui->tabWidget->setCurrentIndex(1);

//  connect(p_Tab2SocketClient, SIGNAL(led_write_sig(int)), &p_Tab1DeviceControl->get_LedKeyDev(), SLOT(writeLedDataSlot(int)));
    connect(p_Tab2SocketClient, SIGNAL(led_write_sig(int)), &p_Tab1DeviceControl->get_dialLed(), SLOT(setValue(int)));
    connect(p_Tab1DeviceControl, SIGNAL(check_box_sig(QString, QString)), p_Tab2SocketClient, SLOT(send_cmd_via_socket_slot(QString, QString)));
    connect(p_Tab3ControlPanel, SIGNAL(socket_send_data_sig(QString, QString)), p_Tab2SocketClient, SLOT(send_cmd_via_socket_slot(QString, QString)));
    connect(p_Tab2SocketClient, SIGNAL(tab3_recv_data_sig(QStringList&)), p_Tab3ControlPanel, SLOT(tab3_recv_data_slot(QStringList&)));
    connect(p_Tab2SocketClient, SIGNAL(tab4_recv_data_sig(QStringList&)), p_Tab4SensorChart, SLOT(tab4_recv_data_slot(QStringList&)));
    connect(p_Tab2SocketClient, SIGNAL(tab5_recv_data_sig(QStringList&)), p_Tab5SensorDatabase, SLOT(tab5_recv_data_slot(QStringList&)));
    connect(p_Tab7CamOpencv->getpWebCamThread(), SIGNAL(socketSendDataSig(QString, QString)), p_Tab2SocketClient, SLOT(send_cmd_via_socket_slot(QString, QString)));
}

mainWidget::~mainWidget()
{
    delete ui;
}
