#include "taskmgr.h"
#include "ui_taskmgr.h"


/// ---------------------------------------------------------------
///        Constructor
/// ---------------------------------------------------------------
taskmgr::taskmgr(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::taskmgr)
{
    ui->setupUi(this);

    // create TASKMGR object
    #ifdef _WIN32
        taskmgr_func=new WIN_TASKMGR();
        //features<<"Name"<<"PID"<<"Parent PID"<<"Thread count"<<"Priory"<<"Create time"<<"Work time"<<"Memory"<<"Path";
        features<<"Name"/*<<"PID"<<"Parent PID"<<"Thread count"<<"Priory"<<"Create time"<<"Work time"*/<<"Memory"<<"Path";
        status_puncts<<"CPU";
    #endif

    active_features=features;
    // model for proc table
    model = new QStandardItemModel;

    // fill proc table
    taskmgr::fill_proc_table();


    SelectRow=0;

    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_slot()));
    timer.start(1000);

    view_window.fill_checkboxes(features);

}




/// ---------------------------------------------------------------
///        Refresh timer
/// ---------------------------------------------------------------
void taskmgr::timer_slot()
{
    refresh_proc_table();
}




///----------------------------------------------------------------
///        Fill proc table
/// ---------------------------------------------------------------
void taskmgr::fill_proc_table()
{
    QStandardItem *item;

    //Заголовки столбцов
    QStringList horizontalHeader;


    proc_info=taskmgr_func->get_proc_info();


    // fill header
    foreach(QString active_punct,active_features)
    {
        horizontalHeader.append(active_punct);
    }

    model->setHorizontalHeaderLabels(horizontalHeader);


    // fill columns
    int i=0,j;
    foreach(QString active_punct,active_features)
    {
        j=0;
        foreach(QString str,proc_info[active_punct])
        {
            item = new QStandardItem(str);
            item->setEditable(0); // lock items
            model->setItem(j, i, item);
            j++;
        }
        i++;
    }
    RowCount=j;
    ui->ProcTable->setModel(model);

//    ui->ProcTable->resizeRowsToContents();
    ui->ProcTable->resizeColumnsToContents();



}




///----------------------------------------------------------------
///        Refresh proc table
/// ---------------------------------------------------------------
void taskmgr::refresh_proc_table()
{
    QStandardItem *item;
    proc_info["Name"].clear();
    proc_info.clear();

    proc_info=taskmgr_func->get_proc_info();
    int i=0,j;
//    RowCount=proc_info["PID"].length();
//    qDebug()<<"RowCount"<<RowCount;
    foreach(QString active_punct,active_features)
    {
        j=0;
        foreach(QString str,proc_info[active_punct])
        {
            // Добавление новое ячейки, если создался процесс
            if(j>RowCount)//if(j>=RowCount-1)
            {
                item = new QStandardItem(str);
                item->setEditable(0); // lock items
                model->setItem(j,i, item);
                ui->ProcTable->setModel(model);
                if(RowCount<j) RowCount=j;//j+1;
            }
            // Обновление значения старой ячейки
            else
//                qDebug()<<j<<i;
                model->item(j, i)->setData(str,Qt::DisplayRole);

            // счетчик строк
            j++;
        }
        // считчик столбцов
        i++;
    }

//    qDebug()<<"j"<<j;
//    qDebug()<<"RowCount"<<RowCount;
    // очистка ячеек при завершении одного из процессов
    for(int i=j;i<RowCount;i++)
    {
//        qDebug()<<"empty row"<<i;
        for(int j1=0;j1<active_features.length();j1++)
        {
            model->item(i, j1)->setData("",Qt::DisplayRole);
        }
    }

    ui->ProcTable->setModel(model);

}


///----------------------------------------------------------------
///        Click on row from proc table
/// ---------------------------------------------------------------
void taskmgr::on_ProcTable_clicked(const QModelIndex &index)
{

    SelectRow=index.row();
    ui->ProcTable->setSelectionBehavior(QAbstractItemView::SelectRows);

}


///----------------------------------------------------------------
///        Kill process
/// ---------------------------------------------------------------
void taskmgr::on_TerminateProcessButton_clicked()
{
    if(!SelectRow) return;

    QString PID=proc_info["PID"][SelectRow];
    taskmgr_func->kill_process(PID);
//    refresh_proc_table();
}


///----------------------------------------------------------------
///        Destructor
/// ---------------------------------------------------------------
taskmgr::~taskmgr()
{
    timer.stop();
    disconnect(this,SLOT(timer_slot()));
    delete model;
    delete taskmgr_func;
    delete ui;
}



///----------------------------------------------------------------
///        Click on view punct menu
/// ---------------------------------------------------------------
void taskmgr::on_actionView_triggered()
{
//    ui->GroupCheckedView->show();
    view_window.exec();
    active_features.clear();
    active_features=view_window.get_checked();
    model->clear();
//    taskmgr::refresh_proc_table();
    taskmgr::fill_proc_table();
}


