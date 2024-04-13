#ifndef HEADSET_H
#define HEADSET_H

#include "defs.h"
#include "Electrode.h"
#include <vector>
#include <QObject>
#include <QTimer>

class Headset : public QObject {
    Q_OBJECT

public:
    explicit Headset(QObject *parent = nullptr);
    ~Headset();
    int getElectrodeNum() { return NUM_ELECTRODES; };
    void startSimulation(int sampleRate);
    void stopSimulation();
    std::vector<float> calculateBaselines(int durationSeconds);
    void applyTreatmentToAllElectrodes();
    void applyTreatmentToOneElectrode(int index);
    void setActiveElectrode(int electrodeIndex);
    const std::vector<float>& getActiveElectrodeWaveform(int activeElectrodeIndex) const;
    void updateSingleElectrodeWaveform(int index);

signals:
    void waveformsUpdated();
    void requestStop();

private slots:
    void updateAllWaveforms();
    void handleTreatmentCompletion();
    void updateSimulationWaveforms();

private:
    std::vector<Electrode> electrodes;
    std::vector<std::vector<float>> allWaveforms;
    QTimer *treatmentTimer;
    QTimer *simulationTimer;
    int activeElectrodeIndex;
    int sampleRate;
    int durationSeconds;
    static const int NUM_ELECTRODES = MAX_SIZE;
};

#endif // HEADSET_H
