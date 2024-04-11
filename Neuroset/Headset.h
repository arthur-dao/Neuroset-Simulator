#ifndef HEADSET_H
#define HEADSET_H

#include "defs.h"
#include "Electrode.h"
#include <vector>

class Headset {

    public:
        Headset();
        ~Headset() = default;

        void calculateAllBaselines();
        void treatAllElectrodes();
        void startSimulation();

    private:
        static const int NUM_ELECTRODES = MAX_SIZE;
        static constexpr float FREQUENCY_OFFSET = 5.0f;

        std::vector<Electrode> electrodes;
};

#endif // HEADSET_H
