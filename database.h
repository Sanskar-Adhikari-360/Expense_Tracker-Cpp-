#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

extern sqlite3* db;

bool openDatabase(const std::string& filename = "expenses.db");
void closeDatabase();
bool createTable();

#endif