#ifndef SESSION_H
#define SESSION_H

#include <QDateTime>
#include <QList>
#include <QDebug>
#include "Frequency.h"

class Session {
public:
    Session(QList<Frequency>, QDateTime=QDateTime::currentDateTime(), QDateTime=QDateTime::currentDateTime().addSecs(300));

    QDateTime getStart();
    QDateTime getEnd();
    QList<Frequency> getFrequencies();
    QString toString();

private:
    QDateTime startDateTime;
    QDateTime endDateTime;
    QList<Frequency> frequencies;
};

#endif // SESSION_H
