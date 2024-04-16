#include "Electrode.h"
#include "QDebug"

Electrode::Electrode(int siteNum) : siteNum(siteNum) {
    initializeWaveform();
}

Electrode::~Electrode() {}

void Electrode::initializeWaveform() {
    waveform.addBand(3.0 + siteNum * 0.05, 1.5 - siteNum * 0.01); // Delta
    waveform.addBand(4.0 + siteNum * 0.05, 1.0); // Theta
    waveform.addBand(10.0 + siteNum * 0.2, 1.0); // Alpha
    waveform.addBand(20.0 + siteNum * 0.1, 0.5 + siteNum * 0.02); // Beta
}

float Electrode::calculateDominantFrequency() {
    return waveform.calculateDominantFrequency();
}

void Electrode::applyLENS(int sampleRate, float durationSeconds, float offsetFrequency) {
    waveform.applyLENS(sampleRate, durationSeconds, offsetFrequency);
//    waveform.generateSignal(sampleRate, durationSeconds);
}

std::vector<float> Electrode::generateWaveform(int sampleRate, int durationSeconds) {
    return waveform.generateSignal(sampleRate, durationSeconds);
}

std::vector<Band> Electrode::getBands() const {
    return waveform.getBands();
}

void Electrode::updateBand(int index, float newFrequency) {
    waveform.updateBandFrequency(index, newFrequency);
}
