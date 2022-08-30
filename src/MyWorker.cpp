#include "MyWorker.h"
#include <QTime>
#include <QtDebug>

MyWorker::MyWorker(QObject *parent) : QObject(parent)
{

}

void MyWorker::sleep(int sec)
{
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime);
}

void MyWorker::run()
{
    qDebug() << "MyWorker thread" << this->thread();
    sleep(3);
    emit progressed(50);
    sleep(3);
    emit progressed(100);
}

void MyWorker::start()
{
    emit started();
    run();
    emit finished();
}

MyWorker::~MyWorker()
{
    qDebug() << "worker destroyed";
}
