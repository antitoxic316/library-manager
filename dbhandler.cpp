#include "dbhandler.h"

#include <fstream>

#include <QVariant>
#include <QString>

#include <ctime>

#include <QDebug>
#include <QSqlError>

DBHandler::DBHandler(std::string config_file_path) {
    this->config_path = config_file_path;
    this->parse_credentials();
}

void DBHandler::parse_credentials(){
    std::string line;
    std::ifstream f;

    f.open(this->config_path, std::ifstream::in);

    if (f.is_open()){
        std::string key;
        std::string value;
        while(f.good()){
            f >> line;
            line += '\0';

            int i = 0;
            bool seen_equals_sign = false;
            while(line[i] != '\0'){
                if(line[i] == '='){
                    seen_equals_sign = true;
                    i++;
                    break;
                }
                key += line[i];
                i++;
            }

            if(!seen_equals_sign){
                continue;
            }

            while(i < line.length()){
                value += line[i];
                i++;
            }

            this->db_credentials.at(key) = value;
            line = "";
            key = "";
            value = "";
        }
    }

    f.close();
};

bool DBHandler::db_connect(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(this->db_credentials["hostname"].c_str());
    db.setDatabaseName(this->db_credentials["databasename"].c_str());
    db.setUserName(this->db_credentials["username"].c_str());
    db.setPassword(this->db_credentials["password"].c_str());

    bool ok = db.open();

    qInfo() << db.lastError().text();

    if (ok){
        qInfo("connected");
    } else {
        qInfo("error db");
    }

    return ok;
};

QSqlQuery DBHandler::make_books_select_query(QString sort_by, QString filter_by, QString filter_str){
    QSqlQuery query;

    QString query_str = QString("SELECT * from books WHERE books." + filter_by + " LIKE '%" + filter_str + "%'" + " ORDER BY books." + sort_by);

    query.prepare(query_str);

    bool ok = query.exec();

    if (!ok){
        qInfo("unsuccesfull query; aborting");
        qInfo() << query.lastError().text();
    }

    return query;
}

bool DBHandler::add_book_query(Book book){
    QSqlQuery query;

    query.prepare("INSERT INTO books (id, book_name, author, book_group, category, publisher, publishing_date, state) \
                   VALUES (gen_random_uuid(), :name_v, :author_v, :group_v, :category_v, :publisher_v, :publishing_date_v, :state_v)");
    query.bindValue(":name_v", book.name);
    query.bindValue(":author_v", book.author);
    query.bindValue(":group_v", book.group);
    query.bindValue(":category_v", book.category);
    query.bindValue(":publisher_v", book.publisher);
    query.bindValue(":publishing_date_v", book.publishing_date);
    query.bindValue(":state_v", book.state);

    bool ok = query.exec();

    if (!ok){
        qInfo("unsuccesfull query; aborting");
        qInfo() << query.lastError().text();
        return ok;
    }

    return ok;
}

bool DBHandler::remove_book_query(QString id){
    QSqlQuery query;

    query.prepare("SELECT remove_book(:id)");
    query.bindValue(":id", id);

    bool ok = query.exec();

    if (!ok){
        qInfo("unsuccesfull query; aborting");
        qInfo() << query.lastError().text();
        qInfo() << id;
        return ok;
    }

    return ok;
}

QSqlQuery DBHandler::make_readers_select_query(){
    QSqlQuery query;
    query.prepare("SELECT * from readers");

    bool ok = query.exec();

    if (!ok){
        qInfo("unsuccesfull query; aborting");
        qInfo() << query.lastError().text();
    }

    return query;
}

bool DBHandler::change_book_field_value_query(QString field_name, QString uuid, QVariant new_value){
    QSqlQuery query;

    QString query_str = QString("UPDATE books SET books." + field_name + " = :new_value WHERE books.id = :book_uuid");

    query.prepare(query_str);
    query.bindValue(":new_value", new_value);
    query.bindValue(":book_uuid", uuid);

    bool ok = query.exec();

    if (!ok){
        qInfo("unsuccesfull query; aborting");
        qInfo() << query.lastError().text();
    }

    return ok;
}
