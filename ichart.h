#ifndef ICHART_H
#define ICHART_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QList>
#include <QDataStream>
#include <QPair>
#include <QString>
#include <QPointF>
#include "idataloader.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>

// Интерфейс класса для создания графика, смены цвета, получение графика
class IChart
{
public:
    virtual void generateGraph(const DataTable& data, int count, const QList<QColor>& colors) = 0;// метод отвечающий за создание графика
    virtual void changeColors(const QList<QColor>& colors) = 0;// метод отвечающий за смену цвета
    virtual QChart* getGraph() = 0;// метод отвечающий за получение графика
};

// Реализация создания графика в виде вертикальных столбцов
class BarChart: public IChart
{
public:
    void generateGraph(const DataTable& data, int count, const QList<QColor>& colors) override;// метод отвечающий за создание графика

    void changeColors(const QList<QColor>& colors) override;// метод отвечающий за смену цвета

    QChart* getGraph() override {
        return chart;
    };

private:
    QChart *chart; // диаграмма
    QBarSeries *series; //представление серии данных в виде вертикальных столбцов
};

// Реализация создания графика в виде круговой диаграммы
class PieChart: public IChart
{
public:
    void generateGraph(const DataTable& data, int count, const QList<QColor>& colors) override;// метод отвечающий за создание графика

    void changeColors(const QList<QColor>& colors) override;// метод отвечающий за смену цвета

    QChart* getGraph() override {
        return chart;
    };

private:
    QChart *chart;// поле диаграммы, хранится сгенерированный график
    QPieSeries *series;// представление данных в виде круговых диаграмм
};

#endif // ICHART_H
