#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <vector>
#include <random>

/**
 * @file Waveform.h
 * @brief Declaration of the Waveform class and the Band class.
 * 
 * This file contains the declaration of the Waveform class, which represents a waveform composed
 * of multiple frequency bands. It also declares the Band class, which represents a frequency band
 * with a specific frequency and amplitude.
 */

class Band {
public:
    float frequency;
    float amplitude;

    Band(float freq, float amp);
};

class Waveform {
private:
    std::vector<Band> bands;
    std::default_random_engine generator;
    std::normal_distribution<float> noise_distribution;

public:
    Waveform();
    void addBand(float frequency, float amplitude);
    std::vector<float> generateSignal(int sampleRate, int durationSeconds);
    float calculateDominantFrequency();
    void applyLENS(int sampleRate, float durationSeconds, float offsetFrequency);
    void updateBandFrequency(int index, float newFrequency);
    std::vector<Band> getBands() const { return bands; }

private:
    float generateSignalValue(float frequency, float amplitude, int sampleIndex, int sampleRate);
};

#endif // WAVEFORM_H
