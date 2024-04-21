#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      device(nullptr),
      headset(new Headset()),
      currentElectrodeIndex(0) {

    ui->setupUi(this);

    // graph
    customPlot = new QCustomPlot(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->plotWidget);
    layout->addWidget(customPlot);
    customPlot->addGraph();
    customPlot->xAxis->setVisible(true);
    customPlot->yAxis->setVisible(true);
    customPlot->xAxis2->setVisible(false);
    customPlot->yAxis2->setVisible(false);

    customPlot->xAxis->setLabel("Time (Data Point Index)");
    customPlot->yAxis->setLabel("Amplitude");


    connect(headset, &Headset::waveformsUpdated, this, &MainWindow::updateGraph, Qt::DirectConnection);
    connect(headset, &Headset::updateProgress, this, &MainWindow::updateProgress, Qt::DirectConnection);

    // Battery timer
    timeInterval = new QTimer(this);
    connect(timeInterval, &QTimer::timeout, this, &MainWindow::updateBattery);
    timeInterval->start(3000);

    device = new HandheldDevice(headset, 100);

    //Init menu
    menuSelection = ui->menuListWidget;
    currMenu = MAIN;
    updateList();
    ui->tabWidget->setCurrentIndex(0);
    ui->progressBar->setValue(0);

    //Adding eletrodes to combo box
    for(int i = 0; i < headset->getElectrodeNum(); i++){
        ui->electrodeCombo->addItem(QString("%2").arg(i + 1));
    }

    // Connect UI signals to slots
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->stopButton, &QPushButton::released, device, &HandheldDevice::stop);
    connect(ui->pauseButton, &QPushButton::released, device, &HandheldDevice::pause);
    connect(ui->startButton, &QPushButton::released, device, &HandheldDevice::resume);

    connect(ui->menuUp, &QPushButton::released, this, &MainWindow::navigateUp);
    connect(ui->menuDown, &QPushButton::released, this, &MainWindow::navigateDown);
    connect(ui->menuSelect, &QPushButton::released, this, &MainWindow::select);
    connect(ui->menuBack, &QPushButton::released, this, &MainWindow::back);

    connect(ui->powerButton, &QPushButton::released, device, &HandheldDevice::powerToggle);
    connect(ui->connectionButton, &QPushButton::released, device, &HandheldDevice::connectionToggle);
    connect(ui->electrodeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setActiveElectrodeIndex(int)));

//    connect(headset, &Headset::requestStop, device, &HandheldDevice::stop);
//    connect(device, &HandheldDevice::stopHeadset, headset, &Headset::stopSimulation);
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
            qv_x[i] = i; // time ig bc its the index of the data point
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

void MainWindow::navigateUp(){
    int currentIndex = menuSelection->currentRow();
    if (currentIndex <= 0) {
        // If at the top, move to the bottom
        menuSelection->setCurrentRow(menuSelection->count() - 1);
    } else if (currentIndex > 0) {
        menuSelection->setCurrentRow(currentIndex - 1);
    }
}

void MainWindow::navigateDown(){
    int currentIndex = menuSelection->currentRow();
    int lastIndex = menuSelection->count() - 1;
    if (currentIndex == lastIndex) {
        // If at the bottom, move to the top
        menuSelection->setCurrentRow(0);
    } else if (currentIndex < lastIndex) {
        menuSelection->setCurrentRow(currentIndex + 1);
    }
}

void MainWindow::select(){
    int currentIndex = menuSelection->currentRow();
    if(currMenu == MAIN){
        if(currentIndex == 0){
            ui->tabWidget->setCurrentIndex(1);
            device->createSession();
        }
        else if(currentIndex == 1){
            currMenu = SESSIONS;
            updateList();
        }
        else if(currentIndex == 2){
            currMenu = SETDATETIME;
            updateList();
        }
    }
    else if(currMenu == SESSIONS){
        device->uploadToPC(currentIndex);
    }
}

void MainWindow::back(){
    if(currMenu != MAIN){
        currMenu = MAIN;
        updateList();
    }
    else if(ui->tabWidget->currentIndex() == 1){
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::updateList(){
    menuSelection->clear();

    if(currMenu == MAIN){
        new QListWidgetItem(tr("Start Session"), menuSelection);
        new QListWidgetItem(tr("Session Log"), menuSelection);
        new QListWidgetItem(tr("Set Date and Time"), menuSelection);
    }
    else if(currMenu == SESSIONS){
        int n = 1;
        for(Session s : device->getSessions()){
            QString row = "SESSION #" + QString::number(n) + ": " + s.getStart().toString() + " - " + s.getEnd().toString();
            new QListWidgetItem(row, menuSelection);
            n++;
        }

    }
    else if(currMenu == SETDATETIME){

    }
}

void MainWindow::updateProgress(){
    int stage = headset->getStage();
    double percentage = static_cast<double>(stage) / 4 * 100;
    QString value = QString::number(percentage);
    ui->progressBar->setValue(static_cast<int>(percentage));
}
