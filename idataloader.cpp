#include "idataloader.h"
#include <QMessageBox>

DataTable SqliteDataLoader::getData(const QString& path)
{
    DataTable data;// определяем переменную , которая будет хранить таблицу с данными
    DataList dataList;//определяем переменную , которая будет хранить таблицу с данными,data состоит из dataList
    int i = 0;// определяем счетчик для подсчета колличества считанных данных
    int sizeData = 11;// устанавливаем максимальное колличество, которое можно считать
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(path);

    if (!dbase.open()) {// условие проверки открытости бд
        QMessageBox msg;
        msg.setText("База данных не открылась " + path);// если не открылась , то выводим данное сообщение
        msg.exec();
    }

    else
    {
        QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst());
        while (query.next() && i < sizeData) {// проходим по бд , те до тех пор пока сущ следующее значение
            // и пока i < sizeData
            QString name = query.value(0).toString();// в переменную name считываем первое значение бд
            QPointF salary(query.value(1).toDouble(),rand());// в переменную salary типа QPointF считываются 2 значения,
            // 1 значение, это значение из бд
            // 2ое значение рандомно, оно использоваться не будет
            dataList << Data(salary, name);// записываем в список считанные значения
            i++; // увеливаем счетчик
        }
    }
    data << dataList;// в таблицу данных записываем список со считаннными значениями
    return data;
}

DataTable JsonDataLoader::getData(const QString& path)// метод получения данных из Json, аргументом является путь до файла ,
// а возвращает таблицу с данными
{
    DataTable data;// определяем переменную , которая будет хранить таблицу с данными
    DataList dataList;//определяем переменную , которая будет хранить таблицу с данными,data состоит из dataList
    QString jsonText;// создание ключа , для чтения файла
    QFile file;// создание файла
    file.setFileName(path);// устанавливаем путь до файла
    file.open(QIODevice::ReadOnly | QIODevice::Text);// открытие файла
    jsonText = file.readAll();// загружаем файл в переменную кеу
    file.close();// закрытие файла

    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    QJsonObject jsonObject = doc.object();// загружаем докумумент
    QJsonArray jsonArray = jsonObject["test"].toArray();//инициализируем массив из файла Json

    foreach (const QJsonValue& value, jsonArray)// проходимся по массиву
    {
        if (value.isObject())// проверяем существование объекта
        {
            QJsonObject obj = value.toObject();// считываем объект
            QString name = obj["name"].toString();// в переменную name считываем первое значение бд
            QPointF salary(obj["key"].toDouble(),rand());// в переменную salary типа QPointF считываются 2 значения,
            // 1 значение, это значение из бд
            // 2ое значение рандомно, оно использоваться не будет
            dataList << Data(salary, name);// записываем в список считанные значения
        }
    }
    data << dataList;// в таблицу данных записываем список со считаннными значениями
    return data;
}

