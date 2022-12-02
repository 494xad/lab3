#include "ichart.h"

void BarChart::generateGraph(const DataTable& m_dataTable, int count_Chart, const QList<QColor>& colors)//метод генерачии графика
// входные данные , из которых будет строится график : цвет, огараничения и сами данные
{
    chart = new QChart(); // инициализация диаграммы
    chart->setTitle("Bar chart"); //название диаграммы
    int count = 0; // сколько построили столбцов диаграммы
    double chartOther = 0.0;// хранятся все остальные значения
    series = new QBarSeries(chart);//создаем вертикальные столбцы
    for (int i(0); i < m_dataTable.count(); i++) {// проходимся по таблице с данными

        for (const Data &data : m_dataTable[i])
        {
            if(count < count_Chart - 1){// если построенных столбцов меньше ограничения, то в set
                // считываем значение и строим вертикальный столбец
                QBarSet* set = new QBarSet(data.second);
                *set << data.first.x();
                set->setBrush(colors.at(count));// устанавливаем цвет
                series->append(set);//добавляем вертикальный столбец в series
            }
            else
                chartOther += data.first.x();// считываем остальные значения в переменную chartOther

            count++;
        }
    }

    if(count > count_Chart){// если счетчик будет больше ограничения, то создаем последний столбец
        QBarSet *set = new QBarSet("Другие");// задаем имя последнему стобцу
        *set << chartOther;// на основе считанных данных строим вертикальный столбец
        set->setBrush(colors.at(count_Chart - 1));// задаем цвет
        series->append(set);//давабавляем вертикальный столбец в series
    }
    chart->addSeries(series);// добавляем series
    chart->createDefaultAxes();// управление графическим представлением серий
}

void BarChart::changeColors(const QList<QColor>& colors)// метод смены цвета, аргументом является список с цветами
{
    int i = 0;// счетчик который будет считать количество вертикальных столбцов
    foreach (QBarSet* x, series->barSets()){// проходимся по вертикальным столбцам
        x->setBrush(colors.at(i));// задаем цвет из списка
        i++;// увеличиваем счетчик
    }
}

void PieChart::generateGraph(const DataTable& m_dataTable, int count_Chart, const QList<QColor>& colors){//метод генерачии графика
    // входные данные , из которых будет строится график : цвет, огараничения и сами данные
    chart = new QChart();// инициализация диаграммы
    chart->setTitle("Pie chart");//название диаграммы
    int count = 0;// сколько построили фрагрментов диаграммы
    double chartOther = 0.0;// хранятся все остальные значения
    qreal pieSize = 1.0 / m_dataTable.count();
    for (int i = 0; i < m_dataTable.count(); i++) {
        series = new QPieSeries(chart);
        for (const Data &data : m_dataTable[i]) {// проходимся по таблице с данными
            if (count < count_Chart){// если построенных столбцов меньше ограничения, то в slice
                // считываем значение и строим фрагмент диаграммы
                QPieSlice *slice = series->append(data.second, data.first.x());
                if (data == m_dataTable[i].first()) {
                    slice->setLabelVisible();//обеспечение отображения текста или изображения
                    slice->setExploded();
                }
                slice->setBrush(colors.at(count));// устанавливаем цвет
            }
            else
                chartOther += data.first.x();
            count++;
        }

        if (count > count_Chart){
            QPieSlice *slice = series->append("Другие", chartOther);
            slice->setBrush(colors.at(count_Chart - 1));
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) m_dataTable.count());

        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.6);
        chart->addSeries(series);
    }
}
void PieChart::changeColors(const QList<QColor>& colors){
    int i = 0;
    foreach (QPieSlice* x, series->slices()){
        x->setBrush(colors.at(i));
        i++;
    }
}
