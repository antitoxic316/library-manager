#ifndef READER_H
#define READER_H

#include <QVariant>
#include <QSqlQuery>

class Reader
{
private:
    QString name;
    QString last_name;
    bool present;

public:
    QString get_name(){
        return name;
    }

    QString get_last_name(){
        return last_name;
    }

    bool is_present(){
        return present;
    }

    void init_from_sql_query(QSqlQuery q);

    Reader();
};

#endif // READER_H
