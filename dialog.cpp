#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>      //Potrzebne biblioteki do obsługi portu szeregowego
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;

/* KOD DO SPRAWDZANIA VENDOR ID, PRODUCT ID 67 DLA ARDUINO UNO
    qDebug() << "Numery dostępnych portów: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Ma Vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Ma Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }
*/

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available){
        // Otwiera i konfiguruje port szeregowy
        arduino->setPortName(arduino_port_name); // SCHEMAT USTAWIANIA PORTU ARDUINO
        arduino->open(QSerialPort::WriteOnly); //Tryb tylko do odczytu, nic nie zapisujemy.
        arduino->setBaudRate(QSerialPort::Baud9600); //Bity na sekundę MUSI BYĆ TAKI SAM JAK NA .ino !!
        arduino->setDataBits(QSerialPort::Data8); // Wielkość słowa
        arduino->setParity(QSerialPort::NoParity); 
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }else{
        // Wyświelti komunikat błędu jeśli arduino będzie niedostępne
        QMessageBox::warning(this, "Błąd!", "Nie można znaleźć arduino!");
    }
}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close(); //Dekonstruktor
    }
    delete ui;
}

void Dialog::on_red_slider_valueChanged(int value) //Kiedykolwiek sygnał będzie zmieniony, wykona się kod między nawiasami.
{
    ui->red_value_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("r%1").arg(value)); /* Wartość ze suwaka konwertujemy na Qstring, przekazuje później jako argument */
    qDebug() << value;
}

void Dialog::on_green_slider_valueChanged(int value)
{
    ui->green_value_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("g%1").arg(value));
    qDebug() << value;
}

void Dialog::on_blue_slider_valueChanged(int value)
{
    ui->blue_value_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("b%1").arg(value));
    qDebug() << value;
}

void Dialog::updateRGB(QString command) //Aktualizacja wartości RGB dla diod.
{
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
    }else{
        qDebug() << "Nie można zapisać, przez port szeregowy!";
    }
}
