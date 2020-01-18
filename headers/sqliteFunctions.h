//
// Created by MaleWhere on 06/01/2020.
//

#ifndef BAD_CODE_SQLITEFUNCTIONS_H
#define BAD_CODE_SQLITEFUNCTIONS_H

void checkVersion() {
    printf("SQLite version: %s\n", sqlite3_version);
}

sqlite3 *connectDB(char *dbname) {
    sqlite3 *db;
    int returnCode = sqlite3_open(dbname, &db);
    if (returnCode) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stdout, "Opened/Created database 'test.db' successfully\n");
    }
    return db;
}

void getAllStudents(sqlite3 *db){
    sqlite3_stmt *request;
    char *sqlSelectStudents = "SELECT * FROM students";
    int id = 0, rows = 0;
    char *first_name = NULL;
    char *last_name = NULL;
    char *class = NULL;
    int returnCode;

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
                rows++;
            }
        }
        sqlite3_finalize(request);
    }
    fprintf(stdout, "Nombre d'eleves : %d", rows);
}


#endif //BAD_CODE_SQLITEFUNCTIONS_H
