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

int insertUserImage(char *dbname, int userId, char *photo_location) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    PARTIE FICHIER /////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    FILE *file = fopen(photo_location, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open image\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    int flen = ftell(file);
    if (flen == -1) {
        fprintf(stderr, "Cannot get filesize");
        fclose(file);
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    char *data = malloc(flen + 1);
    unsigned long long size = fread(data, sizeof(char), flen, file);
    //data contient maintenant tout le binaire du fichier, on peut donc fermer le fichier

    int r = fclose(file);
    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    PARTIE SQL /////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "UPDATE USER SET photo = ? WHERE id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_blob(pStmt, 1, data, (int) size, SQLITE_STATIC);
    //remplace le parametre 1 ('?' n°1) par un blob
    sqlite3_bind_int(pStmt, 2, userId);
    //remplace le parametre 2 ('?' n°2) par un entier

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {

        printf("execution failed: %s", sqlite3_errmsg(db));
    }

    free(data);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}


#endif //BAD_CODE_SQLITEFUNCTIONS_H
