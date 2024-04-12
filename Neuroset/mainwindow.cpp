#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      device(nullptr),
      headset(new Headset()),
      currentElectrodeIndex(0) {

    ui->setupUi(this);

    customPlot = new QCustomPlot(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->plotWidget);
    layout->addWidget(customPlot);
    customPlot->addGraph();

    connect(headset, &Headset::waveformsUpdated, this, &MainWindow::updateGraph, Qt::DirectConnection);

    // Battery timer
    timeInterval = new QTimer(this);
    connect(timeInterval, &QTimer::timeout, this, &MainWindow::updateBattery);
    timeInterval->start(3000);

    device = new HandheldDevice(headset, 100);

    // Connect UI signals to slots
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->stopButton, &QPushButton::released, device, &HandheldDevice::stop);
    connect(ui->pauseButton, &QPushButton::released, device, &HandheldDevice::pause);
    connect(ui->startButton, &QPushButton::released, device, &HandheldDevice::resume);
    connect(ui->menuUp, &QPushButton::released, device, &HandheldDevice::navigateUp);
    connect(ui->menuDown, &QPushButton::released, device, &HandheldDevice::navigateDown);
    connect(ui->powerButton, &QPushButton::released, device, &HandheldDevice::powerToggle);

    connect(ui->electrodeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveElectrodeIndex(int)));
}

MainWindow::~MainWindow() {
    delete headset;
    delete device;
    delete ui;
}

void MainWindow::updateBattery() {
    QString battery = QString::number(device->getBattery()) + "%";
    ui->batteryIndicator->setText(battery);
}

void MainWindow::updateGraph() {
    std::vector<float> waveform = headset->getActiveElectrodeWaveform(currentElectrodeIndex);
    if (!waveform.empty()) {
        QVector<double> qv_x(waveform.size()), qv_y(waveform.size());
        for (size_t i = 0; i < waveform.size(); ++i) {
            qv_x[i] = i; // The x-axis can represent the time or index of the data points
            qv_y[i] = waveform[i]; // The y-axis represents the amplitude of the waveform
//            qDebug() << "Point" << i << ": x =" << qv_x[i] << ", y =" << qv_y[i];
        }
        customPlot->graph(0)->setData(qv_x, qv_y);
        customPlot->graph(0)->rescaleAxes();
        customPlot->replot();
    } else {
        qDebug() << "Waveform data is empty.";
        customPlot->graph(0)->data()->clear();
        customPlot->replot();
    }
}

void MainWindow::setActiveElectrodeIndex(int index) {
    if (index >= 0 && index < 20) {
        currentElectrodeIndex = index;
        qDebug() << "Active electrode index set to:" << currentElectrodeIndex;
        updateGraph();
    }
}


