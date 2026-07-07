#ifndef TAB3CONTROLPANEL_H
#define TAB3CONTROLPANEL_H

#include <QWidget>
#include <QPalette>

namespace Ui {
class Tab3ControlPanel;
}

class Tab3ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit Tab3ControlPanel(QWidget *parent = nullptr);
    ~Tab3ControlPanel();

private slots:
    void on_p_PB_Lamp_clicked(bool checked);
    void on_p_PB_Plug_clicked(bool checked);
    void tab3_recv_data_slot(QStringList);

private:
    Ui::Tab3ControlPanel *ui;
    QPalette paletteOn;
    QPalette paletteOff;

signals:
    void socket_send_data_sig(QString, QString);

};
#endif // TAB3CONTROLPANEL_H
