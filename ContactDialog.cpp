#include "ContactDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QMessageBox>



ContactDialog::ContactDialog(Contact& contact, QWidget* parent)
    : QDialog(parent), contact(contact) {
    setWindowTitle("Editing a contact");

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

    phoneNumbersEdit = new QLineEdit(this);
    phoneNumbersEdit->setText(QStringList::fromVector(contact.phoneNumbers).join(", "));

    auto* formLayout = new QFormLayout;
    formLayout->addRow("First Name", firstNameEdit);
    formLayout->addRow("Last Name", lastNameEdit);
    formLayout->addRow("Middle Name", middleNameEdit);
    formLayout->addRow("Address", addressEdit);
    formLayout->addRow("Birth Date", birthDateEdit);
    formLayout->addRow("Email", emailEdit);
    formLayout->addRow("Phone numbers", phoneNumbersEdit);

    auto* buttonBox = new QHBoxLayout;
    auto* okButton = new QPushButton("Ok", this);
    auto* cancelButton = new QPushButton("Cancel", this);

    buttonBox->addWidget(okButton);
    buttonBox->addWidget(cancelButton);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonBox);

    connect(okButton, &QPushButton::clicked, this, &ContactDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &ContactDialog::reject);
}

void ContactDialog::accept() {
    // Валидация данных
    QRegularExpression nameRegex(R"(^[A-ZА-Я][a-zа-яёЁ\s-]*[a-zа-яёЁ]$)");
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpression phoneRegex(R"(^(\+?\d{1,3})?[\s\-]?\(?\d{1,4}\)?[\s\-]?\d{1,9}([\s\-]?\d{1,4})?$)");

    if (!nameRegex.match(firstNameEdit->text()).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid name.");
        return;
    }

    if (!emailRegex.match(emailEdit->text()).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid email.");
        return;
    }

    // Установка данных
    contact.firstName = firstNameEdit->text().trimmed();
    contact.lastName = lastNameEdit->text().trimmed();
    contact.middleName = middleNameEdit->text().trimmed();
    contact.address = addressEdit->text().trimmed();
    contact.birthDate = birthDateEdit->date();
    contact.email = emailEdit->text().trimmed();
    contact.phoneNumbers = phoneNumbersEdit->text().split(", ").toVector();

    QDialog::accept();
}
