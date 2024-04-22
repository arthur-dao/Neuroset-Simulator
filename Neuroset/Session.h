#ifndef SESSION_H
#define SESSION_H

#include <QDateTime>
#include <QList>
#include <QDebug>
#include "Frequency.h"


/**
 * @file Session.h
 * @brief Declaration of the Session class.
 * 
 * This file contains the declaration of the Session class, which represents a session
 * containing a list of frequency data recorded over a period of time. It includes methods
 * to retrieve session start and end times, frequencies, and convert session information
 * to a string format.
 */

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
