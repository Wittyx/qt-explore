#include "task.h"
#include "ui_task.h"
#include <QInputDialog>
#include "MyTaskSignals.h"
#include "MyTask.h"

Task::Task(QThreadPool *threadPool,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task),
    threadPool(threadPool)
{
   thread = new QThread(this);
    qDebug() << "Create Task Thread ID:" <<thread->currentThreadId();
    ui->setupUi(this);
    connect(ui->editButton, &QPushButton::clicked, this, &Task::rename);
    connect(ui->checkBox, &QCheckBox::clicked, this, &Task::checked);

    //connect(ui->removeButton, &QPushButton::clicked, this, &Task::removeSelf);
    connect(ui->removeButton, &QPushButton::clicked, this,
            [this](){ this->emit removed(this); });
    connect(ui->startTaskButton,&QPushButton::clicked, this, &Task::startTask);

}

void Task::rename()
{
    bool ok;
    QString value = QInputDialog::getText(
                this, tr("Edit task"),
                tr("Task name"),
                QLineEdit::Normal,
                this->name(), &ok);
    if (ok && !value.isEmpty()) {
        setName(value);
    }
}

void Task::removeSelf()
{
    emit removed(this);
}

void Task::checked(bool checked)
{
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    emit statusChanged(this);
}

QString Task::name() const
{
    return ui->checkBox->text();
}

void Task::setName(const QString& name) {
    if (name != this->name()) {
        ui->checkBox->setText(name);
    }
}

bool Task::isCompleted() const {
    return ui->checkBox->isChecked();
}

void Task::startTask(){
    /*
    //Use Thread Start
    MyWorker* worker = new MyWorker();
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &MyWorker::start);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);

    connect(worker, &MyWorker::finished, thread, &QThread::quit);
    connect(worker, &MyWorker::finished, worker, &QObject::deleteLater);

    connect(worker, &MyWorker::started,
            this, [this](){ui->startTaskButton->setDisabled(true); ui->startTaskButton->setText("Running...");});
    connect(worker, &MyWorker::progressed,
            this, [this](int percentage){ui->startTaskButton->setText(QString("%1% done").arg(percentage));});
    connect(worker, &MyWorker::finished,
            this, [this](){ui->startTaskButton->setDisabled(false);});

    thread->start();
     qDebug() << "thread ID:" <<thread->thread();
     //Use Thread End
     */

    //Use Thread pool Start
    MyTask* task = new MyTask();

    connect(task->get_signals(), &MyTaskSignals::started,
            this, [this](){ui->startTaskButton->setDisabled(true); ui->startTaskButton->setText("Running...");});
    connect(task->get_signals(), &MyTaskSignals::progressed,
            this, [this](int percentage){ui->startTaskButton->setText(QString("%1% done").arg(percentage));});
    connect(task->get_signals(), &MyTaskSignals::finished,
            this, [this](){ui->startTaskButton->setDisabled(false);});

    threadPool->start(task);
    //Use Thread pool End

}

Task::~Task()
{
    if(ui){
        delete ui;
        ui = nullptr;
        qDebug() << "Ui Task destroyed";
    }

    if(thread){
        delete thread;
        thread = nullptr;
        qDebug() << "thread destroyed"  <<thread->currentThreadId();
    }
}
