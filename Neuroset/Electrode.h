#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <vector>
#include "Waveform.h"

/**
 * @file Electrode.h
 * @brief Declaration of the Electrode class.
 * 
 * This file contains the declaration of the Electrode class, which represents
 * an electrode used in neurofeedback therapy. Each electrode has properties
 * such as site number and waveform, and methods to apply LENS treatment, 
 * calculate dominant frequency, generate waveform, and update bands.
 */

class Electrode {
public:
    Electrode(int siteNum);
    ~Electrode();

    void applyLENS(int sampleRate, float durationSeconds, float offsetFrequency);
    float calculateDominantFrequency();
    std::vector<float> generateWaveform(int sampleRate, int durationSeconds);
    std::vector<Band> getBands() const;
    void updateBand(int index, float newFrequency);

    int getSiteNum() const { return siteNum; }

private:
    int siteNum;
    Waveform waveform;

    void initializeWaveform();
};

#endif // ELECTRODE_H
