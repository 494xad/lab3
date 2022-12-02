#ifndef IDATALOADER_H
#define IDATALOADER_H

#include <QMainWindow>
#include <QWidget>
#include <QtSql>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QDataStream>
#include <QFile>
#include <QList>
#include <QDataStream>
#include <QPair>
#include <QString>
#include <QPointF>

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;

class IDataLoader// создаем интерфейсный класс , который будет получать данные
{
public:
    virtual DataTable getData(const QString& path) = 0; // метод получения данных, возвращает таблицу с данными
};

// Класс реализующий метод getData для загрузки данных из SQLite таблицы
class SqliteDataLoader : public IDataLoader
{
public:
    // метод получения данных из Sqlite
    // path - путь к файлу
    // return Таблица с данными
    DataTable getData(const QString& path);
};

// Класс реализующий метод getData для загрузки данных из Json файла
class JsonDataLoader: public IDataLoader
{
public:
    // метод получения данных из JSON файла
    // path - путь к файлу
    // return Таблица с данными
    DataTable getData(const QString& path);
};


#endif // IDATALOADER_H
