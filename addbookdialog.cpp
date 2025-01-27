#include "addbookdialog.h"
#include "ui_addbookdialog.h"

#include <QMessageBox>
#include <QStyle>

bool createEmptyFieldConfirmDialog(std::string field_name);


AddBookDialog::AddBookDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

void AddBookDialog::accept(){
    this->current_name = this->ui->nameLineEdit->text();
    this->current_author = this->ui->authorLineEdit->text();
    this->current_group = this->ui->groupLineEdit->text();
    this->current_category = this->ui->categoryLineEdit->text();
    this->current_publisher = this->ui->publisherLineEdit->text();
    this->current_publishdate = this->ui->publishDateEdit->date();

    //check for nulls and wrong date
    if(this->current_name.isEmpty()){
        bool res = createEmptyFieldConfirmDialog("name");
        if (!res){
            return;
        }
    }

    if(this->current_author.isEmpty()){
        bool res = createEmptyFieldConfirmDialog("author");
        if (!res){
            return;
        }
    }

    if(this->current_group.isEmpty()){
        bool res = createEmptyFieldConfirmDialog("group");
        if (!res){
            return;
        }
    }

    if(this->current_category.isEmpty()){
        bool res = createEmptyFieldConfirmDialog("category");
        if (!res){
            return;
        }
    }

    if(this->current_publisher.isEmpty()){
        bool res = createEmptyFieldConfirmDialog("publisher");
        if (!res){
            return;
        }
    }

    emit add_book_accepted();

    QDialog::accept();
}

bool AddBookDialog::createEmptyFieldConfirmDialog(std::string field_name){
    QMessageBox msgBox(this);

    QString msg_str;
    msg_str.append(&field_name[0]);
    msg_str.append(" field is empty");

    msgBox.setText(msg_str);
    msgBox.setInformativeText("Do you still wish to add the book?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Discard);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        return true;
    } else {
        return false;
    }
}
