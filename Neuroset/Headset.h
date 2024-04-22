#ifndef HEADSET_H
#define HEADSET_H

#include "defs.h"
#include "Electrode.h"
#include "TreatmentWorker.h"
#include "Session.h"
#include <vector>
#include <QThread>
#include <QObject>
#include <QTimer>
#include <QDateTime>

/**
 * @file Headset.h
 * @brief Declaration of the Headset class.
 * 
 * This file contains the declaration of the Headset class, which represents
 * the Neureset EEG headset used for direct neurofeedback therapy.
 * It includes methods for managing treatment stages, starting and stopping simulation,
 * and updating electrode waveforms.
 */

class Headset : public QObject {
    Q_OBJECT

public slots:
    void stopSimulation();
    void onRunStatusChanged(RunStatus runStatus);


public:
    explicit Headset(QObject *parent = nullptr);
    ~Headset();
    int getElectrodeNum() { return NUM_ELECTRODES; }
    int getStage() { return currentStage; }
    void startSimulation(int sampleRate);
    std::vector<float> calculateBaselines(int durationSeconds);
    void startConcurrentTreatment();
    const std::vector<float>& getActiveElectrodeWaveform(int activeElectrodeIndex) const;
    void updateAllWaveforms();

    void manageStages();
    void setCurrSessionTime(QDateTime);

    QTimer* getSimulationTimer() {return simulationTimer; };

    RunStatus getCurrentRunStatus() const { return currentRunStatus; }


signals:
    void waveformsUpdated();
    void requestStop();
    void updateProgress();
    void sendSession(const Session& session);
    void treatmentStart();
    void treatmentEnd();
    void sessionStart();
    void sessionEnd();
    void contactLostStart();
    void contactLostEnd();
    void updateCountdown(QString time);

private slots:
    void updateSimulationWaveforms();

private:
    std::vector<Electrode> electrodes;
    std::vector<std::vector<float>> allWaveforms;
    QTimer *simulationTimer;
    std::vector<QThread*> activeThreads;
    int sampleRate;
    static const int NUM_ELECTRODES = MAX_SIZE;
    int currentStage = 0;
    const int totalStages = 5;
    Status status;
    std::vector<float> initialFrequencies;

    QDateTime currSessionTime;

    RunStatus currentRunStatus;
    bool waitingForResume = false;
    QTimer *sessionTimer;
    int sessionDuration;
    void incrementSessionDuration();

};

#endif // HEADSET_H
