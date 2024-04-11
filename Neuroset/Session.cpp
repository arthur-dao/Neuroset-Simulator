// Session.cpp
#include "Session.h"

#include "QDebug"


Session::Session() : startDateTime(QDateTime::currentDateTime()) {}

void Session::startSession() {
    startDateTime = QDateTime::currentDateTime();
    qDebug() << "Session started at" << startDateTime.toString();
    // Start EEG simulation on all electrodes
    headset.startSimulation(); // Assume Headset class has this method.
}

void Session::analyzeData() {
    qDebug() << "Analyzing EEG data...";
}

void Session::endSession() {
    endDateTime = QDateTime::currentDateTime();
    qDebug() << "Session ended at" << endDateTime.toString();
}


void Session::addFrequency(float initial, float final) {
    frequencies.append(Frequency(initial, final));
}
