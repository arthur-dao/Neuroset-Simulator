#include "Headset.h"
#include <QDebug>
#include <QTimer>

Headset::Headset(QObject *parent)
    : QObject(parent), waveformUpdateTimer(new QTimer(this)), activeElectrodeIndex(0) {
    electrodes.reserve(NUM_ELECTRODES);
    allWaveforms.resize(NUM_ELECTRODES);

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        electrodes.emplace_back(Electrode(i));
    }

    // Connect the timer's timeout signal to the updateAllWaveforms slot.
    connect(waveformUpdateTimer, &QTimer::timeout, this, &Headset::updateAllWaveforms);
}

Headset::~Headset() {
    delete waveformUpdateTimer;
}

void Headset::startSimulation(int sampleRate, int durationSeconds) {
    if (sampleRate <= 0) {
        qDebug() << "Invalid sample rate provided. Simulation not started.";
        return;
    }

    this->sampleRate = sampleRate;
    this->durationSeconds = durationSeconds;

    updateAllWaveforms();

    int interval = 1000 / sampleRate;
    waveformUpdateTimer->start(interval);  // Avoid division by zero
    qDebug() << "Simulation started with sample rate:" << sampleRate << " duration seconds:" << durationSeconds << " at interval:" << interval << "ms";
}

void Headset::updateAllWaveforms() {
//    qDebug() << "Updating waveforms";
    for (size_t i = 0; i < electrodes.size(); ++i) {
        allWaveforms[i] = electrodes[i].generateWaveform(sampleRate, durationSeconds);
//        QVector<double> qVectorWaveform(allWaveforms[i].begin(), allWaveforms[i].end());
//        qDebug() << "Electrode" << i << "Waveform:" << qVectorWaveform;
    }
//    qDebug() << "Emitting waveformsUpdated before";
    emit waveformsUpdated();
//    qDebug() << "Emitting waveformsUpdated after";

}

void Headset::setActiveElectrode(int electrodeIndex) {
    if (electrodeIndex >= 0 && static_cast<size_t>(electrodeIndex) < electrodes.size()) {
        activeElectrodeIndex = electrodeIndex;
//         emit signal?
    }
}

const std::vector<float>& Headset::getActiveElectrodeWaveform(int activeElectrodeIndex) const {
    QVector<double> qVectorWaveform(allWaveforms[0].begin(), allWaveforms[0].end());
    qDebug() << "Electrode" << 0 << "Waveform:" << qVectorWaveform;
    qDebug() << "Retrieving waveform for electrode" << activeElectrodeIndex << "size:" << allWaveforms[activeElectrodeIndex].size();
    return allWaveforms[activeElectrodeIndex];
}

void Headset::stopSimulation() {
    waveformUpdateTimer->stop();
    allWaveforms.clear();
    allWaveforms.resize(21);
    qDebug() << "Simulation stopped.";
    emit waveformsUpdated();
}

void Headset::calculateAllBaselines() {

}

void Headset::treatAllElectrodes() {

}
