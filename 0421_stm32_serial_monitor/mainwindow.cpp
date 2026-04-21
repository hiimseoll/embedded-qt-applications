#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->openButton->setProperty("isConnected", false);
    ui->openButton->style()->unpolish(ui->openButton);
    ui->openButton->style()->polish(ui->openButton);

    ui->tempValue->style()->unpolish(ui->tempValue);
    ui->tempValue->style()->polish(ui->tempValue);

    ui->portCombo->addItem("/dev/ttyACM0");
    ui->portCombo->addItem("/dev/ttyACM1");

    ui->baudCombo->addItem("115200");
    ui->baudCombo->addItem("9600");

    applyStyles();

    //ui->openButton->setStyleSheet("background-color: green; color: white;");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_ledBlink1sButton_clicked(){

    if(serial != NULL && serial->isOpen()){
        QString led_toggle_cmd = "led toggle 1000\r\n";

        qint64 bytes_written = serial->write(led_toggle_cmd.toUtf8());

        serial->flush();

        if(bytes_written == -1){
            ui->textBrowser->append("Serial write error.");
        }
        else{
            ui->textBrowser->append("Tx => " + led_toggle_cmd);
        }
    }
}

void MainWindow::on_monitorStartButton_clicked(){

    if(serial != NULL && serial->isOpen()){
        QString cmd = "monitor on 1000\r\ntemp 1000\r\nled toggle 1000\r\n";
        qint64 bytes_written = serial->write(cmd.toUtf8());
        serial->flush();

        if(bytes_written == -1){
            ui->textBrowser->append("Serial write error.");
        }
        else{
            ui->textBrowser->append("Tx => " + cmd);
        }
    }
}

void MainWindow::on_monitorStopButton_clicked(){

    if(serial != NULL && serial->isOpen()){
        QString cmd = "monitor off\r\ntemp\r\nled off\r\n";
        qint64 bytes_written = serial->write(cmd.toUtf8());
        serial->flush();

        if(bytes_written == -1){
            ui->textBrowser->append("Serial write error.");
        }
        else{
            ui->textBrowser->append("Tx => " + cmd);
        }
    }
}

void MainWindow::on_openButton_clicked(){

    if(serial != NULL && ui->openButton->text() == "Close" && serial->isOpen()){
        serial->close();
        serial = NULL;

        ui->textBrowser->append("Conneciton: port closed.\n");
        ui->openButton->setText("Open");
        ui->openButton->setProperty("isConnected", false);
        ui->openButton->style()->unpolish(ui->openButton);
        ui->openButton->style()->polish(ui->openButton);
        //ui->openButton->setStyleSheet("background-color: green; color: white;");

        return;
    }

    serial = new QSerialPort(this);

    QString portName = ui->portCombo->currentText();
    int32_t baudRate = ui->baudCombo->currentText().toInt();

    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    qint64 pid = getSerialPortPID(portName);

    if(pid > 0){
        qDebug() << portName << "는 현재 사용 중입니다. PID:" << pid;
        return;
    }
    else{
        if(serial->open(QIODevice::ReadWrite)){
            ui->textBrowser->append("Connection: Port opened.\n");
            ui->openButton->setText("Close");
            ui->openButton->setProperty("isConnected", true);
            ui->openButton->style()->unpolish(ui->openButton);
            ui->openButton->style()->polish(ui->openButton);

            //ui->openButton->setStyleSheet("background-color: red; color: white;");
        }
        else{
            ui->textBrowser->append("Conneciton: Port open failed.\n");
            serial = NULL;
        }
    }

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
}

void MainWindow::readData(){
    m_buffer.append(serial->readAll());
    QByteArray data;

    while(m_buffer.contains('$') && m_buffer.contains('#')){
        int start = m_buffer.indexOf('$');
        int end = m_buffer.indexOf('#', start);

        if(end == -1) break;

        QByteArray packet = m_buffer.mid(start + 1, end - start - 1);
        m_buffer.remove(0, end + 1);

        if(!packet.isEmpty()){
            //ui->textBrowser->append(packet);
            parse_protocol(packet);
        }
    }

/*
    while(m_buffer.contains('\n')){
        int line_feed_idx = m_buffer.indexOf('\n');
        data = m_buffer.remove(0, line_feed_idx + 1);

        if(m_buffer.contains('\r')){
            m_buffer.chop(1);
        }
    }
*/
/*
    QByteArray buffer = serial->readAll();
    QByteArray data;

    buffer.removeAt(buffer.indexOf('\r'));
    //buffer.removeAt(buffer.indexOf('\n'));

    ui->textBrowser->insertPlainText(buffer);
*/
}

void MainWindow::parse_protocol(const QByteArray &packet){
    QString data = QString::fromUtf8(packet);
    QStringList parts = data.split(',');

    if(parts.size() < 2) return;

    // parts[0] == data count
    for(int i = 1; i < parts.size(); ++i){
        QStringList item = parts[i].split(':');

        if(item.size() < 3) continue;

        int sensor_id = item[0].toInt();
        int sensor_type = item[1].toInt();
        QString val_str = item[2];


        switch(sensor_id){
        case ID_ENV_TEMP:
            ui->tempValue->setText("Temp: " + val_str + " ℃");
            break;
        case ID_OUT_LED_STATE:
            ui->ledValue->setText(val_str == "1" ? "🟢" : " ");
            break;
        default:
            break;
        }
    }
}

qint64 MainWindow::getSerialPortPID(QString portPath){
    QProcess process;

    process.start("lsof", QStringList() << "-t" << portPath);
    process.waitForFinished();

    QString output = process.readAllStandardOutput().trimmed();

    if(output.isEmpty()){
        return 0;
    }
    return output.toLongLong();
}

void MainWindow::applyStyles(){
    QString qss = R"(
        QPushButton#openButton[isConnected = false] {background-color: green; color: white;}
        QPushButton#openButton[isConnected = true] {background-color: red; color: white;}

        /*color*/
        QLabel#tempValue {font-size: 14px; font-weight: 700;}
    )";

    this->setStyleSheet(qss);
}
