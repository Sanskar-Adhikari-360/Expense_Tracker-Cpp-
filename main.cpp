#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <fstream>
#include "database.h"

class ExpenseManager {
public:
    void addExpense() {
        double amount;
        std::string category, description, date;

        std::cout << "Enter amount: ";
        std::cin >> amount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     // this code is to prevent displaying of numbers in scientific notation

        // getline(...) is to allow spaces in our input

        std::cout << "Enter category (Food/Groceries/Travel): ";
        std::getline(std::cin, category);

        std::cout << "Enter description: ";
        std::getline(std::cin, description);

        std::cout << "Enter date (YYYY/MM/DD): ";
        std::getline(std::cin, date);

        const char* sql = "INSERT INTO expenses(amount, category, description, date) VALUES(?,?,?,?)"; // the VALUES(?,?,?,?) is the placeholder for our actual values   and our 'sql' is actually a constant read only pointer
        sqlite3_stmt* stmt; // sqlite3_stmt is a sqlite3 libary function that represents compiled version of sql query that sqlite can execute  and stmt is the pointer that stores the memory address of our prepeared SQL statment

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) { // turns sql string into a prepeared statment
            std::cerr << "Failed to prepare statement\n";
            return;
        }


        // binds our values to our placeholder VALUES(?,?,?,?)
        sqlite3_bind_double(stmt, 1, amount);
        sqlite3_bind_text(stmt, 2, category.c_str(),    -1, SQLITE_TRANSIENT); // SQLITE_TRANSIENT tells to make copy of our string basically
        sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, date.c_str(),        -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE)  // executes the SQL query
            std::cerr << "Failed to add expense\n";
        else
            std::cout << "Expense added successfully!\n";

        sqlite3_finalize(stmt); // frees allocated memory basically
    }

    void viewAll() {
        char* err = nullptr;
        if (sqlite3_exec(db, "SELECT * FROM expenses;", callback, nullptr, &err) != SQLITE_OK) {
            std::cerr << "Select error: " << (err ? err : "unknown") << "\n";
            sqlite3_free(err); // frees the memory allocated for the error message
        }  
    }

    void deleteExpense() {
        int id;
        std::cout << "Enter the expense ID to delete: ";
        std::cin >> id;

        const char* sql = "DELETE FROM expenses WHERE id = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare delete statement\n";
            return;
        }

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
            std::cerr << "Failed to delete expense\n";
        else if (sqlite3_changes(db) == 0) // checks how many rows were changed
            std::cout << "No expense found with ID " << id << "\n";
        else
            std::cout << "Expense deleted successfully!\n";

        sqlite3_finalize(stmt);
    }

    void exportToCSV() {
        std::ofstream file("expense.csv"); // creates and opens the file for writing 
        if (!file.is_open()) {
            std::cerr << "Failed to create CSV file\n";
            return;
        }

        const char* sql = "SELECT * FROM expenses";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Query failed\n";
            return;
        }

        file << "Id,Amount,Category,Description,Date\n";
        file << std::fixed << std::setprecision(2); // makes so numbers like 20.5 becomes 20.50

        // loops through our table and insterts them
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            file << sqlite3_column_int(stmt, 0)    << ","
                 << sqlite3_column_double(stmt, 1)  << ","
                 << sqlite3_column_text(stmt, 2)    << ","
                 << sqlite3_column_text(stmt, 3)    << ","
                 << sqlite3_column_text(stmt, 4)    << "\n";
        }

        sqlite3_finalize(stmt);
        std::cout << "Expenses exported to expense.csv\n";
    }

    void exportToJSON() {
        std::ofstream file("expense.json");
        if (!file.is_open()) {
            std::cerr << "Failed to create JSON file\n";
            return;
        }

        const char* sql = "SELECT * FROM expenses";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Query failed\n";
            return;
        }

        file << std::fixed << std::setprecision(2);
        file << "[\n";

        bool first = true;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) file << ",\n";
            first = false;

            file << "  {\n"
                 << "    \"id\": "          << sqlite3_column_int(stmt, 0)    << ",\n"
                 << "    \"amount\": "      << sqlite3_column_double(stmt, 1)  << ",\n"
                 << "    \"category\": \""  << sqlite3_column_text(stmt, 2)    << "\",\n"
                 << "    \"description\": \""<< sqlite3_column_text(stmt, 3)   << "\",\n"
                 << "    \"date\": \""      << sqlite3_column_text(stmt, 4)    << "\"\n"
                 << "  }";
        }

        file << "\n]\n";
        sqlite3_finalize(stmt);
        std::cout << "Expenses exported to expense.json\n";
    }

    void plotGraph() {
        std::cout << "Generating graph...\n";
        exportToCSV();
        if (system("\".venv\\Scripts\\python.exe\" plot_expenses.py") != 0) {
            std::cerr << "Error running Python script.\n"
                      << "Make sure Python is installed and the virtual environment is set up.\n";
        }
    }

private:
    // the callback runs for every row returned by sqlite3_exec Source: Trust me bro
    static int callback(void*, int argc, char** argv, char** colName) {
        for (int i = 0; i < argc; i++)
            std::cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
        std::cout << "\n";
        return 0;
    }
};

int main() {
    if (!openDatabase("expenses.db")) return 1;
    if (!createTable()) { closeDatabase(); return 1; }

    ExpenseManager manager;
    int choice;

    while (true) {
        std::cout << "\n--- Expense Manager ---\n"
                  << "1. Add Expense\n"
                  << "2. View All Expenses\n"
                  << "3. Delete Expense\n"
                  << "4. Export to CSV\n"
                  << "5. Export to JSON\n"
                  << "6. Plot Graph\n"
                  << "7. Exit\n"
                  << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
            case 1: manager.addExpense();    break;
            case 2: manager.viewAll();       break;
            case 3: manager.deleteExpense(); break;
            case 4: manager.exportToCSV();   break;
            case 5: manager.exportToJSON();  break;
            case 6: manager.plotGraph();     break;
            case 7:
                std::cout << "Exiting program...\n";
                closeDatabase();
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
                break;
        }
    }
}