#pragma once
#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include <QPushButton>
#include "contactmodel.h"

class QLineEdit;
class QDateEdit;

class ContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit ContactDialog(Contact& contact, QWidget* parent = nullptr);

private slots:
    void accept() override;
    void addPhoneNumberField();
    void removePhoneNumberField();

private:
    void updateRemoveButtonVisibility(); // Обновить видимость кнопки "-"

    Contact& contact;
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* middleNameEdit;
    QLineEdit* addressEdit;
    QDateEdit* birthDateEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneNumbersEdit;

    QVBoxLayout* phoneNumbersLayout; // Лейаут для хранения всех полей телефонов
    QHBoxLayout* phoneButtonsLayout;
    QPushButton* addPhoneButton;     // Кнопка для добавления телефона
    QPushButton* removePhoneButton;  // Кнопка для удаления телефона
    QVector<QLineEdit*> phoneNumbersEdits;
};

