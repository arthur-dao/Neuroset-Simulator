#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    device = new HandheldDevice();

    timeInterval = new QTimer(this);
    timeInterval->start(10000);
    connect(timeInterval, SIGNAL(timeout()), device, SLOT(reduceBattery()));

    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->stopButton, SIGNAL(released()), device, SLOT(stop()));
    connect(ui->pauseButton, SIGNAL(released()), device, SLOT(pause()));
    connect(ui->startButton, SIGNAL(released()), device, SLOT(resume()));

    connect(ui->menuUp, SIGNAL(released()), device, SLOT(navigateUp()));
    connect(ui->menuDown, SIGNAL(released()), device, SLOT(navigateDown()));

    connect(ui->powerButton, SIGNAL(released()), device, SLOT(powerToggle()));

}

MainWindow::~MainWindow() {
    delete ui;
}
