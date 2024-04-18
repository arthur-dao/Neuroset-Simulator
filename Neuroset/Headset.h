#ifndef HEADSET_H
#define HEADSET_H

#include "defs.h"
#include "Electrode.h"
#include "TreatmentWorker.h"
#include <vector>
#include <QThread>
#include <QObject>
#include <QTimer>

class Headset : public QObject {
    Q_OBJECT

public slots:
    void stopSimulation();

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

signals:
    void waveformsUpdated();
    void requestStop();
    void updateProgress();

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
};

#endif // HEADSET_H
