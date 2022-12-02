#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QCoreApplication>
#include "ioc.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    setWindowTitle("Laba3"); // заголовк окна
    resize(1500, 500); //Устанавливаем размер главного окна

    QString path = PRJ_PATH;//в path переменная сохраняется путь до проекта с файлами, при помощи PRJ_PATH

    homePath = path + "/InputData";// определяем путь до папки с файлом с форматами Sqlite  и JSON
    // Определим  файловой системы:
    fileModel = new QFileSystemModel(this);// устанавливаем модель системы
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);// фильтруем системную модель
    fileModel->setRootPath(homePath);// устанавливаем корневой путь
    label_path = new QLabel("Выберите путь: ");// создаем текстовый объект , который отображает путь до файла
    label_path->setText(homePath);// отображаем путь до файла
    label = new QLabel("Выберите тип диаграммы");//текст перед combobox
    combobox = new QComboBox();//выбор графика
    btnPrint = new QPushButton("Печать графика"); //кнопка печати графика в пдф
    checkbox = new QCheckBox("Черно-белый график"); // выбор цвета графика
    selectDir = new QPushButton("Выбор папки");// кнопка выбора файла с данными
    countChart = 11; // ограничение для выводимых элементов графика

    // задаем цвет для диаграммы
    for (int i = 0; i < countChart; i++)
    {
        int R = rand()%250;// задаем цветной цвет
        int G = rand()%250;// задаем цветной цвет
        int B = rand()%250;// задаем цветной цвет
        int BW = (R+G+B)/3;// задаем черно-белый цвет
        colorsColored.push_back(QColor(R,G,B));//добавляем цвет в список
        colorsBlackWhite.push_back(QColor(BW, BW, BW));//добавляем в список черно-белый цвет
    }

    QHBoxLayout *layout = new QHBoxLayout();// создаем компонент layout, который будет хранить в себе
    //компоненты ниже
    //добавляем компоненты интерфейса в горизонтальный layout
    layout->addWidget(label);// добавление тестового поля
    layout->addWidget(combobox);//добавление списка
    layout->addWidget(checkbox);//добавление checkbox
    layout->addWidget(btnPrint);//добавление кнопка печати
    layout->addWidget(selectDir);//добавление кнопки выбора папки
    layout->setAlignment(Qt::AlignCenter);//центрирование layout
    splitter = new QSplitter(parent);// создаем разделитель
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // заполнять всё доступное пространство по вертикали
    tableView = new QTableView(splitter);// создаем таблицу с файлами
    tableView->setModel(fileModel);// устанавливаем модель

    //1.Добавление диаграммы
    chartView = new QChartView(splitter);//поле отображения граффика
    flag_color = false;// флаг смены цвета
    splitter->setStretchFactor(0, 1); // устанавливаем начальное положение разделителя
    splitter->setStretchFactor(1, 150);// устанавливаем начальное положение разделителя
    //создаем вертикальный layout
    QVBoxLayout *layout_vertical = new QVBoxLayout(this);
    layout_vertical->addLayout(layout);// добавляем layout
    layout_vertical->addWidget(splitter);// добавляем разделитель
    layout_vertical->addWidget(label_path);// добавляем текстовое поле пути

    QItemSelectionModel *selectionModel = tableView->selectionModel();// создаем объект, который
    //будет отслеживать выбранный элемент
    QModelIndex rootIx = fileModel->index(0, 0, QModelIndex());//задаем индекс корневого элемента
    QModelIndex indexHomePath = fileModel->index(homePath);// задаем объект для поиска данных в моделе
    QFileInfo fileInfo = fileModel->fileInfo(indexHomePath);// задаем объект, который будет
    //предоставлять независимую от системы информацию о файлах

    combobox->addItem("Bar Chart");// добавляем в список столбчатую диаграмму
    combobox->addItem("Pie Chart");//добавляем в список круглую диаграмму

    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TableView
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    connect(combobox, &QComboBox::currentIndexChanged, this, &MainWindow::on_select_comboboxOnChangedSlot);// выбор графика
    connect (btnPrint, SIGNAL(clicked()), this, SLOT(on_print_chart_slot())); //печать графика
    connect (selectDir, SIGNAL(clicked()), this, SLOT(on_select_dir_slot())); //выбор папки
    connect (checkbox, SIGNAL(toggled(bool)), this, SLOT(on_color_chart_slot()));//смена цвета

    //Пример организации установки курсора в TableView относительно модельного индекса

    tableView->setRootIndex(fileModel->setRootPath(homePath));// устанавливаем корневой индекс в таблицу
}
// слот смены цвета диаграммы
void MainWindow::on_color_chart_slot(){
        if (checkbox->checkState())// проверяем состояние активирован ли флажок checkbox
        {
            chart->changeColors(colorsBlackWhite);// меняем цвет на черно-белый
            flag_color = true;// меняем флаг
        }
        else
        {
            chart->changeColors(colorsColored);// меняем цвет на цветной
            flag_color = false;// меняем флаг
        }
        on_reDraw();//перерисовываем диаграмму
}
void MainWindow::on_select_dir_slot()// слот выбора папки с файлом
{
    QFileDialog dialog(this);// предоставление диалогового окна
    dialog.setFileMode(QFileDialog::Directory);// устанавливаем файловую модель директории
    if (dialog.exec())
    {
        homePath = dialog.selectedFiles().first();//сохраняем в переменную, выбранный путь
    }
    label_path->setText(homePath);
    tableView->setRootIndex(fileModel->setRootPath(homePath));//меняем корень таблицы
}

 //слот печати графика с помощью QPdfWriter
void MainWindow::on_print_chart_slot()// слот выбора папки с файлом
{
    QFileDialog dialog(this);// предоставление диалогового окна
    dialog.setFileMode(QFileDialog::Directory);// устанавливаем файловую модель директории
    if (dialog.exec())
    {
        homePathSavePdf = dialog.selectedFiles().first();//сохраняем в переменную, выбранный путь
    }
    QPdfWriter writer(homePathSavePdf + "/" + "grafics.pdf");// создаем объект для записи в пдф файл с указателем пути

    writer.setCreator("Someone");//Указываем создателя документ
    writer.setPageSize(QPageSize(QPageSize::A4));//Устанавливаем размер страницы

    QPainter painter(&writer);
    chartView->render(&painter);
    painter.end();

    QMessageBox mes;
    mes.setText("PDF создан в " + homePathSavePdf);//сообщение о том что пдф создан
    mes.exec();
}

//Слот для обработки выбора элемента в TableView
//выбор осуществляется с помощью курсора

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndex index = tableView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    filePath = "";

    // Размещаем инфо в label_path относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
        label_path->setText(filePath);
        //this->statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
    }

    //TODO: !!!!!
    /*
    Тут простейшая обработка ширины первого столбца относительно длины названия папки.
    Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
    Требуется доработка(переработка).
    */
    int length = 200;
    int dx = 30;

    if (fileModel->fileName(index).length() * dx > length) {
        length = length + fileModel->fileName(index).length() * dx;
        qDebug() << "r = " << index.row() << "c = " << index.column() << fileModel->fileName(index) << fileModel->fileInfo(
                     index).size();

    }

    //тип графика
    QString type_chart = combobox->currentText();// из combobox считываем текст, те какая диаграмма открыта
     if (type_chart == "Pie Chart"){// если тип диаграммы Pie Chart
         //то через Ioc контейнер регистрируем нужный нам тип диаграммы
         gContainer.RegisterInstance<IChart, PieChart>();// регистрируем тип диаграммы
     }
     else if (type_chart == "Bar Chart")// если тип диаграммы Pie Chart
         //то через Ioc контейнер регистрируем нужный нам тип диаграммы
     {
         gContainer.RegisterInstance<IChart, BarChart>();
     }
     else {
         QMessageBox mes;
         mes.setText("Тип графика не верный: " + type_chart);// если тип диаграммы неверный то
         // выводим сообщение
         mes.exec();
      }

    if(filePath.endsWith(".sqlite"))// если конец пути sqlite, то регистрируем считывание данных из sqlite
    {
        //считываем данные из sqlite
        gContainer.RegisterInstance<IDataLoader, SqliteDataLoader>();// регистрируем счтывание данных из sqlite
    }
    else
        if(filePath.endsWith(".json"))// если конец пути json, то регистрируем считывание данных из json
        {
            //считываем данные из json
            gContainer.RegisterInstance<IDataLoader, JsonDataLoader>();// регистрируем счтывание данных из json
        }
        else
        {
            //если не правильно выбран формат файла
            QMessageBox mes;
            mes.setText("Формат файла выбран не верно");// вывод сообщения о том, что формат выбран неверно
            mes.exec();
        }
    on_reDraw();// перерисовываем график
}

void MainWindow::on_reDraw(){// слот перерисовки графика
    chart = gContainer.GetObject<IChart>().get(); //получаем график из контейнера
    if(flag_color)// проверяем какой цвет активен
    {
        //строим график, ставим ограничения на количество рисуемых столбцов
        chart->generateGraph(gContainer.GetObject<IDataLoader>()->getData(filePath), countChart, colorsBlackWhite);
        //меняем цвет на черно-белый
    }
    else
    {
        //строим график, ставим ограничения на количество рисуемых столбцов
        chart->generateGraph(gContainer.GetObject<IDataLoader>()->getData(filePath), countChart, colorsColored);
        //меняем цвет на цветной
    }
    chartView->setChart(chart->getGraph());//отображаем график
}

void MainWindow::on_select_comboboxOnChangedSlot(const int index)// слот выбора типа диаграммы
{
    try
    {
        switch (index) // получаем из выбранного индекса тип отображения
        {
        case 0:
            gContainer.RegisterInstance<IChart, BarChart>();// устанавливаем вертикальную диаграмму
            break;
        case 1:

            gContainer.RegisterInstance<IChart, PieChart>();// устанавливаем круговую диаграмму
            break;
        default:
            throw std::runtime_error("Error: тип выбран не верно");
            break;
        }
    on_reDraw();// перерисовываем график
    }
    catch (const std::runtime_error &e)
    {
        QMessageBox::about(this, "Error", e.what()); // сообщаем что в процессе выбора вида отображения что-то пошло не так
    }
}

MainWindow::~MainWindow()
{

}
