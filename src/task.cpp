#include "task.h"
#include "ui_task.h"
#include <QInputDialog>

Task::Task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    connect(ui->editButton, &QPushButton::clicked, this, &Task::rename);
    connect(ui->checkBox, &QCheckBox::clicked, this, &Task::checked);

    //connect(ui->removeButton, &QPushButton::clicked, this, &Task::removeSelf);
    connect(ui->removeButton, &QPushButton::clicked, this,
            [this](){ this->emit removed(this); });
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

Task::~Task()
{
    delete ui;
}
