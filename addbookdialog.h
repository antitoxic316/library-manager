#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    ~AddBookDialog();

    QString current_name;
    QString current_author;
    QString current_group;
    QString current_category;
    QString current_publisher;
    QDate current_publishdate;

public slots:
    void accept();
signals:
    void add_book_accepted();

private:
    Ui::AddBookDialog *ui;

    bool createEmptyFieldConfirmDialog(std::string field_name);
};

#endif // ADDBOOKDIALOG_H
