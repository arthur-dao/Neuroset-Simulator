#ifndef SESSION_H
#define SESSION_H

#include <QDateTime>
#include <QList>
#include "Frequency.h"
#include "Headset.h"

class Session {
public:
    Session();

    void startSession();
    void analyzeData();
    void endSession();

    void addFrequency(float initial, float final);

private:
    QDateTime startDateTime;
    QDateTime endDateTime;
    QList<Frequency> frequencies;
    Headset headset;
};

#endif // SESSION_H
