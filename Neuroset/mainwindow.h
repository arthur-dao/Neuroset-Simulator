#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HandheldDevice.h"
#include "Headset.h"
#include "Electrode.h"
#include "qcustomplot.h"
#include "defs.h"
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @file MainWindow.h
 * @brief Declaration of the MainWindow class.
 * 
 * This file contains the declaration of the MainWindow class, which represents
 * the main window interface for the Neureset application.
 * It includes methods for updating battery status, displaying EEG data graphically,
 * and handling user interactions with the menu.
 */

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

    void updateList();
    void setDateTime();

private slots:
    void updateBattery();
    void updateGraph();
    void setActiveElectrodeIndex(int index);

    void navigateUp();
    void navigateDown();
    void select();
    void back();

    void updateProgress();

    void onUpdateCountdown(const QString& time);

};
#endif // MAINWINDOW_H
