#ifndef TASKMGR_H
#define TASKMGR_H

#include <QMainWindow>

#include <QDebug>
#include <QMessageBox>


#include "QStandardItemModel"
#include "QStandardItem"
#include <QMenu>
#include <QTimer>
#include <QCheckBox>
#include <QGridLayout>
#include <QInputDialog>
#include "view.h"

#ifdef _WIN32

    #include "win_taskmgr.h"
    #include <windows.h>
    #include <tlhelp32.h>

#endif



namespace Ui {
class taskmgr;
}

class taskmgr : public QMainWindow
{
    Q_OBJECT

public:
    explicit taskmgr(QWidget *parent = 0);
    ~taskmgr();

private slots:
    void on_ProcTable_clicked(const QModelIndex &index);

    void on_TerminateProcessButton_clicked();

    void timer_slot();

    void on_actionView_triggered();

private:
    Ui::taskmgr *ui;

    // список свойств диспечера задач
    QStringList features;

    // чекбоксы для выбора отображения столбцов
    QCheckBox *checkBoxes;

    // указатель на абстрактый класс
    ProcList *taskmgr_func;

    // модель для таблицы
    QStandardItemModel *model;

    // номер текущей выборанной строки
    int SelectRow;

    // ассоциативный массив инфы о процессах
    QMap<QString,QStringList>  proc_info;

    // таймер для обновления инфы
    QTimer timer;

    view view_window;


    int RowCount;

    void fill_proc_table();
    void refresh_proc_table();

    QStringList status_puncts;
    QStringList active_features;


};

#endif // TASKMGR_H
