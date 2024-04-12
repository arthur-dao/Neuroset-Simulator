#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <vector>
#include <cmath>
#include <cstdlib>

class Electrode{

    public:
        Electrode(int siteNum);
        ~Electrode();

        std::vector<float> generateWaveform(int sampleRate, int durationSeconds);
        float calculateBaseline();
        void sendTreatment(float offsetFrequency, int duration, int intervals);

        float getBaselineAverageBefore() const { return baselineAverageBefore; }
        float getBaselineAverageAfter() const {return baselineAverageAfter; }


    private:
        int siteNum;
        float baselineAverageBefore;
        float baselineAverageAfter;

        std::vector<float> frequencies;
        std::vector<float> amplitudes;

        std::vector<float> simulateEEGSignal(int sampleRate, int durationSeconds);
        float generateSignalValue(float frequency, float amplitude, int sampleIndex, int sampleRate);
};

#endif // ELECTRODE_H
