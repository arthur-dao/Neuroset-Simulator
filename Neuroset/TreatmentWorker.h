#ifndef TREATMENTWORKER_H
#define TREATMENTWORKER_H

#include <QObject>
#include <QTimer>
#include "Electrode.h"

/**
 * @file TreatmentWorker.h
 * @brief Declaration of the TreatmentWorker class.
 * 
 * This file contains the declaration of the TreatmentWorker class, which represents a worker
 * responsible for processing treatment on a specific electrode. It includes methods to initiate
 * the treatment process and handle the treatment operation.
 */

class TreatmentWorker : public QObject {
    Q_OBJECT

public:
    explicit TreatmentWorker(Electrode *electrode, int sampleRate, QObject *parent = nullptr);
    ~TreatmentWorker();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

private:
    Electrode *m_electrode;
    int m_sampleRate;
    QTimer *m_timer;
};

#endif // TREATMENTWORKER_H
