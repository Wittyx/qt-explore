#include "producttableview.h"
#include "ui_producttableview.h"

productTableView::productTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::productTableView)
{
    ui->setupUi(this);
    itemDelegate = new MyItemDelegate();
    dataModel = new QStandardItemModel(4,2,this);
    for(int row =0; row<4; row ++){
        for(int col =0; col<2; col ++){
            QModelIndex index = dataModel->index(row,col,QModelIndex());
            dataModel->setData(index,0);
        }
    }
    ui->tableView->setModel(dataModel);
    ui->tableView->setItemDelegate(itemDelegate);
}

productTableView::~productTableView()
{
    delete itemDelegate;
    delete dataModel;
    delete ui;
}
