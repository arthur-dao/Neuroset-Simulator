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

    void setActiveElectrode(int electrodeIndex);
    void addFrequency(float initial, float final);
    void selectElectrode(int electrodeIndex);

private:
    QDateTime startDateTime;
    QDateTime endDateTime;
    QList<Frequency> frequencies;
    Headset headset;
};

#endif // SESSION_H
