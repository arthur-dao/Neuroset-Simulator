#ifndef FREQUENCY_H
#define FREQUENCY_H

class Frequency{
    public:
        Frequency(float initial, float final);
        ~Frequency();

        float getInitialFrequency() const;
        float getFinalFrequency() const;

    private:
        float initialFrequency;
        float finalFrequency;
};

#endif // FREQUENCY_H
