#include "ContactDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QMessageBox>
#include <QString>



ContactDialog::ContactDialog(Contact& contact, QWidget* parent)
    : QDialog(parent), contact(contact) {
    setWindowTitle(QStringLiteral("Редактирование контакта"));

    firstNameEdit = new QLineEdit(this);
    firstNameEdit->setText(contact.firstName);

    lastNameEdit = new QLineEdit(this);
    lastNameEdit->setText(contact.lastName);

    middleNameEdit = new QLineEdit(this);
    middleNameEdit->setText(contact.middleName);

    addressEdit = new QLineEdit(this);
    addressEdit->setText(contact.address);

    birthDateEdit = new QDateEdit(this);
    birthDateEdit->setDate(contact.birthDate);
    birthDateEdit->setCalendarPopup(true);

    emailEdit = new QLineEdit(this);
    emailEdit->setText(contact.email);


    // Лейаут для ввода телефонных номеров
    phoneNumbersLayout = new QVBoxLayout;
    QLineEdit* initialPhoneNumberEdit;
    for (const auto& phone : contact.phoneNumbers) {
        initialPhoneNumberEdit = new QLineEdit(this);
        initialPhoneNumberEdit->setText(phone);
        phoneNumbersLayout->addWidget(initialPhoneNumberEdit);
        phoneNumbersEdits.append(initialPhoneNumberEdit);
    }

    phoneButtonsLayout = new QHBoxLayout;

    // Кнопка "+" для добавления нового номера
    addPhoneButton = new QPushButton("+", this);
    removePhoneButton = new QPushButton("-", this);
    phoneButtonsLayout->addWidget(addPhoneButton);
    phoneButtonsLayout->addWidget(removePhoneButton);
    connect(addPhoneButton, &QPushButton::clicked, this, &ContactDialog::addPhoneNumberField);
    connect(removePhoneButton, &QPushButton::clicked, this, &ContactDialog::removePhoneNumberField);


    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QStringLiteral("Имя"), firstNameEdit);
    formLayout->addRow(QStringLiteral("Фамилия"), lastNameEdit);
    formLayout->addRow(QStringLiteral("Отчество"), middleNameEdit);
    formLayout->addRow(QStringLiteral("Адрес"), addressEdit);
    formLayout->addRow(QStringLiteral("Дата рождения"), birthDateEdit);
    formLayout->addRow("Email", emailEdit);


    QVBoxLayout* phoneGroupLayout = new QVBoxLayout;
    phoneGroupLayout->addLayout(phoneNumbersLayout);
    phoneGroupLayout->addLayout(phoneButtonsLayout);
    formLayout->addRow(QStringLiteral("Телефоны"), phoneGroupLayout);

    QHBoxLayout* buttonBox = new QHBoxLayout;
    QPushButton* okButton = new QPushButton(QStringLiteral("OK"), this);
    QPushButton* cancelButton = new QPushButton(QStringLiteral("Отмена"), this);

    buttonBox->addWidget(okButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonBox);

    connect(okButton, &QPushButton::clicked, this, &ContactDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &ContactDialog::reject);

    updateRemoveButtonVisibility();
}

void ContactDialog::addPhoneNumberField() {
    // Создаём новое поле для телефона
    auto* newPhoneNumberEdit = new QLineEdit(this);
    newPhoneNumberEdit->setPlaceholderText(QStringLiteral("Новый телефон"));

    phoneNumbersEdits.append(newPhoneNumberEdit);
    phoneNumbersLayout->insertWidget(phoneNumbersLayout->count() - 1, newPhoneNumberEdit);

    updateRemoveButtonVisibility();
}

void ContactDialog::removePhoneNumberField() {
    // Удаляем последнее поле телефона, если их больше одного
    if (phoneNumbersEdits.size() > 1) {
        auto* lastPhoneNumberEdit = phoneNumbersEdits.takeLast();
        phoneNumbersLayout->removeWidget(lastPhoneNumberEdit);
        delete lastPhoneNumberEdit;

        // Обновляем видимость кнопки "-"
        updateRemoveButtonVisibility();
    }
}

void ContactDialog::updateRemoveButtonVisibility() {
    // Показываем кнопку "-", только если есть больше одного поля
    removePhoneButton->setVisible(phoneNumbersEdits.size() > 1);
}

void ContactDialog::accept() {
    // Валидация данных
    QString firstName = firstNameEdit->text().trimmed();
    QString lastName = lastNameEdit->text().trimmed();
    QString middleName = middleNameEdit->text().trimmed();
    QString address = addressEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QDate birthDate = birthDateEdit->date();

    QVector<QString> phonesV;
    for (auto phoneEdit : phoneNumbersEdits) {
        phonesV.append(phoneEdit->text().trimmed());
    }
     

    QRegularExpression nameRegex(R"(^[\p{Lu}][\p{Ll}\p{N}\- ]*[^\-\s]$)");
    QRegularExpression emailRegex(QStringLiteral("(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"));
    QRegularExpression phoneRegex(R"(^(\+?\d{1,3})?[\s\-]?\(?\d{3}\)?[\s\-]?\d{3}[\s\-]?\d{2}[\s\-]?\d{2}$)");

    if (!nameRegex.match(firstName).hasMatch()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректное имя."));
        return;
    }

    if (!nameRegex.match(lastName).hasMatch()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректная фамилия."));
        return;
    }

    if (!nameRegex.match(middleName).hasMatch()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректное отчество."));
        return;
    }

    if (!nameRegex.match(address).hasMatch()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректный адрес."));
        return;
    }

    if (birthDate >= QDate::currentDate()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректная дата."));
        return;
    }

    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректный email."));
        return;
    }

    for (auto phone : phonesV) {
        if (!phoneRegex.match(phone).hasMatch()) {
            QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Некорректный телефон."));
            return;
        }
    }

    for (auto& phone : phonesV) {
        phone = phone.replace(QRegularExpression(R"((?<!^)\D)"), "");
    }

    // Установка данных
    contact.firstName = firstName;
    contact.lastName = lastName;
    contact.middleName = middleName;
    contact.address = address;
    contact.birthDate = birthDate;
    contact.email = email;
    contact.phoneNumbers = phonesV;

    QDialog::accept();
}
