#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QThreadPool>
#include <QSettings>

#include "task.h"
#include "SysInfo.h"
#include "CpuWidget.h"
#include "MemoryWidget.h"
#include "SettingsTree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef QSharedPointer<QSettings> SettingsPtr;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void addTask();
    void removeOneTask(Task*);
    void taskStatusChanged(Task* = nullptr);

    void createTableView();

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setSettingsObject(const SettingsPtr &settings);
    void addMenuWithAction();
    void openIniFile();
    void openTreeView();
    void qmlexpore();

private:
    Ui::MainWindow *ui;
    QVector<Task*> mTasks;
    CpuWidget mCpuWidget;
    MemoryWidget mMemoryWidget;
    QThreadPool *thread_pool;


};

#endif // MAINWINDOW_H
