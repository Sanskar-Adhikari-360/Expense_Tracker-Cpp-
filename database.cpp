#include "database.h"
#include <iostream>

sqlite3* db = nullptr;

bool openDatabase(const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

bool createTable() {
    const char* sql =
      "CREATE TABLE IF NOT EXISTS expenses ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "amount REAL NOT NULL,"
      "category TEXT NOT NULL,"
      "description TEXT,"
      "date TEXT);";

    char* err = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << (err ? err : "unknown") << "\n";
        sqlite3_free(err);
        return false;
    }
    return true;
}

void closeDatabase() {
    if (db) sqlite3_close(db);
}