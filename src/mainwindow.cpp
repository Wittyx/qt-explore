#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thread_pool(new QThreadPool(this))
{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    //Why not working, because sinal and slot arguments are not compatible
    //connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::taskStatusChanged);
    connect(ui->addTaskButton, SIGNAL(clicked()), this, SLOT(taskStatusChanged()));


    SysInfo::instance().init();

    QHBoxLayout *statusBarLayout = new QHBoxLayout();
    ui->statusBar->setLayout(statusBarLayout);
    ui->statusBar->addWidget(&mCpuWidget);

    QLabel *cpuStatusLabel = new QLabel(ui->statusBar);
    cpuStatusLabel->setObjectName(QString::fromUtf8("statusLabel"));
    cpuStatusLabel->setText(QString::fromUtf8("CPU"));
    ui->statusBar->addWidget(cpuStatusLabel);


    ui->statusBar->addWidget(&mMemoryWidget);
    QLabel *memoryStatusLabel = new QLabel(ui->statusBar);
    memoryStatusLabel->setObjectName(QString::fromUtf8("statusLabel"));
    memoryStatusLabel->setText(QString::fromUtf8("Memory"));
    ui->statusBar->addWidget(memoryStatusLabel);

    ui->statusBar->setStyleSheet(
        "QStatusBar::item { border: none; } "
        //"QStatusBar::item { border: 1px solid red; border-radius: 3px; } "
    );

    ui->statusBar->setLayoutDirection(Qt::RightToLeft);
    //ui->statusBar->showMessage("status");

    //change pool size
    thread_pool->setMaxThreadCount(3);
}

void MainWindow::removeOneTask(Task* task)
{
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    task->setParent(nullptr);
    // other slot invocations will be cancelled at task instance's deletion
    // deleting a QObject directly/explictly is not a good practice in Qt
//    delete task;
    task->deleteLater();
}

void MainWindow::taskStatusChanged(Task*)
{
    int completedCount = 0;
    for(auto t : mTasks) {
        if (t->isCompleted()) {
            completedCount++;
        }
    }
    int todoCount = mTasks.size() - completedCount;
    ui->statusLabel->setText(
                QString("Status: %1 todo / %2 completed")
                .arg(todoCount)
                .arg(completedCount)
                );
}

void MainWindow::addTask(){
    Task *task = new Task(thread_pool);
    connect(task, &Task::statusChanged,
            this, &MainWindow::taskStatusChanged);
    connect(task, &Task::removed,
            this, &MainWindow::removeOneTask);
    connect(task, &Task::removed,
            this, &MainWindow::taskStatusChanged);
    mTasks.append(task);
    ui->tasksLayout->addWidget(task);
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    taskStatusChanged();
}

MainWindow::~MainWindow()
{
    thread_pool->clear();
    thread_pool->waitForDone();
    delete thread_pool;
    delete ui;
}
