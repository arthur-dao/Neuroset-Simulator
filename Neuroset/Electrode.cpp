#include "Electrode.h"

Electrode::Electrode(int siteNum)
    : siteNum(siteNum), baselineAverageBefore(0.0f), baselineAverageAfter(0.0f) {
    if (siteNum % 7 == 0) {
        frequencies = {10.0f, 20.0f, 3.0f, 40.0f};
        amplitudes = {1.0f, 0.5f, 1.5f, 0.3f};
    }
}

Electrode::~Electrode() {}

std::vector<float> Electrode::simulateEEGSignal(int sampleRate, int durationSeconds, const std::vector<float>& frequencies, const std::vector<float>& amplitudes) {
    std::vector<float> signal(sampleRate * durationSeconds, 0.0f);
    for (size_t i = 0; i < frequencies.size(); ++i) {
        float frequency = frequencies[i];
        float amplitude = amplitudes.size() > i ? amplitudes[i] : 1.0f; // Default amplitude if not specified
        for (int j = 0; j < signal.size(); ++j) {
            signal[j] += generateSignalValue(frequency, amplitude, j, sampleRate);
        }
    }
    return signal;
}

float Electrode::generateSignalValue(float frequency, float amplitude, int sampleIndex, int sampleRate) {
    float t = static_cast<float>(sampleIndex) / sampleRate;
    float value = amplitude * std::sin(2.0f * M_PI * frequency * t);

    value += ((rand() % 100) / 100.0f) * 0.2f - 0.1f;

    return value;
}
