#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

using namespace std;

#define DEFAULT_VOLUME 10

struct volume_lvl {
    QString labl;
    int percent;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // get inisial volume value
    int value = getSystemVolume();

    // set volume to dial first render
    horizontalSlider = ui->horizontalSlider;
    horizontalSlider->setRange(0, 100);
    horizontalSlider->setValue(value);

    // set volume value to label
    ui->label->setText(QString::number(value) + "%");
    // update label button

    QString labl = value < 1 ? "UnMute" : "Mute";
    ui->pushButton->setText(labl);

    getDevices();

    // add event valueChanged for slider
    // connect(ui->comboBox, &QComboBox::highlighted, this, &MainWindow::setOutput);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSystemVolume(int value)
{
    QProcess::execute("pactl", {"set-sink-volume", "@DEFAULT_SINK@", QString::number(value) + "%"});
}

int MainWindow::getSystemVolume()
{
    QProcess process;
    process.start("pactl", {"get-sink-volume", "@DEFAULT_SINK@"});
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList parts = output.split('/');
    if (parts.size() > 1) {
        bool ok;
        int volume = parts[1].trimmed().remove('%').toInt(&ok);
        if (ok) return volume;
    }
    return DEFAULT_VOLUME;
}

void MainWindow::getDevices(){
    QProcess process;
    process.start("pactl", {"-f", "json", "list"});
    process.waitForFinished();
    QByteArray out = process.readAllStandardOutput();

    // convert stdout to json document
    QJsonDocument json = QJsonDocument::fromJson(out);
    QJsonObject root = json.object();
    QJsonArray ports = root["sinks"][0]["ports"].toArray();
    this->ports = ports;
    // QJsonArray ports = sources["ports"].toArray();

    QStringList lists;

    for(const QJsonValue &val : ports){
        QJsonObject obj = val.toObject();
        lists.append(obj["description"].toString());
    }


    ui->comboBox->addItems(lists);
}

void MainWindow::setOutput(int idx){


}

void MainWindow::on_pushButton_clicked()
{
    qDebug("Volume is changed");
    volume_lvl v_lvl;
    int v_level = ui->horizontalSlider->value();

    if (v_level < 1){
        v_lvl.labl = "UnMute";
        v_lvl.percent = DEFAULT_VOLUME;
    }else{
        v_lvl.labl = "Mute";
        v_lvl.percent = 0;
    }

    ui->pushButton->setText(v_lvl.labl);
    ui->horizontalSlider->setValue(v_lvl.percent);
    ui->label->setText(QString::number(v_lvl.percent) + "%");
    setSystemVolume(v_lvl.percent);
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // handle volume is change
    ui->label->setText(QString::number(value) + "%");

    // update label button
    if(value < 1){
        ui->pushButton->setText("UnMute");
    }else{
        ui->pushButton->setText("Mute");
    }

    setSystemVolume(value);
}


void MainWindow::on_comboBox_highlighted(int idx)
{
    QJsonArray ports = this->ports;
    QJsonObject p_selct = ports[idx].toObject();

    // QProcess process;
    // process.start("pactl", {"set-sink-port", idx});
    // process.waitForFinished();


    cout<<"ports: "<<p_selct["description"].toString().toStdString()<<endl;
}

