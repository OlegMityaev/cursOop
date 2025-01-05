#include "ContactModel.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

ContactModel::ContactModel(QObject* parent)
    : QAbstractTableModel(parent) {
}

int ContactModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;
    return contacts.size();
}

int ContactModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;
    return 7;
}

QVariant ContactModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const Contact& contact = contacts.at(index.row());

    switch (index.column()) {
    case 0: return contact.firstName;
    case 1: return contact.lastName;
    case 2: return contact.middleName;
    case 3: return contact.address;
    case 4: return contact.birthDate.toString("yyyy-MM-dd");
    case 5: return contact.email;
    case 6: {
        QStringList phoneList = QStringList::fromVector(contact.phoneNumbers);
        return phoneList.join(", ");
    }
    default: return {};
    }
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    switch (section) {
    case 0: return QStringLiteral("Имя");
    case 1: return QStringLiteral("Фамилия");
    case 2: return QStringLiteral("Отчество");
    case 3: return QStringLiteral("Адрес");
    case 4: return QStringLiteral("Дата рождения");
    case 5: return "Email";
    case 6: return QStringLiteral("Телефоны");
    default: return {};
    }
}

bool ContactModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    auto& contact = contacts[index.row()];
    switch (index.column()) {
    case 0: {
        auto names = value.toString().split(" ");
        if (names.size() == 3) {
            contact.firstName = names[0];
            contact.middleName = names[1];
            contact.lastName = names[2];
        }
        break;
    }
    case 1: contact.address = value.toString(); break;
    case 2: contact.birthDate = QDate::fromString(value.toString(), "dd.MM.yyyy"); break;
    case 3: contact.email = value.toString(); break;
    case 4: contact.phoneNumbers = value.toString().split(", ").toVector(); break;
    default: return false;
    }
    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags ContactModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void ContactModel::addContact(const Contact& contact) {
    beginInsertRows(QModelIndex(), contacts.size(), contacts.size());
    contacts.append(contact);
    originalContacts.append(contact); // ориг
    endInsertRows();
}

void ContactModel::updateContact(int row, const Contact& contact) {
    if (row < 0 || row >= contacts.size())
        return;
    contacts[row] = contact;
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

void ContactModel::removeContact(int row) {
    if (row < 0 || row >= contacts.size())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    contacts.removeAt(row);
    originalContacts.removeAt(row); // ориг
    endRemoveRows();
}

bool ContactModel::saveToFile(const QString& fileName) const {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QJsonArray jsonArray;
    for (const auto& contact : contacts) {
        QJsonObject obj;
        obj["firstName"] = contact.firstName;
        obj["lastName"] = contact.lastName;
        obj["middleName"] = contact.middleName;
        obj["address"] = contact.address;
        obj["birthDate"] = contact.birthDate.toString(Qt::ISODate);
        obj["email"] = contact.email;
        obj["phoneNumbers"] = QJsonArray::fromStringList(contact.phoneNumbers.toList());
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson());
    return true;
}

bool ContactModel::loadFromFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray())
        return false;

    beginResetModel();
    contacts.clear();
    for (const auto& value : doc.array()) {
        QJsonObject obj = value.toObject();
        Contact contact;
        contact.firstName = obj["firstName"].toString();
        contact.lastName = obj["lastName"].toString();
        contact.middleName = obj["middleName"].toString();
        contact.address = obj["address"].toString();
        contact.birthDate = QDate::fromString(obj["birthDate"].toString(), Qt::ISODate);
        contact.email = obj["email"].toString();
        for (const auto& phone : obj["phoneNumbers"].toArray()) {
            contact.phoneNumbers.append(phone.toString());
        }
        contacts.append(contact);
        originalContacts.append(contact);
    }
    endResetModel();
    return true;
}

void ContactModel::filterContacts(const QString& query) {
    if (query.isEmpty()) {
        resetFilter();
        return;
    }

    beginResetModel();
    contacts.clear();

    for (const auto& contact : originalContacts) {
        // Проверяем все поля на совпадение с запросом
        if (contact.firstName.contains(query, Qt::CaseInsensitive) ||
            contact.lastName.contains(query, Qt::CaseInsensitive) ||
            contact.middleName.contains(query, Qt::CaseInsensitive) ||
            contact.address.contains(query, Qt::CaseInsensitive) ||
            contact.email.contains(query, Qt::CaseInsensitive) ||
            QStringList::fromVector(contact.phoneNumbers).join(", ").contains(query, Qt::CaseInsensitive) ||
            contact.birthDate.toString("yyyy-MM-dd").contains(query)) {
            contacts.append(contact);
        }
    }

    endResetModel();
}

void ContactModel::resetFilter() {
    beginResetModel();
    contacts = originalContacts; // Восстанавливаем из исходного списка
    endResetModel();
}

Contact ContactModel::getContact(int row) const {
    if (row < 0 || row >= contacts.size())
        return {};
    return contacts[row];
}


void ContactModel::sort(int column, Qt::SortOrder order) {
    beginResetModel(); // Уведомляем представление о начале изменений

    // Лямбда-функция для сравнения элементов
    auto comparator = [column, order](const Contact& a, const Contact& b) {
        switch (column) {
        case 0: return order == Qt::AscendingOrder ? a.firstName < b.firstName : a.firstName > b.firstName;
        case 1: return order == Qt::AscendingOrder ? a.lastName < b.lastName : a.lastName > b.lastName;
        case 2: return order == Qt::AscendingOrder ? a.middleName < b.middleName : a.middleName > b.middleName;
        case 3: return order == Qt::AscendingOrder ? a.address < b.address : a.address > b.address;
        case 4: return order == Qt::AscendingOrder ? a.birthDate < b.birthDate : a.birthDate > b.birthDate;
        case 5: return order == Qt::AscendingOrder ? a.email < b.email : a.email > b.email;
        case 6: return order == Qt::AscendingOrder ? QStringList::fromVector(a.phoneNumbers).join(", ") < QStringList::fromVector(b.phoneNumbers).join(", ") : QStringList::fromVector(a.phoneNumbers).join(", ") > QStringList::fromVector(b.phoneNumbers).join(", ");
        default: return false;
        }
    };

    std::sort(contacts.begin(), contacts.end(), comparator);

    endResetModel(); // Уведомляем представление о завершении изменений
}