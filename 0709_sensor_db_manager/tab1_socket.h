#ifndef TAB1_SOCKET_H
#define TAB1_SOCKET_H

#include <QWidget>
#include "socketclient.h"
#include "keyboard.h"

namespace Ui {
class tab1_socket;
}

class tab1_socket : public QWidget
{
    Q_OBJECT

public:
    explicit tab1_socket(QWidget *parent = nullptr);
    ~tab1_socket();

private slots:
    void on_p_connect_button_clicked(bool checked);
    void on_p_send_button_clicked();
    void update_recv_data_slot(QString&);
    void on_p_msg_input_selectionChanged();
    void on_p_id_input_selectionChanged();

private:
    Ui::tab1_socket *ui;
    SocketClient* p_socket_client;
    Keyboard* p_keyboard;

signals:
    void sensor_recv_data_sig(QStringList&);
    void sqlite_recv_data_sig(QStringList&);
};

#endif // TAB1_SOCKET_H
