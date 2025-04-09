#include "allusers.h"
#include <QPalette>
#include <QHeaderView>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QEvent>
#include "DatabaseManager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mesgboxutil.h"

AllUsers::AllUsers(QWidget *parent,QWidget *topBar) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);

    QWidget *topBarPlaceholder = new QWidget();
    topBarPlaceholder->setFixedHeight(topBar->height());
    mainLayout->addWidget(topBarPlaceholder, 0, Qt::AlignTop);
    setupUI();
    setupHoverEffect();
    setupConnections();


}


void AllUsers::setupUI() {
    this->setMinimumSize(800,500);
    this->setStyleSheet("background-color: #0D1B2A;");  // Dark blue background
    // topBar = new TopBar(this);
    getAllUsers();
    QPushButton *backButton = new QPushButton;
    backButton->setIcon(QIcon("images/back_arrow.png")); // Use an appropriate left arrow icon
    backButton->setIconSize(QSize(30, 30));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; padding: 5px; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 5px; }"
        );

    // **Connect Back Button to Close Form**
    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit backButtonClicked();
    });


    // **All Users Heading**
    heading = new QLabel("All Users", this);
    heading->setStyleSheet("font-size: 22px; color: white; font-weight: bold;");
    heading->setContentsMargins(0,0,0,0);
    heading->setAlignment(Qt::AlignCenter);

    // **Category Selection**
    studentBtn = new QPushButton("Students", this);
    teacherBtn = new QPushButton("Teachers", this);

    hoverBar = new QWidget(this);
    hoverBar->setStyleSheet("background-color: #1E90FF; border-radius: 5px;");
    hoverBar->setFixedHeight(5);

    studentBtn->setStyleSheet(    "QPushButton {"
                              "   background-color: #1b263b; color: white; font-size: 18px; font-weight: bold;"
                              "   border-radius: 7px; padding: 5px 10px; border: 2px solid #415a77;"
                              " }"
                              "QPushButton:hover, QPushButton:checked {"
                              "   background-color: #2d3e50; border-color: #778da9;"
                              "}"
                              "QPushButton:pressed { background-color: #0f1925; }");


    teacherBtn->setStyleSheet(  "QPushButton {"
                              "   background-color: #1b263b; color: white; font-size: 18px; font-weight: bold;"
                              "   border-radius: 7px; padding: 5px 10px; border: 2px solid #415a77;"
                              "   }"
                              "QPushButton:hover, QPushButton:checked {"
                              "   background-color: #2d3e50; border-color: #778da9; "
                              "}"
                              "QPushButton:pressed { background-color: #0f1925; }");


    // **Search Bar & Buttons**
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search user...");
    searchBar->setStyleSheet( "QLineEdit {"
                             "   background-color: #2C3E50;"  // Default color (dark blue)
                             "   color: white;"
                             "   border: 1px solid #34495E;"
                             "   padding: 5px;"
                             "   border-radius: 5px;"
                             "}"
                             "QLineEdit:focus {"
                             "   background-color: #1B263B;"  // Active color (lighter blue)
                             "   border: 1px solid #1B263B;"
                             "}"
                             );
    connect(searchBar, &QLineEdit::textChanged, this, &AllUsers::searchUser);
    searchBar->installEventFilter(this);
    searchBar->setContentsMargins(20,0,20,0);

    refreshBtn = new QPushButton("Refresh", this);
    filterBtn = new QPushButton("Filter", this);
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #1A2B50; color: white; padding: 6px; border-radius: 5px;"
        "     border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0D1B2A; border: 1px solid #FFFFFF;"
        "}";

    refreshBtn->setStyleSheet(buttonStyle);
    filterBtn->setStyleSheet(buttonStyle);
    filterBtn->setContentsMargins(0,0,20,0);

    // **Table Setup**
    table = new QTableWidget(0, 8, this);
    table->setHorizontalHeaderLabels({"ID", "Name", "Gender", "Department","Email","Phone","Fee Status","Actions"});
    table->setStyleSheet("background-color: #1B263B; color: white; font-size: 14px;");
    table->horizontalHeader()->setStyleSheet("background-color: #162C5D; color: white; font-weight: bold;");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    populateTable(students);  // Show students by default


    totalUsersLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: white; background-color:#1B263B;");


    QHBoxLayout *categoryLayout = new QHBoxLayout();
    categoryLayout->addWidget(studentBtn);
    categoryLayout->addWidget(teacherBtn);
    categoryLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(refreshBtn);
    searchLayout->addWidget(filterBtn);




    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);
    mainLayout->addWidget(heading);
    mainLayout->addLayout(categoryLayout);
    mainLayout->addWidget(hoverBar);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(table);
    mainLayout->addWidget(totalUsersLabel, 0, Qt::AlignLeft);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}
void AllUsers::getAllUsers() {
    QSqlDatabase db = DatabaseManager::getInstance().getDatabase();
    if (!db.isOpen()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Database connection is not open!",
            "Database Error",
            "OK"
            );
        return;
    }

    QSqlQuery teacherQuery;
    teacherQuery.prepare("SELECT unique_id, first_name, last_name,email,gender,department,fee_status,phone FROM vls_schema.users WHERE role = 'teacher'");
    if (!teacherQuery.exec()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Failed to retrieve users: " + teacherQuery.lastError().text(),
            "Database Error",
            "OK"
            );
        return;
    }

    while (teacherQuery.next()) {
        QString uniqueId = teacherQuery.value("unique_id").toString();
        QString firstName = teacherQuery.value("first_name").toString();
        QString lastName = teacherQuery.value("last_name").toString();
        QString fullName = firstName + " " + lastName;
        QString email = teacherQuery.value("email").toString();
        QString gender = teacherQuery.value("gender").toString();
        QString phone = teacherQuery.value("phone").toString();

        qDebug() << "Gender fetched:" << gender;

        // Format and store the data
        teachers << QString("%1, %2 , %3, %4, %5")
                        .arg(uniqueId, fullName, gender, email, phone);
    }

    totalUsersLabel = new QLabel("Total Users: 0", this);

    QSqlQuery studentQuery;
    studentQuery.prepare("SELECT unique_id, first_name, last_name,email,gender,department,fee_status,phone FROM vls_schema.users WHERE role = 'student'");
    if (!studentQuery.exec()) {
        MessageBoxUtil::showCustomMessage(
            this,
            "Failed to retrieve users: " + studentQuery.lastError().text(),
            "Database Error",
            "OK"
            );
        return;
    }

    while (studentQuery.next()) {
        QString uniqueId = studentQuery.value("unique_id").toString();
        QString firstName = studentQuery.value("first_name").toString();
        QString lastName = studentQuery.value("last_name").toString();
        QString fullName = firstName + " " + lastName;
        QString email = studentQuery.value("email").toString();
        QString gender = studentQuery.value("gender").toString();
        QString department = studentQuery.value("department").toString();
        QString feeStatus = studentQuery.value("fee_status").toString();
        QString phone = studentQuery.value("phone").toString();

        // Format and store the data
        students << QString("%1, %2, %3, %4, %5,%6,%7")
                        .arg(uniqueId, fullName, gender, department, email, phone, feeStatus);
    }
}

void AllUsers::setupHoverEffect() {
    // **Hover effect for category selection**
    QPropertyAnimation *animation = new QPropertyAnimation(hoverBar, "geometry");
    animation->setDuration(200);
    connect(studentBtn, &QPushButton::clicked, [=]() {
        animation->setStartValue(hoverBar->geometry());
        animation->setEndValue(QRect(studentBtn->x(), studentBtn->y() + 35, studentBtn->width(), 5));
        animation->start();
        switchCategory("Students");
    });

    connect(teacherBtn, &QPushButton::clicked, [=]() {
        animation->setStartValue(hoverBar->geometry());
        animation->setEndValue(QRect(teacherBtn->x(), teacherBtn->y() + 35, teacherBtn->width(), 5));
        animation->start();
        switchCategory("Teachers");
    });
}

void AllUsers::setupConnections() {
    connect(searchBar, &QLineEdit::textChanged, this, &AllUsers::searchUser);
    connect(refreshBtn, &QPushButton::clicked, this, &AllUsers::refreshTable);
    connect(filterBtn, &QPushButton::clicked, [=]() {
        MessageBoxUtil::showCustomMessage(
            this,
            "Filtering functionality will be added soon!",
            "Filter",
            "OK"
            );
    });
}

void AllUsers::populateTable(const QStringList &data) {
    table->setRowCount(0);  // Clear existing rows

    for (const QString &entry : data) {
        QStringList details = entry.split(",");
        int row = table->rowCount();
        table->insertRow(row);

        // Populate user details
        for (int i = 0; i < details.size(); ++i) {
            table->setItem(row, i, new QTableWidgetItem(details[i]));
        }

        // Create Update button
        QPushButton *updateBtn = new QPushButton("Update");
        updateBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #3CB371;
        color: white;
        border:2px solid #3CB371;
        border-radius: 8px;
        padding : 6px 12px;
        font-weight: semi-bold;
    }
    QPushButton:hover {
        background-color: #1B263B;
    })");
        connect(updateBtn, &QPushButton::clicked, this, [this, row]() { updateUser(row); });

        // Create Delete button
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #FF5C5C;
        color: white;
        border:2px solid #FF5C5C;
        border-radius: 8px;
        padding : 6px 12px;
        font-weight: semi-bold;
    }
    QPushButton:hover {
        background-color: #1B263B;
    })");
        connect(deleteBtn, &QPushButton::clicked, this, [this, row]() { deleteUser(row); });

        // Add buttons to a widget
        QWidget *btnWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(btnWidget);
        layout->addWidget(updateBtn);
        layout->addWidget(deleteBtn);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(5);
        layout->setContentsMargins(0, 0, 0, 0);
        btnWidget->setLayout(layout);

        table->setCellWidget(row, details.size(), btnWidget); // Add buttons to the last column
    }

    updateUserCount(); // Update count after populating table
}

void AllUsers::deleteUser(int row) {
    if (row < 0 || row >= table->rowCount()) return;

    QString userID = table->item(row, 0)->text();
    qDebug() << "row ID: " << userID;

    bool confirm = MessageBoxUtil::showCustomQuestion(
        this,
        "Are you sure you want to delete this user?",
        "Confirm Deletion",
        "Yes, Delete",
        "Nope"
        );

    if (confirm) {
        QSqlQuery query;
        query.prepare("DELETE FROM vls_schema.users WHERE unique_id = :id");
        query.bindValue(":id", userID);

        if (query.exec()) {
            table->removeRow(row);
            updateUserCount();
            MessageBoxUtil::showCustomMessage(
                this,
                "✅ User Deleted Successfully!",
                "Success",
                "OK"
                );
        } else {
            MessageBoxUtil::showCustomMessage(
                this,
                "Failed to delete user from the database.",
                "Error",
                "OK"
                );
        }
    }
}





void AllUsers::updateUser(int row) {
    if (row < 0 || row >= table->rowCount()) return;

    QString newName = QInputDialog::getText(this, "Update User", "Enter new name:");
    if (!newName.isEmpty()) {
        table->item(row, 1)->setText(newName);
        MessageBoxUtil::showCustomMessage(
            this,
            "User updated successfully!",
            "Success",
            "OK"
            );
    }
}




void AllUsers::switchCategory(const QString &category) {
    currentCategory = category;
    if (category == "Students") {
        table->setColumnCount(8); // Adjust columns for Students
        table->setHorizontalHeaderLabels({"ID", "Name", "Gender", "Department", "Email", "Phone", "Fee Status", "Actions"});
        populateTable(students);
    } else if (category == "Teachers") {
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({"ID", "Name", "Gender","Email","Phone","Actions"});
        populateTable(teachers);
    }
    updateUserCount(); // Update count when switching categories
}

void AllUsers::searchUser() {
    QString keyword = searchBar->text().toLower();
    for (int i = 0; i < table->rowCount(); ++i) {
        bool match = table->item(i, 0)->text().toLower().contains(keyword); //i, 0 where 0 is column number which is ID , 1 for Name
        table->setRowHidden(i, !match);
    }
}
/*FOR BOTH NAME AND ID
void AllUsers::searchUser() {
    QString keyword = searchBar->text().toLower();
    for (int i = 0; i < table->rowCount(); ++i) {
        bool idMatch = table->item(i, 0)->text().toLower().contains(keyword);  // Search in ID column (index 0)
        bool nameMatch = table->item(i, 1)->text().toLower().contains(keyword); // Search in Name column (index 1)

        table->setRowHidden(i, !(idMatch || nameMatch));  // Show row if either matches
    }
}*/

void AllUsers::refreshTable() {
    searchBar->clear();
    table->clear();
    students.clear();
    teachers.clear();
    table->setRowCount(0);
    getAllUsers();
    switchCategory(currentCategory);
}
bool AllUsers::eventFilter(QObject *obj, QEvent *event) {
    if (obj == searchBar) {
        if (event->type() == QEvent::Enter) {
            searchBar->setStyleSheet(
                "background-color: #1B263B; color: white; padding: 6px; border-radius: 5px;"
                "border: 1px solid white;"
                );
        } else if (event->type() == QEvent::Leave) {
            searchBar->setStyleSheet(
                "background-color: #1A2B50; color: white; padding: 6px; border-radius: 5px;"
                "border: 1px solid transparent;"
                );
        }
    }
    return QWidget::eventFilter(obj, event);
}
void AllUsers::updateUserCount() {
    if (!table) return; // Ensure table exists
    int rowCount = table->rowCount();
    totalUsersLabel->setText("Total Users: " + QString::number(rowCount));
}
