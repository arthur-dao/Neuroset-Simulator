#ifndef TREATMENTWORKER_H
#define TREATMENTWORKER_H

#include <QObject>
#include <QTimer>
#include "Electrode.h"

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
