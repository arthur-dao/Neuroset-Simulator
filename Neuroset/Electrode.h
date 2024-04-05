#ifndef ELECTRODE_H
#define ELECTRODE_H

class Electrode{

    public:
        Electrode();
        float calculateBaseline();
        void sendTreatment();

    private:
        float baselineAverageBefore;
        float baselineAverageAfter;

};

#endif // ELECTRODE_H
