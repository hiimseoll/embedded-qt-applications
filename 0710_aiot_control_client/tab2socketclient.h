#ifndef TAB2SOCKETCLIENT_H
#define TAB2SOCKETCLIENT_H

#include <QWidget>
#include <QTime>
#include <QLineEdit>
#include "socketclient.h"
#include "keyboard.h"

namespace Ui {
class Tab2SocketClient;
}

class Tab2SocketClient : public QWidget
{
    Q_OBJECT

public:
    explicit Tab2SocketClient(QWidget *parent = nullptr);
    ~Tab2SocketClient();

private slots:
    void on_p_PB_servConn_clicked(bool checked);
    void on_p_PB_send_clicked();
    void update_recv_data_slot(QString);
    void send_cmd_via_socket_slot(QString, QString);
    void on_p_LE_recvId_selectionChanged();
    void on_p_LE_sendMsg_selectionChanged();

private:
    Ui::Tab2SocketClient *ui;
    SocketClient* p_SocketClient;
    Keyboard* p_Keyboard;

signals:
    void led_write_sig(int);
    void tab3_recv_data_sig(QStringList);
    void tab4_recv_data_sig(QStringList);

};

#endif // TAB2SOCKETCLIENT_H
