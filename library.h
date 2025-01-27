#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include "book.h"
#include "reader.h"
#include "dbhandler.h"

class Library
{
private:
    DBHandler db_handler;

public:
    std::vector<Book> books;
    std::vector<Reader> readers;

    Library();

    bool add_book(Book book); // add to db and fetch again
    bool remove_book(QString id); // remove from list by s_id and from db
    void fetch_books(QString sort_by, QString filter_by, QString filter_str); // creating Book list // db_handler does the query search
    bool borrow_book(QString uuid);
    bool change_book_properties(QString field_name, QString uuid, QVariant new_value);

    void fetch_readers();
};

#endif // LIBRARY_H
