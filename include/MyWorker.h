#ifndef MYWORKER_H
#define MYWORKER_H

#include <QObject>

class MyWorker : public QObject
{
    Q_OBJECT
public:
    explicit MyWorker(QObject *parent = nullptr);
     ~MyWorker();

private:
    void sleep(int sec);
signals:
    void started();
    void finished();
    void progressed(int percentage);

public slots:
    void start();

protected:
    void run();signals:

};

#endif // MYWORKER_H
