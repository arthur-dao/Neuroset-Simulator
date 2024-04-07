#ifndef HEADSET_H
#define HEADSET_H

#include "defs.h"
#include "Electrode.h"

#include "list"

using namespace std;

class Headset{

    public:
        Headset();
        void calculateAllBaselines();
        void treatAllElectrodes();

    private:
        float frquencyOffset;

        list<Electrode> electrodes;

        static int size;
};

#endif // HEADSET_H
