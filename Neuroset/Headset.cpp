#include "Headset.h"
#include <QDebug>
#include <QTimer>

Headset::Headset(QObject *parent)
    : QObject(parent), treatmentTimer(new QTimer(this)), simulationTimer(new QTimer(this)), activeElectrodeIndex(0) {
    electrodes.reserve(NUM_ELECTRODES);
    allWaveforms.resize(NUM_ELECTRODES);

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        electrodes.emplace_back(Electrode(i));
    }

    connect(treatmentTimer, &QTimer::timeout, this, &Headset::handleTreatmentCompletion);
    connect(simulationTimer, &QTimer::timeout, this, &Headset::updateSimulationWaveforms);
}

Headset::~Headset() {
    delete treatmentTimer;
    delete simulationTimer;
}

void Headset::startSimulation(int sampleRate) {
    this->sampleRate = sampleRate;
    std::vector<float> baselineFrequencies = calculateBaselines(60); // 60 seconds for baseline generated instantly

    qDebug() << "Baselines calculated. Starting treatment.";
    applyTreatmentToAllElectrodes();
    simulationTimer->start(1000 / 16);
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

void Headset::applyTreatmentToAllElectrodes() {
    activeElectrodeIndex = 0;
    applyTreatmentToOneElectrode(activeElectrodeIndex);
}

void Headset::applyTreatmentToOneElectrode(int index) {
    if (index < NUM_ELECTRODES) {
        qDebug() << "Starting treatment for electrode" << index + 1;
        const float lensTreatmentDurationSeconds = 1.0f; // Treatment lasts for 1 second
        const float lensOffsetFrequency = 5.0f; // Frequency offset applied during LENS treatment
        electrodes[index].applyLENS(sampleRate, lensTreatmentDurationSeconds, lensOffsetFrequency);
        treatmentTimer->start(1000 / 16);  // Trigger recalculation every 1/16 second within the 1-second treatment
    } else {
        qDebug() << "All electrodes treated.";
        stopSimulation();
    }
}

void Headset::handleTreatmentCompletion() {
    static int timesCalled = 0;
    timesCalled++;

    if (timesCalled < 16) {
        electrodes[activeElectrodeIndex].applyLENS(sampleRate, 1.0f / 16, 5.0f);
        updateSingleElectrodeWaveform(activeElectrodeIndex);
        emit waveformsUpdated();
    } else {
        // After 16 calls (1 second), move to the next electrode
        timesCalled = 0;
        treatmentTimer->stop();
        activeElectrodeIndex++;
        if (activeElectrodeIndex < NUM_ELECTRODES) {
            applyTreatmentToOneElectrode(activeElectrodeIndex);
        } else {
            calculateBaselines(1);
            qDebug() << "Completed treatment for all electrodes.";
            stopSimulation();
        }
    }
}

void Headset::updateSingleElectrodeWaveform(int index) {
    if (index >= 0 && index < static_cast<int>(electrodes.size())) {
        allWaveforms[index] = electrodes[index].generateWaveform(sampleRate, 1);
    }
}


void Headset::updateAllWaveforms() {
    for (size_t i = 0; i < electrodes.size(); ++i) {
        allWaveforms[i] = electrodes[i].generateWaveform(sampleRate, 60);
    }
    emit waveformsUpdated();
}

void Headset::setActiveElectrode(int electrodeIndex) {
    if (electrodeIndex >= 0 && static_cast<size_t>(electrodeIndex) < electrodes.size()) {
        activeElectrodeIndex = electrodeIndex;
        qDebug() << "Active electrode set to:" << electrodeIndex;
    }
}

const std::vector<float>& Headset::getActiveElectrodeWaveform(int activeElectrodeIndex) const {
    return allWaveforms[activeElectrodeIndex];
}

void Headset::stopSimulation() {
    treatmentTimer->stop();
    simulationTimer->stop();
    emit requestStop();
    emit waveformsUpdated();
}

void Headset::updateSimulationWaveforms() {
    int updateDurationSeconds = 1;
    for (size_t i = 0; i < electrodes.size(); ++i) {
        allWaveforms[i] = electrodes[i].generateWaveform(sampleRate, updateDurationSeconds);
    }
    emit waveformsUpdated();
}

