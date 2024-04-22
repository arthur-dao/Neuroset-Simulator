#ifndef FREQUENCY_H
#define FREQUENCY_H

/**
 * @file Frequency.h
 * @brief Declaration of the Frequency class.
 * 
 * This file contains the declaration of the Frequency class, which represents
 * a frequency range defined by its initial and final values.
 */

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
