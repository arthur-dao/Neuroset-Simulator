// Session.cpp
#include "Session.h"

Session::Session(QList<Frequency> freq, QDateTime start, QDateTime end){
    frequencies = freq;
    startDateTime = start;
    endDateTime = end;
}

QDateTime Session::getStart(){
    return startDateTime;
}

QDateTime Session::getEnd(){
    return endDateTime;
}

QList<Frequency> Session::getFrequencies(){
    return frequencies;
}

QString Session::toString(){
    QString text = "SESSION: "  + startDateTime.toString() + " - " + endDateTime.toString() + "\n";
    for(Frequency f : frequencies){
        text += "\t Initial: " + QString::number(f.getInitialFrequency()) +  " - Final: " +  QString::number(f.getFinalFrequency()) + "\n";
    }

    return text;
}
