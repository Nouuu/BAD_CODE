#include <stdio.h>
#include <string.h>
#include "headers/sqlite/sqlite3.h"

int main(void) {
    printf("SQLite version: %s\n", sqlite3_version);
    sqlite3 *db;
    sqlite3_stmt *request;
    char *sqlSelectStudents = "SELECT * FROM students";
    int id = 0;
    char *first_name = NULL;
    char *last_name = NULL;
    char *class = NULL;
    char ErrMsg = 0;

    int returnCode = sqlite3_open("test.db", &db);
    if (returnCode) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    } else {
        fprintf(stdout, "Opened/Created database 'test.db' successfully\n");
    }

    fprintf(stdout, "Recuperation de tout les eleves\n");

    returnCode = sqlite3_prepare_v2(db, sqlSelectStudents, strlen(sqlSelectStudents), &request, NULL);
    if (!returnCode) {
        while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
            returnCode = sqlite3_step(request);
            if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
                id = sqlite3_column_int(request, 0);
                first_name = (char *) sqlite3_column_text(request, 1);
                last_name = (char *) sqlite3_column_text(request, 2);
                class = (char *) sqlite3_column_text(request, 3);

                fprintf(stdout, "| %d\t| %s\t| %s\t| %s\t|\n", id, first_name, last_name, class);
            }
        }
        sqlite3_finalize(request);
    }

    sqlite3_close(db);

    return 0;
}