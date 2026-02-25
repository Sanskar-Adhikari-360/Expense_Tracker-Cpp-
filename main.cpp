#include <iostream>
#include <string>
#include "database.h"
#include<limits>
#include <iomanip>
#include <fstream>

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

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout<<"Enter category (Food/Groceries/Travel): ";
            std::getline(std::cin, category);

            std::cout<<"Enter description: ";
            std::getline(std::cin,description);

            std::cout<<"Enter date (YYYY/MM/DD): ";
            std::getline(std::cin, date);

        const char* sql = "INSERT INTO expenses(amount, category, description, date) VALUES(?,?,?,?)";
        
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


        // Expenses view garna ko lagi
        void viewAll() {
            char* err = nullptr;
                if (sqlite3_exec(db, "SELECT * FROM expenses;", callback, nullptr, &err) != SQLITE_OK) {
                    std::cerr << "Select error: " << (err ? err : "unknown") << "\n";
                    sqlite3_free(err);
                }
            }
        
        
        void deleteExpense(){
            int c;
            std::cout<<"Enter the expense id to delete: ";
            std::cin>>c;
            const char* sql = "DELETE FROM expenses WHERE id = ?;";
            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            sqlite3_bind_int(stmt, 1, c);
            sqlite3_step(stmt);
            sqlite3_changes(db);
            sqlite3_finalize(stmt);
        }

        void exportToCSV(){
            std::ofstream file ("expense.csv");
            if (!file.is_open()){
                std::cout<<"Failed to create CSV file \n";
                return;
            }
            file << "Id,amount,category,description,date\n";
            const char* sql = "SELECT * FROM expenses";
            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr) != SQLITE_OK)
            {
                std::cout<<"Query Failed\n";
                return;
            }
            file << std::fixed << std::setprecision(2);
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                file<<sqlite3_column_int(stmt, 0) << ","
            << sqlite3_column_double(stmt, 1) << ","
            << sqlite3_column_text(stmt, 2) << ","
            << sqlite3_column_text(stmt, 3) << ","
            << sqlite3_column_text(stmt, 4) << "\n";
            }
            sqlite3_finalize(stmt);
            file.close();
            std::cout<<"Expenses exported to CSV";  
        }
        void exportToJSON(){
            std::ofstream file ("expense.json");
            if (!file.is_open()){
                std::cout<<"Failed to create JSON file \n";
                return;
            }
            const char* sql = "SELECT * FROM expenses";
            sqlite3_stmt* stmt;
            if (sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr) != SQLITE_OK)
            {
                std::cout<<"Query Failed\n";
                return;
            }
           file << "[\n";
           bool firstComma = true;
           file << std::fixed << std::setprecision(2);
           while (sqlite3_step(stmt) == SQLITE_ROW)
           {
                if (!firstComma)
                {
                    file << ",\n";
                }
                firstComma = false;

                file << "  {\n";
                file << "    \"id\": " << sqlite3_column_int(stmt, 0) << ",\n";
                file << "    \"amount\": " << sqlite3_column_double(stmt, 1) << ",\n";
                file << "    \"category\": \"" << sqlite3_column_text(stmt, 2) << "\",\n";
                file << "    \"description\": \"" << sqlite3_column_text(stmt, 3) << "\",\n";
                file << "    \"date\": \"" << sqlite3_column_text(stmt, 4) << "\"\n";
                file << "  }";
                
           }
           file<<"\n]\n";
           sqlite3_finalize(stmt);
           file.close();

           std::cout<<"Expenses exported to expense.json";
            
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
        std::cout << "3. Delete Expense\n";
        std::cout << "4. Export expense to CSV\n";
        std::cout << "5. Export expense to JSON\n";  
        std::cout << "6. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 6) {
            std::cout << "Exiting program...\n";
            break;
        }

        switch (choice) {
            case 1: {
                obj.addexpense();
                break;
            }
            case 2: {
                obj.viewAll();
                break;
            }
            case 3:
                obj.deleteExpense();
                break;
            case 4: {
                obj.exportToCSV();
                break;
            }
            case 5: {
                obj.exportToJSON();
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