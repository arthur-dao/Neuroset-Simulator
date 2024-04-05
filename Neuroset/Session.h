#ifndef SESSION_H
#define SESSION_H

#include "Frequency.h"

#include "QDateTime"
#include "QList"

class Session{

    public:
        Session();
        void addFrequency(float, float);

    private:
        QDateTime startDateTime;
        QList<Frequency> frequencies;

};

#endif // SESSION_H
