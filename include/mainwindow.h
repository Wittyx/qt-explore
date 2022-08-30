#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "task.h"
#include "SysInfo.h"
#include "CpuWidget.h"
#include "MemoryWidget.h"
#include <QThreadPool>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTask();
    void removeOneTask(Task*);
    void taskStatusChanged(Task* = nullptr);

protected:
    void showEvent(QShowEvent *event) override;


private:
    Ui::MainWindow *ui;
    QVector<Task*> mTasks;
    CpuWidget mCpuWidget;
    MemoryWidget mMemoryWidget;
    QThreadPool *thread_pool;

};

#endif // MAINWINDOW_H
