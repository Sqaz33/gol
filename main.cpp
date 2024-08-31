#include <iostream>

#include "pqxx/pqxx"

int main() {
    pqxx::connection con("dbname=goldb user=postgres password=****");
    
    pqxx::transaction tx(con);

    auto val = tx.query_value<int>("select val from test limit 1");

    std::cout << val;
}