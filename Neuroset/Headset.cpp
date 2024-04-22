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
    currSessionTime = QDateTime::currentDateTime();
    sessionTimer = new QTimer(this);
    connect(sessionTimer, &QTimer::timeout, this, &Headset::incrementSessionDuration);
}

Headset::~Headset() {
    stopSimulation();
    delete simulationTimer;
}

void Headset::startSimulation(int rate) {
    emit sessionStart();
    this->sampleRate = rate;
    status = CONNECT;
    manageStages();
    sessionDuration = 0;
    sessionTimer->start(1000); //1s interval
}

void Headset::manageStages() {
    static QList<Frequency> sessionFrequencySets;
    int timePerStage = 6;
    int initialTime = 29;

    int remainingTime = initialTime - currentStage * timePerStage;

    emit updateCountdown(QString("0:%1").arg(remainingTime, 2, 10, QChar('0')));


    if (currentRunStatus == PAUSED) {
        qDebug() << "Session is paused. Waiting to resume.";
        waitingForResume = true;
        return;
    }

    if (currentStage < 4) {
        // Wait for 5 seconds before starting the baseline calculation for stage
        qDebug() << "Calculating baseline for stage" << currentStage + 1;
        QTimer::singleShot(5000, this, [this, remainingTime]() {

            //If the session's been stopped
            if(status == STOP){
                emit updateProgress();
                status = DISCONNECT;
                return;
            }

            if (getCurrentRunStatus() == PAUSED) {
                emit updateProgress();
                waitingForResume = true;
                qDebug() << "Session is paused. Waiting to resume before final calculations.";
                return;
            } else if (getCurrentRunStatus() == DISCONNECTED) {
                emit updateProgress();
                qDebug() << "Headset disconnected.";
                return;
            }

            std::vector<float> baselineFrequencies = calculateBaselines(5);
            qDebug() << "Baseline calculated for stage" << currentStage + 1;

            if(currentStage  == 0  ){
                this->initialFrequencies = baselineFrequencies;

            }

            qDebug() << "Starting concurrent treatment for stage" << currentStage + 1;
            emit treatmentStart();
            qDebug() << "Processing input waveform and delivering feedback...";
            startConcurrentTreatment();
            simulationTimer->start(1000 / 16); // update 1/16

            emit updateProgress();
            // 1s before next stage
            QTimer::singleShot(1000, this, [this]() {
                currentStage++;
                manageStages();  // recursion!
                emit treatmentEnd();
            });
            int newTime = remainingTime - 6; // Subtract 6 seconds after each stage
            emit updateCountdown(QString("0:%1").arg(newTime, 2, 10, QChar('0')));
        });
    } else if (currentStage == 4) {
        // Wait for 5 seconds before starting the final baseline calculation
        qDebug() << "Calculating final 5-second baseline";
        QTimer::singleShot(5000, this, [this, remainingTime]() {
            //If the session's been stopped
            if(status == STOP){
                emit updateProgress();
                status = DISCONNECT;
                return;
            }

            if (getCurrentRunStatus() == PAUSED) {
                emit updateProgress();
                waitingForResume = true;
                qDebug() << "Session is paused. Waiting to resume before final calculations.";
                return;
            } else if (getCurrentRunStatus() == DISCONNECTED) {
                emit updateProgress();
                qDebug() << "Headset disconnected.";
                return;
            }

            std::vector<float> baselineFrequencies = calculateBaselines(5);
            qDebug() << "Final baseline calculated";

            //Update final frequencies
            for(size_t x = 0; x < baselineFrequencies.size(); ++x){
                Frequency freq(this->initialFrequencies[x], baselineFrequencies[x]);
                sessionFrequencySets.append(freq);
            }
            //currSessionTime is the start time of the session
            QDateTime dummyEndTime = currSessionTime.addSecs(sessionDuration);
            Session createdSession(sessionFrequencySets, currSessionTime, dummyEndTime);

            emit sendSession(createdSession);

            QTimer::singleShot(5000, this, [this, remainingTime]() {
                emit updateProgress();
                qDebug() << "Final stage complete, stopping simulation.";
                emit sessionEnd();
                stopSimulation();
                emit updateCountdown(QString("0:%1").arg(remainingTime - 5, 2, 10, QChar('0'))); // Final 5 seconds
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
    qDebug() << "Session stopped";
    simulationTimer->stop();
    emit requestStop();
    emit waveformsUpdated();
    sessionTimer->stop();
    emit sessionEnd();
}

void Headset::onRunStatusChanged(RunStatus status) {
    currentRunStatus = status;
    if (status == PAUSED) {
        sessionTimer->stop();
    } else if (status == ACTIVE) {
        sessionTimer->start(1000);
    }
    if (status == ACTIVE && waitingForResume) {
        qDebug() << "Resuming stages after pause.";
        waitingForResume = false;
        manageStages();
    }
}

void Headset::setCurrSessionTime(QDateTime newDateTime){
    currSessionTime = newDateTime;
}

void Headset::incrementSessionDuration() {
    sessionDuration++;
}
