#pragma once
#include <QDialog>
#include "contactmodel.h"

class QLineEdit;
class QDateEdit;

class ContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit ContactDialog(Contact& contact, QWidget* parent = nullptr);

private slots:
    void accept() override;

private:
    Contact& contact;
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* middleNameEdit;
    QLineEdit* addressEdit;
    QDateEdit* birthDateEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneNumbersEdit;
};

