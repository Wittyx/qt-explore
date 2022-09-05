#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QThread>
#include "MyWorker.h"
#include <QThreadPool>

namespace Ui {
class Task;
}

class Task : public QWidget
{
    Q_OBJECT

public:
    explicit Task(QThreadPool *threadPool= nullptr, QWidget *parent = nullptr);
    ~Task();
    QString name() const;
    void setName(const QString& name);
    bool isCompleted() const;

signals:
    void removed(Task* task);
    void statusChanged(Task* task);
    void startRenameClicked();
public slots:
    void rename();
    void checked(bool checked);
    void startTask();

private slots:
    void removeSelf();

private:
    Ui::Task *ui;
    QThread *thread;
    QThreadPool *threadPool;
};

#endif // TASK_H
