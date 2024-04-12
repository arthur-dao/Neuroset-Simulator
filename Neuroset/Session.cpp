// Session.cpp
#include "Session.h"

#include "QDebug"


Session::Session() : startDateTime(QDateTime::currentDateTime()) {}

void Session::startSession() {

}

void Session::selectElectrode(int electrodeIndex) {
    headset.setActiveElectrode(electrodeIndex);
}

void Session::addFrequency(float initial, float final) {
    frequencies.append(Frequency(initial, final));
}
