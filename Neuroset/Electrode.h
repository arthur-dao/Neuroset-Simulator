#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <vector>
#include "Waveform.h"

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
