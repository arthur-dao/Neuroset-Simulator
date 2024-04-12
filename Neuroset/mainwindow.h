#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HandheldDevice.h"
#include "Headset.h"
#include "Electrode.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sessionStarted();
    void sessionStopped();

private:
    Ui::MainWindow *ui;
    HandheldDevice* device;
    Headset* headset;
    QTimer* timeInterval;

    QCustomPlot *customPlot;
    QTimer *dataTimer;
    int currentElectrodeIndex;

private slots:
    void updateBattery();
    void updateGraph();
    void setActiveElectrodeIndex(int index);
};
#endif // MAINWINDOW_H
