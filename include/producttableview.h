#ifndef PRODUCTTABLEVIEW_H
#define PRODUCTTABLEVIEW_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include "myitemdelegate.h"

namespace Ui {
class productTableView;
}

class productTableView : public QWidget
{
    Q_OBJECT

public:
    explicit productTableView(QWidget *parent = nullptr);
    ~productTableView();

private:
    Ui::productTableView *ui;
    QStandardItemModel *dataModel;
    MyItemDelegate *itemDelegate;

};

#endif // PRODUCTTABLEVIEW_H
