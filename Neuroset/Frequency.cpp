#include "Frequency.h"

Frequency::Frequency(float initial, float final)
    : initialFrequency(initial), finalFrequency(final) {}

float Frequency::getInitialFrequency() const {
    return initialFrequency;
}

float Frequency::getFinalFrequency() const {
    return finalFrequency;
}
