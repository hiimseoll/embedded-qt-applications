#include "tab1_socket.h"
#include "ui_tab1_socket.h"

tab1_socket::tab1_socket(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tab1_socket)
{
    ui->setupUi(this);
    p_socket_client = new SocketClient(this);
    p_keyboard = new Keyboard();

    connect(p_socket_client, SIGNAL(socketRecvDataSig(QString&)), this, SLOT(update_recv_data_slot(QString&)));
}

tab1_socket::~tab1_socket()
{
    delete ui;
}

void tab1_socket::on_p_connect_button_clicked(bool checked){
    bool flag;

    if(checked){
        p_socket_client->connectToServerSlot(flag);

        if(flag){
            ui->p_connect_button->setText("서버 해제");
        }
    }
    else{
        p_socket_client->socketClosedServerSlot();
        ui->p_connect_button->setText("서버 연결");
    }

}


void tab1_socket::on_p_send_button_clicked(){
    QString str_target_id = ui->p_id_input->text();
    QString str_send_msg = ui->p_msg_input->text();

    str_send_msg = str_target_id.isEmpty() ? "[ALLMSG]" + str_send_msg :
                                             "[" + str_target_id + "]" + str_send_msg;

    p_socket_client->socketWriteDataSlot(str_send_msg);

    ui->p_msg_input->clear();
}

void tab1_socket::update_recv_data_slot(QString& str_recv_msg){
    QStringList str_list;

    str_recv_msg.chop(1);
    ui->p_monitor->append(QTime::currentTime().toString() + " " + str_recv_msg);

    str_recv_msg.replace("[", "@");
    str_recv_msg.replace("]", "@");
    str_list = str_recv_msg.split("@");

    if(str_list[2].compare("SENSOR") == 0){
        emit sensor_recv_data_sig(str_list);
        emit sqlite_recv_data_sig(str_list);
    }
}



void tab1_socket::on_p_id_input_selectionChanged(){
    QLineEdit* p_QLineEdit = (QLineEdit*)sender();
    p_keyboard->setLineEdit(p_QLineEdit);
    p_keyboard->show();
}


void tab1_socket::on_p_msg_input_selectionChanged(){
    QLineEdit* p_QLineEdit = (QLineEdit*)sender();
    p_keyboard->setLineEdit(p_QLineEdit);
    p_keyboard->show();
}