#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

using namespace std;

#define DEFAULT_VOLUME 10

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
    if(value < 1){
        ui->pushButton->setText("UnMute");
    }else{
        ui->pushButton->setText("Mute");
    }

    getDevices();


    // add event valueChanged for slider
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::changeVolume);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::toggleMute);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeVolume(){
    // handle volume is change
    int value = ui->horizontalSlider->value();
    ui->label->setText(QString::number(value) + "%");

    // update label button
    if(value < 1){
        ui->pushButton->setText("UnMute");
    }else{
        ui->pushButton->setText("Mute");
    }

    setSystemVolume(value);

}

void MainWindow::toggleMute(){
    int v_level = ui->horizontalSlider->value();

    if (v_level < 1){
        ui->pushButton->setText("UnMute");
        ui->horizontalSlider->setValue(DEFAULT_VOLUME);
        ui->label->setText(QString::number(DEFAULT_VOLUME) + "%");

        setSystemVolume(DEFAULT_VOLUME);
    }else{
        ui->pushButton->setText("Mute");
        ui->horizontalSlider->setValue(0);
        ui->label->setText(QString::number(0) + "%");

        setSystemVolume(0);
    }

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
    // cout<<json.toStdString()<<endl;

    QJsonDocument json = QJsonDocument::fromJson(out);
    QJsonValue modules = json["modules"];

    QStringList lists;
    // QByteArray m = json["modules"];

    for(int i=0;i<modules.size();i++){
        lists.append(modules[i].get("name"));
    }


    ui->comboBox->addItems(lists);
}
