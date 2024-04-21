#include "qledlabel.h"

#include <QDebug>

static const int SIZE = 20;
static const QString greenSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 rgba(20, 252, 7, 255), stop:1 rgba(25, 134, 5, 255));").arg(SIZE / 2);
static const QString redSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:0.92, y2:0.988636, stop:0 rgba(255, 12, 12, 255), stop:0.869347 rgba(103, 0, 0, 255));").arg(SIZE / 2);
static const QString orangeSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.232, y1:0.272, x2:0.98, y2:0.959773, stop:0 rgba(255, 113, 4, 255), stop:1 rgba(91, 41, 7, 255))").arg(SIZE / 2);
static const QString blueSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.04, y1:0.0565909, x2:0.799, y2:0.795, stop:0 rgba(203, 220, 255, 255), stop:0.41206 rgba(0, 115, 255, 255), stop:1 rgba(0, 49, 109, 255));").arg(SIZE / 2);
static const QString blackSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.232, y1:0.272, x2:0.98, y2:0.959773, stop:0 rgba(160, 160, 160, 255), stop:1 rgba(0, 0, 0, 255));").arg(SIZE / 2);

QLedLabel::QLedLabel(QWidget *parent) : QLabel(parent) {
    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, &QLedLabel::toggleVisibility);
    setState(StateBlack);
    setFixedSize(SIZE, SIZE);
}

QLedLabel::~QLedLabel() {
    flashTimer->stop();
    delete flashTimer;
}

void QLedLabel::setState(State state) {
    currentState = state;
    switch(state){
        case StateInitiatedBlue:
            setStyleSheet(blueSS);
            break;
        case StateContactLostRed:
            setStyleSheet(redSS);
            break;
        case StateTreatmentGreen:
            setStyleSheet(greenSS);
            break;
        case StateBlack:
        default:
            setStyleSheet(blackSS);
            break;
    }
    if (!isFlashing) {
        setVisible(true);
    }
}

void QLedLabel::setGreen() {
    setState(StateTreatmentGreen);
}

void QLedLabel::setBlack() {
    setState(StateBlack);
    if (isFlashing == true) {
        stopFlashing();
    }
}

void QLedLabel::setBlue() {
    setState(StateInitiatedBlue);
}

void QLedLabel::setRed() {
    setState(StateContactLostRed);
    startFlashing(500);
}

void QLedLabel::startFlashing(int interval) {
    flashTimer->start(interval);
    isFlashing = true;
}

void QLedLabel::stopFlashing() {
    flashTimer->stop();
    isFlashing = false;
    setVisible(true);
}

void QLedLabel::toggleVisibility() {
    setVisible(!isVisible());
}
