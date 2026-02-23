#include <iostream>
#include <string>
#include "database.h"

bool insertExpense(double amount, const std::string& category,
                   const std::string& description, const std::string& date) {
    const char* sql = "INSERT INTO expenses(amount, category, description, date) VALUES(?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

int callback(void* , int argc, char** argv, char** colName) {
    for (int i = 0; i < argc; ++i) {
        std::cout << (colName[i] ? colName[i] : "") << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    std::cout << "\n";
    return 0;
}

void viewAll() {
    char* err = nullptr;
    if (sqlite3_exec(db, "SELECT * FROM expenses;", callback, nullptr, &err) != SQLITE_OK) {
        std::cerr << "Select error: " << (err ? err : "unknown") << "\n";
        sqlite3_free(err);
    }
}

int main() {
    if (!openDatabase("expenses.db")) return 1;
    if (!createTable()) { closeDatabase(); return 1; }

    // quick test insert
    if (!insertExpense(25.5, "Groceries", "Milk and eggs", "2026-02-23")) {
        std::cerr << "Insert failed\n";
    }

    std::cout << "Current rows:\n";
    viewAll();

    closeDatabase();
    return 0;
}