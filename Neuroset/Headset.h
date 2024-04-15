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

public:
    explicit Headset(QObject *parent = nullptr);
    ~Headset();
    int getElectrodeNum() { return NUM_ELECTRODES; }
    void startSimulation(int sampleRate);
    void stopSimulation();
    std::vector<float> calculateBaselines(int durationSeconds);
    void startConcurrentTreatment();
    const std::vector<float>& getActiveElectrodeWaveform(int activeElectrodeIndex) const;
    void updateAllWaveforms();

signals:
    void waveformsUpdated();
    void requestStop();

private slots:
    void updateSimulationWaveforms();

private:
    std::vector<Electrode> electrodes;
    std::vector<std::vector<float>> allWaveforms;
    QTimer *simulationTimer;
    int sampleRate;
    static const int NUM_ELECTRODES = MAX_SIZE;
};

#endif // HEADSET_H
