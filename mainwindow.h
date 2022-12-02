#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QSplitter>
#include <QPdfWriter>
#include <QFileDialog>
#include "ichart.h"


class MainWindow : public QWidget//
{
    Q_OBJECT
private slots:

    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);//СЛОТ выбора файла
    void on_select_comboboxOnChangedSlot(const int index);//слот выбора диаграммы
    void on_print_chart_slot();//слот печати графика
    void on_select_dir_slot();//слот выбора папки с файлами
    void on_color_chart_slot();//слот смены цвета
private:
    void on_reDraw();//перерисовка диаграммы
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QString filePath; //хранится путь до файла
    QString homePath; //хранится путь до папки
    QString homePathSavePdf; // хранится путь для сохранения в PDF
    QFileSystemModel *fileModel;// модель файлов
    QTableView *tableView;//таблица с файлами
    QPushButton *btnPrint;//кнопка печати
    QPushButton *selectDir;//кнопка выбора папки
    QCheckBox *checkbox;//смена цвета
    QComboBox *combobox;//выбор типа диаграммы
    QLabel *label, *label_path;//текстовые поля
    QChartView *chartView;//отображения диаграммы
    QSplitter *splitter;//разделитель
    IChart *chart;//объект диаграммы
    QList<QColor> colorsColored;//список цвета - цветной
    QList<QColor> colorsBlackWhite;//список цвета - черно - белый
    bool flag_color;//флаг для цвета
    int countChart;//количество рисуемых столбцов или кусков для диаграммы

};
#endif // MAINWINDOW_H
