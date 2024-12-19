#pragma once

#include <QtWidgets/QWidget>
#include "contactmodel.h"

class QTableView;
class QPushButton;

class MainWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainWidget(QWidget* parent = nullptr);

private slots:
    void addContact();
    void editContact();
    void removeContact();
    void saveToFile();
    void loadFromFile();

private:
    ContactModel* model;
    QTableView* tableView;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* removeButton;
    QPushButton* saveButton;
    QPushButton* loadButton;
};


