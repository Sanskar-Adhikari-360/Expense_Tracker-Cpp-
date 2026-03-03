# Expense Tracker built using C++

## A fully function CLI expense tracker built on C++ with features for plotting expense Graph and exporting expenses in CSV/JSON

This is a CLI expense tracker primarily built using C++ and a python script for plotting graphs based on the expenses. It uses SQLite as the database for storing user expenses. This Expense tracker offers features like

- CRUD Operations: Effortlessly insert, view, and delete expenses through a clean CLI.

- Data Persistence: Uses SQLite for lightweight, reliable, and serverless data storage.

- Data Portability: Export your records into CSV or JSON formats for use in Excel or other tools.

- Visual Insights: Generates graphical trends using Matplotlib and Pandas via an integrated Python bridge.

## Requirments

Before installing this project you must have the following installed:

- A C++ compiler (GCC/Clang)

- SQLite3 development libraries

- Python 3.x

- Python dependencies:
  For pip
  `pip install pandas matplotlib`
  For uv
  `uv add pandas matplotlib`

## Installing

1. Clone the repository:

   ```git clone https://github.com/yourusername/expense-tracker.git
    cd expense-tracker
   ```

2. Compile the C++ source:

   ```
    g++ main.cpp -lsqlite3 -o expense-tracker
   ```

3. Run the application:
   ```
   ./expense-tracker
   ```
