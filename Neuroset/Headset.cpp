#include "Headset.h"
#include "Session.h"
#include <QDebug>
#include <QTimer>

Headset::Headset(QObject *parent)
    : QObject(parent), simulationTimer(new QTimer(this)), sampleRate(0) {
    electrodes.reserve(NUM_ELECTRODES);
    allWaveforms.resize(NUM_ELECTRODES);

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        electrodes.emplace_back(Electrode(i));
    }
    status = DISCONNECT;
    connect(simulationTimer, &QTimer::timeout, this, &Headset::updateSimulationWaveforms);
}

Headset::~Headset() {
    stopSimulation();
    delete simulationTimer;
}

void Headset::startSimulation(int rate) {
    this->sampleRate = rate;
    status = CONNECT;
    manageStages();
}

void Headset::manageStages() {
    static QList<Frequency> sessionFrequencySets;

    if (currentStage < 4) {
        // Wait for 5 seconds before starting the baseline calculation for stage
        qDebug() << "Calculating baseline for stage" << currentStage + 1;
        QTimer::singleShot(5000, this, [this]() {

            //If the session's been stopped
            if(status == STOP){
                emit updateProgress();
                status = DISCONNECT;
                return;
            }

            std::vector<float> baselineFrequencies = calculateBaselines(5);
            qDebug() << "Baseline calculated for stage" << currentStage + 1;

            if(currentStage  == 0  ){
                this->initialFrequencies = baselineFrequencies;

            }

            qDebug() << "Starting concurrent treatment for stage" << currentStage + 1;
            startConcurrentTreatment();
            simulationTimer->start(1000 / 16); // update 1/16

            emit updateProgress();
            // 1s before next stage
            QTimer::singleShot(1000, this, [this]() {
                currentStage++;
                manageStages();  // recursion!
            });
        });
    } else if (currentStage == 4) {
        // Wait for 5 seconds before starting the final baseline calculation
        qDebug() << "Calculating final 5-second baseline";
        QTimer::singleShot(5000, this, [this]() {
            //If the session's been stopped
            if(status == STOP){
                emit updateProgress();
                status = DISCONNECT;
                return;
            }

            if(status == DISCONNECT){
                qInfo("Headset has been disconnected");
                return;
            }

            std::vector<float> baselineFrequencies = calculateBaselines(5);
            qDebug() << "Final baseline calculated";

            //Update final frequencies
            for(size_t x = 0; x < baselineFrequencies.size(); ++x){
                Frequency freq(this->initialFrequencies[x], baselineFrequencies[x]);
                sessionFrequencySets.append(freq);
            }

            //Dummy, Timmy fill
            QDateTime dummyStartTime = QDateTime::currentDateTime();
            QDateTime dummyEndTime = dummyStartTime.addSecs(300);

            Session createdSession(sessionFrequencySets, dummyStartTime, dummyEndTime);

            emit sendSession(createdSession);

            QTimer::singleShot(5000, this, [this]() {
                emit updateProgress();
                qDebug() << "Final stage complete, stopping simulation.";
                stopSimulation();
            });
        });
    }
}

std::vector<float> Headset::calculateBaselines(int durationSeconds) {
    std::vector<float> baselineFrequencies;
    baselineFrequencies.reserve(NUM_ELECTRODES);

    for (auto& electrode : electrodes) {
        auto waveform = electrode.generateWaveform(sampleRate, durationSeconds);
        float freq = electrode.calculateDominantFrequency();
        baselineFrequencies.push_back(freq);
        qDebug() << "Baseline frequency for electrode" << electrode.getSiteNum() + 1 << ":" << freq;
    }

    return baselineFrequencies;
}

void Headset::startConcurrentTreatment() {
    for (auto& electrode : electrodes) {
        QThread* thread = new QThread;
        TreatmentWorker* worker = new TreatmentWorker(&electrode, sampleRate);
        worker->moveToThread(thread);
        connect(thread, &QThread::started, worker, &TreatmentWorker::process);
        connect(worker, &TreatmentWorker::finished, thread, &QThread::quit);
        connect(worker, &TreatmentWorker::finished, worker, &TreatmentWorker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }
}

const std::vector<float>& Headset::getActiveElectrodeWaveform(int activeElectrodeIndex) const {
    return allWaveforms[activeElectrodeIndex];
}

void Headset::updateAllWaveforms() {
    for (size_t i = 0; i < electrodes.size(); ++i) {
        allWaveforms[i] = electrodes[i].generateWaveform(sampleRate, 60);
    }
    emit waveformsUpdated();
}

void Headset::updateSimulationWaveforms() {
    for (auto& electrode : electrodes) {
        allWaveforms[electrode.getSiteNum()] = electrode.generateWaveform(sampleRate, 1);
    }
    emit waveformsUpdated();
}

void Headset::stopSimulation() {
    currentStage = 0;
    status = STOP;
    simulationTimer->stop();
    emit requestStop();
    emit waveformsUpdated();
}
