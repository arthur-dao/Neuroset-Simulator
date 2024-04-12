#include "Electrode.h"
#include "QDebug"

Electrode::Electrode(int siteNum)
    : siteNum(siteNum), baselineAverageBefore(0.0f), baselineAverageAfter(0.0f) {
    frequencies = {10.0f + siteNum * 0.2f, 20.0f + siteNum * 0.1f, 3.0f + siteNum * 0.05f, 40.0f + siteNum * 0.15f};
    amplitudes = {1.0f, 0.5f + siteNum * 0.02f, 1.5f - siteNum * 0.01f, 0.3f + siteNum * 0.05f};
}

Electrode::~Electrode() {}

std::vector<float> Electrode::generateWaveform(int sampleRate, int durationSeconds) {
    return simulateEEGSignal(sampleRate, durationSeconds);
}

std::vector<float> Electrode::simulateEEGSignal(int sampleRate, int durationSeconds) {
    std::vector<float> signal(sampleRate * durationSeconds, 0.0f);
    for (size_t i = 0; i < frequencies.size(); ++i) {
        float frequency = frequencies[i];
        float amplitude = amplitudes[i];
        for (size_t j = 0; j < signal.size(); ++j) {
            signal[j] += generateSignalValue(frequency, amplitude, j, sampleRate);
        }
    }
    return signal;
}


float Electrode::generateSignalValue(float frequency, float amplitude, int sampleIndex, int sampleRate) {
    float t = static_cast<float>(sampleIndex) / sampleRate;
    float value = amplitude * std::sin(2.0f * M_PI * frequency * t);

    value += static_cast<float>(rand() % 100) / 500.0f;

    return value;
}
