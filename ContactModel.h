﻿#pragma once
#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QDate>

struct Contact {
    QString firstName;
    QString lastName;
    QString middleName;
    QString address;
    QDate birthDate;
    QString email;
    QVector<QString> phoneNumbers;
};

class ContactModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ContactModel(QObject* parent = nullptr);

    // Методы модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Управление данными
    void addContact(const Contact& contact);
    void updateContact(int row, const Contact& contact);
    void removeContact(int row);
    void sort(int column, Qt::SortOrder order);

    // Сохранение/загрузка данных
    bool saveToFile(const QString& fileName) const;
    bool loadFromFile(const QString& fileName);

    // Поиск и фильтрация
    void filterContacts(const QString& query);
    void resetFilter();

    Contact getContact(int row) const;

private:
    QVector<Contact> contacts;         // Отображаемые контакты
    QVector<Contact> originalContacts; // Исходный полный список контактов
};

