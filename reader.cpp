#include "reader.h"

Reader::Reader() {}

void Reader::init_from_sql_query(QSqlQuery q){
    this->name = q.value(1).toString();
    this->last_name = q.value(2).toString();
    this->present = q.value(5).toBool();
}
