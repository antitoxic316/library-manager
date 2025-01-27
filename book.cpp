#include "book.h"

Book::Book() {
    this->state = 0;
}

void Book::init_from_sql_query(QSqlQuery q){
    this->name = q.value(1).toString();
    this->author = q.value(2).toString();
    this->group = q.value(3).toString();
    this->category = q.value(4).toString();
    this->publisher = q.value(5).toString();
    this->publishing_date = q.value(6).toDate();

    this->state = q.value(7).toInt();

    this->set_book_uuid(q.value(0).toString());
}

QString Book::get_state_name(){
    if(this->state > this->state_names.size()-1){
        return QString("");
    }

    return this->state_names[state];
}
