#include "TreatmentWorker.h"
#include <QDebug>

TreatmentWorker::TreatmentWorker(Electrode *electrode, int sampleRate, QObject *parent)
    : QObject(parent), m_electrode(electrode), m_sampleRate(sampleRate) {
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &TreatmentWorker::process);
    m_timer->start();
}

TreatmentWorker::~TreatmentWorker() {
    delete m_timer;
}

void TreatmentWorker::process() {
    if (!m_electrode) {
        emit error("Electrode pointer is null");
        return;
    }

//    qDebug() << "Applying treatment to electrode" << m_electrode->getSiteNum();
    m_electrode->applyLENS(m_sampleRate, 1.0, 5.0);
    m_electrode->generateWaveform(m_sampleRate, 1);

    emit finished(); // Signal that the processing is done
}
