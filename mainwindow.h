#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include "library.h"
#include "addbookdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Library library;

    void add_book();
    void list_books();

    void on_addBookButton_clicked();
    void on_removeBookButton_clicked();
    void on_sortBooksButton_clicked();
    void on_filterBooksButton_clicked();
    void on_bookPropertiesButton_clicked();
    void on_borrowBookButton_clicked();

    QString current_sort_by = QString("id");
    QString current_filter_by = QString("book_name");
    QString current_filter_string = QString("");

    void list_readers();

    QPointer<AddBookDialog> add_book_dialog_ptr;

    bool createRemoveConfirmDialog();

private slots:
    void add_book_accepted();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
