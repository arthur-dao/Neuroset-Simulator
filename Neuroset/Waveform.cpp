#include "Waveform.h"
#include <cmath>
#include <chrono>

Band::Band(float freq, float amp) : frequency(freq), amplitude(amp) {}

Waveform::Waveform() : generator(std::chrono::system_clock::now().time_since_epoch().count()), noise_distribution(0.0f, 0.1f) {}

void Waveform::addBand(float frequency, float amplitude) {
    bands.emplace_back(frequency, amplitude);
}

std::vector<float> Waveform::generateSignal(int sampleRate, int durationSeconds) {
    std::vector<float> signal(sampleRate * durationSeconds, 0.0f);
    for (const Band& band : bands) {
        for (size_t i = 0; i < signal.size(); ++i) {
            signal[i] += generateSignalValue(band.frequency, band.amplitude, i, sampleRate);
        }
    }
    return signal;
}

float Waveform::calculateDominantFrequency() {
    float numerator = 0.0f;
    float denominator = 0.0f;
    for (const Band& band : bands) {
        float amplitudeSquared = band.amplitude * band.amplitude;
        numerator += band.frequency * amplitudeSquared;
        denominator += amplitudeSquared;
    }
    return numerator / denominator;
}

void Waveform::applyLENS(int sampleRate, float durationSeconds, float offsetFrequency) {
    int lensIterations = static_cast<int>(sampleRate * durationSeconds);
    int updatesPerSecond = 1;
    int updateInterval = sampleRate / updatesPerSecond;

    for (int i = 0; i < lensIterations; ++i) {
        if (i % updateInterval == 0) {
            for (auto& band : bands) {
                band.frequency += offsetFrequency;
            }
        }
    }
}

void Waveform::updateBandFrequency(int index, float newFrequency) {
    if (static_cast<size_t>(index) < bands.size()) {
        bands[index].frequency = newFrequency;
    }
}

float Waveform::generateSignalValue(float frequency, float amplitude, int sampleIndex, int sampleRate) {
    float t = static_cast<float>(sampleIndex) / sampleRate;
    return amplitude * std::sin(2.0f * M_PI * frequency * t) + noise_distribution(generator);
}
