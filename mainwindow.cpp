#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

bool MainWindow::createRemoveConfirmDialog(){
    QMessageBox msgBox(this);

    QString msg_str = QString("are you sure you want to remove selected book?");

    msgBox.setText(msg_str);
    msgBox.setInformativeText("This action in permanent");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
    msgBox.setDefaultButton(QMessageBox::Discard);

    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        return true;
    } else {
        return false;
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addBookButton, &QPushButton::clicked, this, &MainWindow::on_addBookButton_clicked);
    connect(ui->removeBookButton, &QPushButton::clicked, this, &MainWindow::on_removeBookButton_clicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::on_sortBooksButton_clicked);
    connect(ui->propertiesButton, &QPushButton::clicked, this, &MainWindow::on_bookPropertiesButton_clicked);
    connect(ui->borrowBookButton, &QPushButton::clicked, this, &MainWindow::on_borrowBookButton_clicked);

    connect(ui->searchByComboBox, &QComboBox::currentTextChanged, this, &MainWindow::on_filterBooksButton_clicked);
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_filterBooksButton_clicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_filterBooksButton_clicked);


    this->library.fetch_books(this->current_sort_by, this->current_filter_by, this->current_filter_string);
    this->list_books();

    ui->booksTableWidget->verticalHeader()->setVisible(false);
    ui->booksTableWidget->setColumnHidden(7, true); // id_hidden column for database communication
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::list_books(){
    this->library.fetch_books(this->current_sort_by, this->current_filter_by, this->current_filter_string);

    this->ui->booksTableWidget->setRowCount(this->library.books.size());

    this->ui->booksTableWidget->clearContents();

    for(int i = 0; i < this->library.books.size(); i++){
        Book current_book = this->library.books[i];

        QTableWidgetItem *name_item = new QTableWidgetItem(current_book.name);
        QTableWidgetItem *author_item = new QTableWidgetItem(current_book.author);
        QTableWidgetItem *group_item = new QTableWidgetItem(current_book.group);
        QTableWidgetItem *category_item = new QTableWidgetItem(current_book.category);
        QTableWidgetItem *publisher_item = new QTableWidgetItem(current_book.publisher);
        QTableWidgetItem *publish_date_item = new QTableWidgetItem(current_book.publishing_date.toString("yyyy.MM.dd"));
        QTableWidgetItem *state_item = new QTableWidgetItem(current_book.get_state_name());

        QTableWidgetItem *uuid_hidden = new QTableWidgetItem(current_book.get_book_uuid());

        this->ui->booksTableWidget->setItem(i, 0, name_item);
        this->ui->booksTableWidget->setItem(i, 1, author_item);
        this->ui->booksTableWidget->setItem(i, 2, group_item);
        this->ui->booksTableWidget->setItem(i, 3, category_item);
        this->ui->booksTableWidget->setItem(i, 4, publisher_item);
        this->ui->booksTableWidget->setItem(i, 5, publish_date_item);
        this->ui->booksTableWidget->setItem(i, 6, state_item);

        this->ui->booksTableWidget->setItem(i, 7, uuid_hidden);
    }
}

void MainWindow::on_addBookButton_clicked(){
    if (!this->add_book_dialog_ptr.isNull()) this->add_book_dialog_ptr->deleteLater();

    this->add_book_dialog_ptr = QPointer(new AddBookDialog(this));
    this->add_book_dialog_ptr ->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    connect(this->add_book_dialog_ptr, &AddBookDialog::add_book_accepted, this, &MainWindow::add_book_accepted);

    this->add_book_dialog_ptr->show();
}

void MainWindow::on_removeBookButton_clicked(){
    if (!this->createRemoveConfirmDialog()){
        return;
    }

    int row_i = this->ui->booksTableWidget->currentRow();

    if(row_i == -1){
        qInfo() << "row not selected, abort";
        return;
    }

    QString id_db = this->ui->booksTableWidget->item(row_i, 7)->text();

    bool ok = this->library.remove_book(id_db);

    if(ok){
        this->ui->booksTableWidget->removeRow(row_i);
    } else {
        qInfo() << "something went wrong, sorry";
    }

    this->list_books();
}

void MainWindow::add_book_accepted(){
    Book b;

    b.name = this->add_book_dialog_ptr->current_name;
    b.author = this->add_book_dialog_ptr->current_author;
    b.group = this->add_book_dialog_ptr->current_group;
    b.category = this->add_book_dialog_ptr->current_category;
    b.publisher = this->add_book_dialog_ptr->current_publisher;
    b.publishing_date = this->add_book_dialog_ptr->current_publishdate;

    this->library.add_book(b);

    this->list_books();
}


void MainWindow::on_sortBooksButton_clicked(){
    this->current_sort_by = this->ui->sortByComboBox->currentText();

    qInfo() << this->current_sort_by;

    this->list_books();
}

void MainWindow::on_filterBooksButton_clicked(){
    this->current_filter_by = this->ui->searchByComboBox->currentText();
    this->current_filter_string = this->ui->searchLineEdit->text();

    qInfo() << this->current_filter_by;
    qInfo() << this->current_filter_string;

    this->list_books();
}

void MainWindow::on_bookPropertiesButton_clicked(){
    list_books();
    if (!this->change_book_dialog_ptr.isNull()) this->change_book_dialog_ptr->deleteLater();

    //CHAAAA
    this->change_book_dialog_ptr = QPointer(new AddBookDialog(this));
    this->change_book_dialog_ptr ->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    connect(this->change_book_dialog_ptr, &AddBookDialog::add_book_accepted, this, &MainWindow::add_book_accepted);

    this->change_book_dialog_ptr->show();
}

void MainWindow::new_properties_accepted(){

}

void MainWindow::on_borrowBookButton_clicked(){
    this->library->change_book_property(QString field_name, QString uuid, QVariant new_value);

    list_books();
}

void MainWindow::list_readers(){
    this->library.fetch_readers();

    this->ui->readersTableWidget->setRowCount(this->library.readers.size());

    this->ui->readersTableWidget->clearContents();

    for(int i = 0; i < this->library.readers.size(); i++){
        Reader current_reader = this->library.readers[i];

        QTableWidgetItem *name_item = new QTableWidgetItem(current_reader.get_name());
        QTableWidgetItem *last_name_item = new QTableWidgetItem(current_reader.get_last_name());
        QTableWidgetItem *present_item = new QTableWidgetItem(current_reader.is_present());

        this->ui->readersTableWidget->setItem(i, 0, name_item);
        this->ui->readersTableWidget->setItem(i, 1, last_name_item);
        this->ui->readersTableWidget->setItem(i, 2, present_item);
    }
}
