#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <string>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <map>
#include "book.h"

class DBHandler
{
private:
    std::string config_path;
    std::map<std::string, std::string> db_credentials{
        {"hostname", ""},{"databasename", ""},{"username", ""},{"password", ""}
    };

    void parse_credentials();

public:
    DBHandler(std::string config_file_path="/home/dima/.config/.dbconfig");
    bool db_connect();
    QSqlQuery make_books_select_query(QString sort_by, QString filter_by, QString filter_str);
    bool add_book_query(Book book);
    bool remove_book_query(QString id);
    bool change_book_field_value_query(QString field_name, QString uuid, QVariant new_value);

    QSqlQuery make_readers_select_query();
};

#endif // DBHANDLER_H
