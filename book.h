#ifndef BOOK_H
#define BOOK_H

#include <QSqlDatabase>
#include <QVariant>
#include <QSqlQuery>
#include <QDate>

class Book
{
private:
    QString db_uuid;

public:
    Book();

    std::vector<QString> state_names = {"AVAILABLE", "BORROWED"};
    int state;

    QString name;
    QString author;
    QString group;
    QString category;
    QString publisher;
    QDate publishing_date;

    void init_from_sql_query(QSqlQuery q);
    QString get_state_name();

    QString get_book_uuid(){
        return db_uuid;
    }

    void set_book_uuid(QString uuid){
        this->db_uuid = uuid;
    }
};

#endif // BOOK_H
