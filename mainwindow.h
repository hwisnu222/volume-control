#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QSlider>
#include <QLabel>

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
    void changeVolume();
    void toggleMute();

private:
    Ui::MainWindow *ui;
    QSlider *horizontalSlider;

    void getName();
    void setSystemVolume(int value);
    int getSystemVolume();
};
#endif // MAINWINDOW_H
