#include "MainWidget.h"
#include "contactdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QFileDialog>
#include <QMessageBox>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent), model(new ContactModel(this)) {

    tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->setSortingEnabled(true);

    addButton = new QPushButton(QStringLiteral("Добавить"), this);
    editButton = new QPushButton(QStringLiteral("Редактировать"), this);
    removeButton = new QPushButton(QStringLiteral("Удалить"), this);
    saveButton = new QPushButton(QStringLiteral("Сохранить"), this);
    loadButton = new QPushButton(QStringLiteral("Загрузить"), this);
    searchEdit = new QLineEdit(this);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(searchEdit);
    layout->addWidget(tableView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(removeButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);

    connect(searchEdit, &QLineEdit::textChanged, model, &ContactModel::filterContacts);
    connect(addButton, &QPushButton::clicked, this, &MainWidget::addContact);
    connect(editButton, &QPushButton::clicked, this, &MainWidget::editContact);
    connect(removeButton, &QPushButton::clicked, this, &MainWidget::removeContact);
    connect(saveButton, &QPushButton::clicked, this, &MainWidget::saveToFile);
    connect(loadButton, &QPushButton::clicked, this, &MainWidget::loadFromFile);
}

void MainWidget::addContact() {
    Contact contact;
    ContactDialog dialog(contact, this);
    if (dialog.exec() == QDialog::Accepted) {
        model->addContact(contact);
    }
}

void MainWidget::editContact() {
    auto index = tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Выделите запись для редактирования."));
        return;
    }
    auto contact = model->getContact(index.row());
    ContactDialog dialog(contact, this);
    if (dialog.exec() == QDialog::Accepted) {
        model->updateContact(index.row(), contact);
    }
}

void MainWidget::removeContact() {
    auto index = tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Выделите запись для удаления."));
        return;
    }
    model->removeContact(index.row());
}

void MainWidget::saveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("Сохранить файл"), "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        if (!model->saveToFile(fileName)) {
            QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Ошибка при сохранении файла."));
        }
    }
}

void MainWidget::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Открыть файл"), "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        if (!model->loadFromFile(fileName)) {
            QMessageBox::critical(this, QStringLiteral("Ошибка"), QStringLiteral("Ошибка при загрузке файла."));
        }
    }
}

