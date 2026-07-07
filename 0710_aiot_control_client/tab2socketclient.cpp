#include "tab2socketclient.h"
#include "ui_tab2socketclient.h"

Tab2SocketClient::Tab2SocketClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab2SocketClient)
{
    ui->setupUi(this);

    p_SocketClient = new SocketClient(this);
    p_Keyboard = new Keyboard();
    connect(p_SocketClient, SIGNAL(socketRecvDataSig(QString)), this, SLOT(update_recv_data_slot(QString)));
}

Tab2SocketClient::~Tab2SocketClient()
{
    delete ui;
}

void Tab2SocketClient::on_p_PB_servConn_clicked(bool checked)
{
    bool flag;

    if(checked){
        p_SocketClient->connectToServerSlot(flag);

        if(flag){
            ui->p_PB_servConn->setText("연결 해제");
        }
    }
    else{
        p_SocketClient->socketClosedServerSlot();
        ui->p_PB_servConn->setText("서버 연결");
    }
}


void Tab2SocketClient::on_p_PB_send_clicked()
{
    QString str_recv_id = ui->p_LE_recvId->text();
    QString str_send_data = ui->p_LE_sendMsg->text();

    if(str_recv_id.isEmpty()){
        str_send_data = "[ALLMSG]" + str_send_data;
    }
    else{
        str_send_data = "[" + str_recv_id + "]" + str_send_data;
    }

    p_SocketClient->socketWriteDataSlot(str_send_data);

    ui->p_LE_sendMsg->clear();
}

void Tab2SocketClient::update_recv_data_slot(QString str_recv_data){
    QStringList str_list;

    str_recv_data.chop(1);
    ui->p_TE_recvData->append(QTime::currentTime().toString() + " " + str_recv_data);

    str_recv_data.replace("[", "@");
    str_recv_data.replace("]", "@");
    str_list = str_recv_data.split("@");

    if(str_list[2].compare("LED") == 0){
        bool flag = false;
        int led_num = str_list[3].toInt(&flag, 16);

        if(flag){
            emit led_write_sig(led_num);
        }
    }
    else if((str_list[2].indexOf("LAMP") == 0) || (str_list[2].indexOf("GAS") == 0)){ // indexof 써보기
        emit tab3_recv_data_sig(str_list);
    }
    else if(str_list[2].compare("SENSOR") == 0){
        emit tab4_recv_data_sig(str_list);
    }
}

void Tab2SocketClient::send_cmd_via_socket_slot(QString target_id, QString cmd){

    p_SocketClient->socketWriteDataSlot("[" + target_id + "]" + cmd);
}

void Tab2SocketClient::on_p_LE_recvId_selectionChanged()
{
    QLineEdit* p_QLineEdit = (QLineEdit*)sender();
    p_Keyboard->setLineEdit(p_QLineEdit);
    p_Keyboard->show();
}


void Tab2SocketClient::on_p_LE_sendMsg_selectionChanged()
{
    QLineEdit* p_QLineEdit = (QLineEdit*)sender();
    p_Keyboard->setLineEdit(p_QLineEdit);
    p_Keyboard->show();
}

