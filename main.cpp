#include <iostream>
#include <string>
#include "database.h"

// bool insertExpense(double amount, const std::string& category,
//                    const std::string& description, const std::string& date) {
//     const char* sql = "INSERT INTO expenses(amount, category, description, date) VALUES(?, ?, ?, ?);";
//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << "\n";
//         return false;
//     }

//     sqlite3_bind_double(stmt, 1, amount);
//     sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);

//     int rc = sqlite3_step(stmt);
//     if (rc != SQLITE_DONE) {
//         std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
//         sqlite3_finalize(stmt);
//         return false;
//     }

//     sqlite3_finalize(stmt);
//     return true;
// }

// int callback(void* , int argc, char** argv, char** colName) {
//     for (int i = 0; i < argc; ++i) {
//         std::cout << (colName[i] ? colName[i] : "") << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
//     }
//     std::cout << "\n";
//     return 0;
// }

// void viewAll() {
//     char* err = nullptr;
//     if (sqlite3_exec(db, "SELECT * FROM expenses;", callback, nullptr, &err) != SQLITE_OK) {
//         std::cerr << "Select error: " << (err ? err : "unknown") << "\n";
//         sqlite3_free(err);
//     }
// }

class ExpenseClass{
  public:
        void addexpense(){
            double amount;
            std::string category, description, date;
            
            std::cout<<"Enter amount: ";
            std::cin>>amount;

            std::cout<<"Enter catrgory (Food/Groceries/Travel): ";
            std::cin>>category;

            std::cout<<"Enter description: ";
            std::cin>>description;

            std::cout<<"Enter date (YYYY/MM/DD): ";
            std::cin>>date;

            const char* sql =
            "INSERT INTO expenses(amount, category, description, date) VALUES(?, ?, ?, ?);";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cout << "Failed to prepare statement\n";
            return;
        }

        sqlite3_bind_double(stmt, 1, amount);
        sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cout << "Failed to add expense\n";
        } else {
            std::cout << "Expense added successfully!\n";
        }

        sqlite3_finalize(stmt);
    }

    static int callback(void*, int argc, char** argv, char** colName) {
        for (int i = 0; i < argc; i++) {
            std::cout << colName[i] << ": "
                      << (argv[i] ? argv[i] : "NULL") << " | ";
        }
        std::cout << "\n";
        return 0;
        }
};



int main() {
    if (!openDatabase("expenses.db")) return 1;
    if (!createTable()) { closeDatabase(); return 1; }

    // // quick test insert
    // if (!insertExpense(25.5, "Groceries", "Milk and eggs", "2026-02-23")) {
    //     std::cerr << "Insert failed\n";
    // }
    ExpenseClass obj;
    int choice;

    while (true) {
        std::cout << "\n--- Expense Manager ---\n";
        std::cout << "1. Add Expense\n";
        std::cout << "2. View All Expense\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 3) {
            std::cout << "Exiting program...\n";
            break;
        }

        switch (choice) {
            case 1: {
                obj.addexpense();
                break;
            }
            case 2: {

                break;
            }
            case 3:
                
                break;
            case 4: {

                break;
            }
            default:
                std::cout << "Invalid choice! Try again.\n";
                break;
        }
    }


    // std::cout << "Current rows:\n";
    // viewAll();

    closeDatabase();
    return 0;
}