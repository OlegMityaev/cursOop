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

    addButton = new QPushButton("Add", this);
    editButton = new QPushButton("Edit", this);
    removeButton = new QPushButton("Delete", this);
    saveButton = new QPushButton("Save", this);
    loadButton = new QPushButton("Load", this);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(removeButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);

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
        QMessageBox::warning(this, "Error", "Select an entry to edit.");
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
        QMessageBox::warning(this, "Error", "Select an entry to delete.");
        return;
    }
    model->removeContact(index.row());
}

void MainWidget::saveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save file", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        if (!model->saveToFile(fileName)) {
            QMessageBox::critical(this, "Error", "Failed to save the file.");
        }
    }
}

void MainWidget::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        if (!model->loadFromFile(fileName)) {
            QMessageBox::critical(this, "Error", "Failed to load the file.");
        }
    }
}

