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

    char *data = malloc(flen);
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
    char *sqlRequest = "UPDATE user SET photo = ?, photo_name = ? WHERE id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    char *filePath = malloc(strlen(photo_location));
    strcpy(filePath, photo_location);

    sqlite3_bind_blob(pStmt, 1, data, (int) size, SQLITE_STATIC);
    //remplace le parametre 1 ('?' n°1) par un blob
    sqlite3_bind_text(pStmt, 2, basename(filePath), -1, 0);
    sqlite3_bind_int(pStmt, 3, userId);
    //remplace le parametre 3 ('?' n°3) par un entier

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    free(data);
    free(filePath);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);

    return 0;
}

int insertUser(char *dbname, char *email, char *first_name, char *last_name, char *photo_location, char *birthdate) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into user (email, first_name, last_name, birthdate) VALUES (?, ?, ?, ?)";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, email, -1, 0);
    sqlite3_bind_text(pStmt, 2, first_name, -1, 0);
    sqlite3_bind_text(pStmt, 3, last_name, -1, 0);
    sqlite3_bind_text(pStmt, 4, birthdate, -1, 0);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }


    if (photo_location != NULL && strlen(photo_location) > 0) {
        int last_id = sqlite3_last_insert_rowid(db);
        returnCode = insertUserImage(dbname, last_id, photo_location);
        if (returnCode) {
            fprintf(stderr, "adding profil picture failed");
            return 1;
        }
    }


    sqlite3_close(db);
    return 0;
}

int updateUser(char *dbname, int id, char *email, char *first_name, char *last_name, char *photo_location,
               char *birthdate) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update user set email = ?, first_name = ?, last_name = ?, birthdate = ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, email, -1, 0);
    sqlite3_bind_text(pStmt, 2, first_name, -1, 0);
    sqlite3_bind_text(pStmt, 3, last_name, -1, 0);
    sqlite3_bind_text(pStmt, 4, birthdate, -1, 0);
    sqlite3_bind_int(pStmt, 5, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }


    if (photo_location != NULL && strlen(photo_location) > 0) {
        returnCode = insertUserImage(dbname, id, photo_location);
        if (returnCode) {
            fprintf(stderr, "adding profil picture failed");
            return 1;
        }
    }
    sqlite3_close(db);
    return 0;
}

int insertClass(char *dbname, char *name, int year, int apprenticeship, int sanction_fk, int user_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into class (name, year, apprenticeship, user_fk, sanction_fk) VALUES (?, ?, ?, ?, ?)";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, name, -1, 0);
    sqlite3_bind_int(pStmt, 2, year);
    sqlite3_bind_int(pStmt, 3, apprenticeship);
    sqlite3_bind_int(pStmt, 4, sanction_fk);
    sqlite3_bind_int(pStmt, 5, user_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

int updateClass(char *dbname, int id, char *name, int year, int apprenticeship, int user_fk, int sanction_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update class set name = ?, year = ?, apprenticeship = ?, user_fk = ?, sanction_fk = ? where id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, name, -1, 0);
    sqlite3_bind_int(pStmt, 2, year);
    sqlite3_bind_int(pStmt, 3, apprenticeship);
    sqlite3_bind_int(pStmt, 4, user_fk);
    sqlite3_bind_int(pStmt, 5, sanction_fk);
    sqlite3_bind_int(pStmt, 6, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

int deleteClass(char *dbname, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "delete from class where id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

void listClass(char *dbname, char **data) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select * from class";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request stattement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    char *result = malloc(0);

    char buffer[6];
    unsigned int rowStringSize = 0;
    while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
        returnCode = sqlite3_step(pStmt);
        if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
            //Colonne 0
            itoa(sqlite3_column_int(pStmt, 0), buffer, 10);
            rowStringSize += strlen(buffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(buffer, ","));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, ",");

            //Colonne 2
            itoa(sqlite3_column_int(pStmt, 2), buffer, 10);
            rowStringSize += strlen(buffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(buffer, ","));

            //Colonne 3
            itoa(sqlite3_column_int(pStmt, 3), buffer, 10);
            rowStringSize += strlen(buffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(buffer, ","));

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, ",");

            //Colonne 5
            itoa(sqlite3_column_int(pStmt, 5), buffer, 10);
            rowStringSize += strlen(buffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(buffer, ","));

            //Colonne 6
            itoa(sqlite3_column_int(pStmt, 6), buffer, 10);
            rowStringSize += strlen(buffer) + 1;// pour le ";"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(buffer, ";"));
        }
    }
    sqlite3_finalize(pStmt);
    *data = result;
    sqlite3_close(db);
}


#endif //BAD_CODE_SQLITEFUNCTIONS_H
