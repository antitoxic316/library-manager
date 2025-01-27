#include "library.h"

#include <QDebug>
#include <QSqlError>

Library::Library() {
    this->db_handler.db_connect();
}

void Library::fetch_books(QString sort_by, QString filter_by, QString filter_str){
    this->books.clear();

    QSqlQuery query_result = this->db_handler.make_books_select_query(sort_by, filter_by, filter_str);

    while(query_result.next()){
        Book b;
        b.init_from_sql_query(query_result);
        this->books.push_back(b);
    }
}

bool Library::add_book(Book book){
    bool ok = this->db_handler.add_book_query(book);
    if(!ok){
        qInfo("book adding failed");
        return ok;
    }

    this->books.push_back(book);
    return ok;
}

bool Library::remove_book(QString id){
    bool query_ok = this->db_handler.remove_book_query(id);
    if (!query_ok){
        qInfo() << "aborting book removal, query failed";
        return query_ok;
    }

    bool removed_ok = false;
    for(int i = 0; i < this->books.size(); i++){
        qInfo() << this->books[i].get_book_uuid();
        qInfo() << id;
        if (this->books[i].get_book_uuid() == id){
            this->books.erase(this->books.begin()+i);
            removed_ok = true;
            break;
        }
    }

    return removed_ok;
}

void Library::fetch_readers(){
    this->readers.clear();

    QSqlQuery query_result = this->db_handler.make_readers_select_query();

    while(query_result.next()){
        Reader r;
        r.init_from_sql_query(query_result);
        this->readers.push_back(r);
    }
}

bool Library::change_book_properties(QString field_name, QString uuid, QVariant new_value){
    bool ok = this->db_handler.change_book_field_value_query(field_name, uuid, new_value);

    return ok;
}

bool Library::borrow_book(QString uuid){
    bool ok = this->db_handler.change_book_field_value_query("state", uuid, 1);

    return ok;
}
