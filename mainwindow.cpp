#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define DEFAULT_VOLUME 30

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
    if(value == 0){
        ui->pushButton->setText("UnMute");
    }else{
        ui->pushButton->setText("Mute");
    }


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
    if(value == 0){
        ui->pushButton->setText("UnMute");
    }else{
        ui->pushButton->setText("Mute");
    }

    setSystemVolume(value);

}

void MainWindow::toggleMute(){
    if (ui->pushButton->text().toStdString() == "Mute"){
        ui->pushButton->setText("UnMute");
        ui->horizontalSlider->setValue(0);
        ui->label->setText(QString::number(0) + "%");

        setSystemVolume(DEFAULT_VOLUME);
    }else{
        ui->pushButton->setText("Mute");
        ui->horizontalSlider->setValue(DEFAULT_VOLUME);
        ui->label->setText(QString::number(DEFAULT_VOLUME) + "%");
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
