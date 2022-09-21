#ifndef MYITEMDELEGATE_H
#define MYITEMDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class MyItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MyItemDelegate(QObject *parent = 0);
    ~MyItemDelegate();
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // MYITEMDELEGATE_H
