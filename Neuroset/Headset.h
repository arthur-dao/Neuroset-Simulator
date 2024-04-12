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
    void startSimulation(int sampleRate, int durationSeconds);
    void setActiveElectrode(int electrodeIndex);
    const std::vector<float>& getActiveElectrodeWaveform(int activeElectrodeIndex) const;
    void calculateAllBaselines();
    void treatAllElectrodes();
    void stopSimulation();

signals:
    void waveformsUpdated();

private slots:
    void updateAllWaveforms();

private:
    std::vector<Electrode> electrodes;
    std::vector<std::vector<float>> allWaveforms;
    QTimer *waveformUpdateTimer;
    int activeElectrodeIndex;
    int sampleRate;
    int durationSeconds;
    static const int NUM_ELECTRODES = MAX_SIZE;
    static constexpr float FREQUENCY_OFFSET = 5.0f;

};

#endif // HEADSET_H
