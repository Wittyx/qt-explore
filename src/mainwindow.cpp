#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "productTableView.h"
#include "ExQTreeWidget.h"
#include "WQMLEngine.h"
#include <QMessageBox>
#include <QMenuBar>
#include <mainwindow.h>
#include <QFileDialog>
#include <mainwindow.h>


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

    //Add table view
    connect(ui->tableViewSample,&QPushButton::clicked,this, &MainWindow::createTableView);

    addMenuWithAction();
}

void MainWindow::addMenuWithAction(){
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openIniFileAct = fileMenu->addAction(tr("Open I&NI File..."), this, &MainWindow::openIniFile);
    openIniFileAct->setShortcut(tr("Ctrl+N"));

    QMenu *treeView = menuBar()->addMenu(tr("&TreeView"));

    QAction *openTreeViewAct = treeView->addAction(tr("TreeViewDemo"), this, &MainWindow::openTreeView);
    openTreeViewAct->setShortcut(tr("Ctrl+T"));

    QMenu *qmlMenu = menuBar()->addMenu(tr("&QML"));
    QAction *openQmlexpore = qmlMenu->addAction(tr("QML_Explore"), this, &MainWindow::qmlexpore);

}

void MainWindow::openIniFile(){
    const QString directory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    const QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open INI File"),
                                     QDir::currentPath()+"/../", tr("INI Files (*.ini *.conf)"));
    if (fileName.isEmpty())
        return;

    SettingsPtr settings(new QSettings(fileName, QSettings::IniFormat));

    setSettingsObject(settings);
    //fallbacksAct->setEnabled(false);
}
void MainWindow::setSettingsObject(const SettingsPtr &settings)
{
    SettingsTree *settingsTree = new SettingsTree();
    settingsTree->resize(1000,1000);
    settingsTree->show();

    //settings->setFallbacksEnabled(fallbacksAct->isChecked());
    settings->setFallbacksEnabled(false);
    settingsTree->setSettingsObject(settings);

    QString niceName = QDir::cleanPath(settings->fileName());
    int pos = niceName.lastIndexOf(QLatin1Char('/'));
    if (pos != -1)
        niceName.remove(0, pos + 1);

    if (!settings->isWritable())
        niceName = tr("%1 (read only)").arg(niceName);

    setWindowTitle(tr("%1 - %2").arg(niceName, QCoreApplication::applicationName()));
    statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(settings->fileName())));
}

void MainWindow::openTreeView(){
    QWidget *wdg = new ExQTreeWidget();
    wdg->resize(2000,2000);
    wdg->show();
}

void MainWindow::qmlexpore(){
    //WQMLEngine *engine = new WQMLEngine();
    //engine->createQmlWindow(nullptr);
    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
    QStringLiteral("ToDoList should not be created in QML"));
    QQmlContext *qmlContext = engine.rootContext();
    QQmlComponent component(&engine,QUrl(QStringLiteral("qrc:/MyToDoList.qml")));
    qmlContext->setContextProperty(QStringLiteral("toDoList"), &toDoList);
    qmlContext->setContextProperty("_aSize", QSize(1200, 1000));
    object = component.create();
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

void MainWindow::createTableView()
{
    QWidget *wdg = new productTableView();
    wdg->resize(1000,1000);
    wdg->show();
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
