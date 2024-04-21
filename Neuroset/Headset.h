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
    std::vector<float> initialFrequencies; // Store initial frequencies

    QDateTime currSessionTime;

    RunStatus currentRunStatus;
    bool waitingForResume = false;

};

#endif // HEADSET_H
