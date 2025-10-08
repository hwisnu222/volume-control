#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_comboBox_highlighted(int index);

private:
    Ui::MainWindow *ui;
    QSlider *horizontalSlider;
    QJsonArray ports;

    void getName();
    void setSystemVolume(int value);
    int getSystemVolume();
    void getDevices();
    void setOutput(int idx);
};
#endif // MAINWINDOW_H
