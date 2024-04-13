#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HandheldDevice.h"
#include "qcustomplot.h"
#include <QListWidget>
#include <QThread>

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

    QListWidget *menuSelection;
    CurrentMenu currMenu;

    QThread deviceThread;

    void updateList();


private slots:
    void updateBattery();
    void updateGraph();
    void setActiveElectrodeIndex(int index);

    void navigateUp();
    void navigateDown();
    void select();
    void back();
};
#endif // MAINWINDOW_H
