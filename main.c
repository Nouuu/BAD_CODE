#include <stdio.h>
#include <string.h>
#include "headers/sqlite/sqlite3.h"
#include "headers/sqliteFunctions.h"

int main(void) {
    checkVersion();
    sqlite3 *db;

    connectDB("tests.db", &db);
    getAllStudents(db);

    sqlite3_close(db);

    return 0;
}