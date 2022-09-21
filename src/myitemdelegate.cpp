#include "myitemdelegate.h"

MyItemDelegate::MyItemDelegate(QObject *parent):
    QItemDelegate(parent)
{

}

QWidget *MyItemDelegate::createEditor(QWidget *parent,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(100);
    return editor;
}

void MyItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index,Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
}

void MyItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void MyItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


MyItemDelegate::~MyItemDelegate()
{

}
