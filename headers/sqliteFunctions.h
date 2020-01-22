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
        fprintf(stdout, "Opened/Created database '%s' successfully\n", dbname);
    }
    return db;
}

int insertTableImage(char *dbname, char *table, int id, char *photo_location) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    PARTIE FICHIER 1 ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = malloc(50);
    sprintf(sqlRequest, "select photo from %s WHERE id = ?", table);

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(pStmt, 1, id);
    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    char *filePathBuffer = malloc(sqlite3_column_bytes(pStmt, 0));
    strcpy(filePathBuffer, sqlite3_column_text(pStmt, 0) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 0));

    if (strlen(filePathBuffer) > 0) {
        remove(filePathBuffer);
    }
    free(filePathBuffer);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    PARTIE FICHIER 2 ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    char fileName[strlen(photo_location)];
    strcpy(fileName, photo_location);

    char *targetFileBuffer = malloc(strlen(fileName) + 50);
    sprintf(targetFileBuffer, "storage/%s/%d/%s", table, id, basename(fileName));

    returnCode = copyFile(photo_location, targetFileBuffer);
    if (returnCode)
        return 1;


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    PARTIE SQL /////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sprintf(sqlRequest, "UPDATE %s SET photo = ? WHERE id = ?", table);

    returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, targetFileBuffer, -1, 0);
    sqlite3_bind_int(pStmt, 2, id);
    //remplace le parametre 2 ('?' n°2) par un entier

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    free(sqlRequest);
    free(targetFileBuffer);
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
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
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
        returnCode = insertTableImage(dbname, "user", last_id, photo_location);
        if (returnCode) {
            fprintf(stderr, "adding profil picture failed");
            return 1;
        }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int updateUser(char *dbname, int id, char *email, char *first_name, char *last_name, char *birthdate) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update user set email = ?, first_name = ?, last_name = ?, birthdate = ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
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

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name getUser
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|email|first_name|last_name|photo|birthdate;\n"
 */
void getUser(char *dbname, char **data, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select * from user where id = ?;";
    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        *data = result;
        return;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        *data = result;
        return;
    }

    char intBuffer[6];

    //Colonne 0 (id)
    itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 1 (email)
    rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
    strcat(result, "|");
    //Colonne 2 (first_name)
    rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
    strcat(result, "|");
    //Colonne 3 (last_name)
    rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
    strcat(result, "|");
    //Colonne 4 (photo)
    rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
    strcat(result, "|");
    //Colonne 5 (birthdate)
    rowStringSize += sqlite3_column_bytes(pStmt, 5) + 2;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
    strcat(result, ";\n");

    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

int insertClass(char *dbname, char *name, int year, int apprenticeship, int sanction_fk, int user_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into class (name, year, apprenticeship, user_fk, sanction_fk) VALUES (?, ?, ?, ?, ?)";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
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

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int updateClass(char *dbname, int id, char *name, int year, int apprenticeship, int user_fk, int sanction_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update class set name = ?, year = ?, apprenticeship = ?, user_fk = ?, sanction_fk = ? where id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
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

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int studentNullClass(char *dbname, int class_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update student set class_fk = null where class_fk = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, class_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int deleteClass(char *dbname, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "delete from class where id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    studentNullClass(dbname, id);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name listClass
 * @param dbname
 * @param data
 *
 * @data = "id| name| year| apprenticeship| major| user(first_name + last_name)| user_fk| sanction(name)| sanction_fk;\n..."
 */
void listClass(char *dbname, char **data) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select class.id,\n"
                       "       class.name,\n"
                       "       year,\n"
                       "       apprenticeship,\n"
                       "       major,\n"
                       "       u.first_name || ' ' || u.last_name as user,\n"
                       "       class.user_fk,\n"
                       "       s.name                             as sanction,\n"
                       "       class.sanction_fk\n"
                       "from class\n"
                       "         left join user u on class.user_fk = u.id\n"
                       "         left join sanction s on class.sanction_fk = s.id;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    char intBuffer[6];
    while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
        returnCode = sqlite3_step(pStmt);
        if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
            //Colonne 0
            itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, "|");

            //Colonne 2
            itoa(sqlite3_column_int(pStmt, 2), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 3
            itoa(sqlite3_column_int(pStmt, 3), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, "|");

            //Colonne 5
            rowStringSize += sqlite3_column_bytes(pStmt, 5) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
            strcat(result, "|");

            //Colonne 6
            itoa(sqlite3_column_int(pStmt, 6), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));


            //Colonne 7
            rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
            strcat(result, "|");

            //Colonne 8
            itoa(sqlite3_column_int(pStmt, 8), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 2;// pour le ";\n"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ";\n"));
        }
    }
    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

/**
 * @name getClass
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|name|year|apprenticeship|major|user|user_fk|sanction|sanction_fk;\n"
 */
void getClass(char *dbname, char **data, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select class.id,\n"
                       "       class.name,\n"
                       "       year,\n"
                       "       apprenticeship,\n"
                       "       major,\n"
                       "       u.first_name || ' ' || u.last_name as user,\n"
                       "       class.user_fk,\n"
                       "       s.name                             as sanction,\n"
                       "       class.sanction_fk\n"
                       "from class\n"
                       "         left join user u on class.user_fk = u.id\n"
                       "         left join sanction s on class.sanction_fk = s.id\n"
                       "where class.id = ?;";
    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        *data = result;
        return;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        *data = result;
        return;
    }

    char intBuffer[6];

    //Colonne 0 (id)
    itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 1 (name)
    rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
    strcat(result, "|");
    //Colonne 2 (year)
    itoa(sqlite3_column_int(pStmt, 2), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 3 (apprenticeship)
    itoa(sqlite3_column_int(pStmt, 3), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 4 (major)
    rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
    strcat(result, "|");
    //Colonne 5 (user)
    rowStringSize += sqlite3_column_bytes(pStmt, 5) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
    strcat(result, "|");
    //Colonne 6 (user_fk)
    itoa(sqlite3_column_int(pStmt, 6), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 7 (sanction)
    rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
    strcat(result, "|");
    //Colonne 8 (sanction_fk)
    itoa(sqlite3_column_int(pStmt, 8), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 2;
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, ";\n"));

    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

int insertStudent(char *dbname, char *first_name, char *last_name, char *photo_location, char *email, int class_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into student (first_name, last_name, email, nb_bottles, class_fk) VALUES (?, ?, ?, 0, ?);";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, first_name, -1, 0);
    sqlite3_bind_text(pStmt, 2, last_name, -1, 0);
    sqlite3_bind_text(pStmt, 3, email, -1, 0);
    sqlite3_bind_int(pStmt, 4, class_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }


    if (photo_location != NULL && strlen(photo_location) > 0) {
        int last_id = sqlite3_last_insert_rowid(db);
        returnCode = insertTableImage(dbname, "student", last_id, photo_location);
        if (returnCode) {
            fprintf(stderr, "adding profil picture failed");
            return 1;
        }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int addStudentBottle(char *dbname, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update student set nb_bottles = nb_bottles + 1 where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int updateStudent(char *dbname, int id, char *first_name, char *last_name, char *email, int class_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update student set first_name = ?, last_name= ?, email = ?, class_fk = ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, first_name, -1, 0);
    sqlite3_bind_text(pStmt, 2, last_name, -1, 0);
    sqlite3_bind_text(pStmt, 3, email, -1, 0);
    sqlite3_bind_int(pStmt, 4, class_fk);
    sqlite3_bind_int(pStmt, 5, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int deleteStudentDeliverables(char *dbname, int student_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "delete from deliverable where student_fk = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, student_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int deleteStudent(char *dbname, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "delete from student where id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    deleteStudentDeliverables(dbname, id);

    char *studentIdBuffer = malloc(4);
    itoa(id, studentIdBuffer, 10);
    char *studentStoragePathBuffer = malloc(14 + strlen(studentIdBuffer));
    strcat(strcat(strcpy(studentStoragePathBuffer, "storage/user/"), studentIdBuffer), "/");
    removeDirectory(studentStoragePathBuffer);
    free(studentIdBuffer);
    free(studentStoragePathBuffer);

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name listStudent
 * @param dbname
 * @param data
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listStudent(char *dbname, char **data) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select student.id,\n"
                       "       first_name,\n"
                       "       last_name,\n"
                       "       photo,\n"
                       "       email,\n"
                       "       (select count(*) from deliverable where student_fk = student.id) as bad_code,\n"
                       "       nb_bottles,\n"
                       "       c.name                                                           as class,\n"
                       "       class_fk\n"
                       "from student\n"
                       "         left join class c on student.class_fk = c.id;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    char intBuffer[10];
    while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
        returnCode = sqlite3_step(pStmt);
        if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {

            //Colonne 0
            itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, "|");

            //Colonne 2
            rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
            strcat(result, "|");

            //Colonne 3
            rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
            strcat(result, "|");

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, "|");

            //Colonne 5
            itoa(sqlite3_column_int(pStmt, 5), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 6
            itoa(sqlite3_column_int(pStmt, 6), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 7
            rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
            strcat(result, "|");

            //Colonne 8
            itoa(sqlite3_column_int(pStmt, 8), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 2;// pour le ";\n"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ";\n"));

        }
    }
    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

//TODO listStudent's class
/**
 * @name getStudent
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|first_name|last_name|photo|email|bad_code(count)|nb_bottles|class|class_fk;\n"
 */
void getStudent(char *dbname, char **data, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select student.id,\n"
                       "       first_name,\n"
                       "       last_name,\n"
                       "       photo,\n"
                       "       email,\n"
                       "       (select count(*) from deliverable where student_fk = student.id) as bad_code,\n"
                       "       nb_bottles,\n"
                       "       c.name                                                           as class,\n"
                       "       class_fk\n"
                       "from student\n"
                       "         left join class c on student.class_fk = c.id\n"
                       "where student.id = ?;";
    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        *data = result;
        return;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        *data = result;
        return;
    }

    char intBuffer[6];

    //Colonne 0 (id)
    itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 1 (first_name)
    rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
    strcat(result, "|");
    //Colonne 2(last_name)
    rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
    strcat(result, "|");
    //Colonne 3(photo)
    rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
    strcat(result, "|");
    //Colonne 4(email)
    rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
    strcat(result, "|");
    //Colonne 5 (bad_code count)
    itoa(sqlite3_column_int(pStmt, 5), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 6 (nb_bottles)
    itoa(sqlite3_column_int(pStmt, 6), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 7(class)
    rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
    strcat(result, "|");
    //Colonne 8 (class_fk)
    itoa(sqlite3_column_int(pStmt, 8), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));

    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}


int insertSanction(char *dbname, char *name, char *description, int user_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into sanction (name, description, user_fk) VALUES (?, ?, ?);";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, name, -1, 0);
    sqlite3_bind_text(pStmt, 2, description, -1, 0);
    sqlite3_bind_int(pStmt, 3, user_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int updateSanction(char *dbname, int id, char *name, char *description, int user_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update sanction set name = ?, description = ?, user_fk = ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, name, -1, 0);
    sqlite3_bind_text(pStmt, 2, description, -1, 0);
    sqlite3_bind_int(pStmt, 3, user_fk);
    sqlite3_bind_int(pStmt, 4, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int classNullSanction(char *dbname, int sanction_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update class set sanction_fk = null where sanction_fk = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, sanction_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int deleteSanction(char *dbname, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "delete from sanction where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    classNullSanction(dbname, id);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name  listSanction
 * @param dbname
 * @param data
 *
 * @data = "id| name| description| user(first_name + last_name)| user_fk;\n"
 */
void listSanction(char *dbname, char **data) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select sanction.id, name, description, u.first_name || ' ' || u.last_name as user, user_fk\n"
                       "from sanction\n"
                       "         left join user u on sanction.user_fk = u.id;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    char intBuffer[10];
    while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
        returnCode = sqlite3_step(pStmt);
        if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {

            //Colonne 0
            itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, "|");

            //Colonne 2
            rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
            strcat(result, "|");

            //Colonne 3
            rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
            strcat(result, "|");

            //Colonne 4
            itoa(sqlite3_column_int(pStmt, 4), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 2;// pour le ";\n"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ";\n"));
        }
    }
    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

/**
 * @name  getSanction
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|name|description|user(first_name + last_name)|user_fk;\n"
 */
void getSanction(char *dbname, char **data, int id) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select sanction.id, name, description, u.first_name || ' ' || u.last_name as user, user_fk\n"
                       "from sanction\n"
                       "         left join user u on sanction.user_fk = u.id\n"
                       "where sanction.id = ?;";
    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        *data = result;
        return;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        *data = result;
        return;
    }

    char intBuffer[6];

    //Colonne 0 (id)
    itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 1;// pour le "|"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));
    //Colonne 1 (name)
    rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
    strcat(result, "|");
    //Colonne 2 (description)
    rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
    strcat(result, "|");
    //Colonne 3 (user (name))
    rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
    strcat(result, "|");
    //Colonne 4 (user_fk)
    itoa(sqlite3_column_int(pStmt, 4), intBuffer, 10);
    rowStringSize += strlen(intBuffer) + 2;// pour le ";\n"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, ";\n"));

    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}


int insertDeliverableFile(char *dbname, char *column, int id, int student_fk, char *file_location) {

    /////////////// DELETE OLDER FILE //////////////////////////////////////////////////////////////////////////////////

    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = malloc(60);
    sprintf(sqlRequest, "select %s from deliverable WHERE id = ?", column);

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(pStmt, 1, id);
    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    char *filePathBuffer = malloc(sqlite3_column_bytes(pStmt, 0));
    strcpy(filePathBuffer, sqlite3_column_text(pStmt, 0) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 0));
    if (strlen(filePathBuffer) > 0) {
        remove(filePathBuffer);
    }
    free(filePathBuffer);

    /////////////////// COPY NEW FILE //////////////////////////////////////////////////////////////////////////////////

    char fileName[strlen(file_location)];
    strcpy(fileName, file_location);

    char *targetFileBuffer = malloc(strlen(basename(fileName)) + strlen(column) + 50);
    // 11 for "deliverable", 3 for "_" * 3
    sprintf(targetFileBuffer, "storage/student/%d/deliverable_%d_%s_%s", student_fk, id, column, basename(fileName));

    returnCode = copyFile(file_location, targetFileBuffer);
    if (returnCode)
        return 1;

    /////////////////// UPDATE SQL WITH LOCATION ///////////////////////////////////////////////////////////////////////

    sprintf(sqlRequest, "update deliverable set %s = ? where id = ?;", column);

    returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, targetFileBuffer, -1, 0);
    sqlite3_bind_int(pStmt, 2, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    free(sqlRequest);
    free(targetFileBuffer);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);

    return 0;
}

int insertDeliverable(char *dbname, char *due_date, char *subject, char *audio_record_path, char *video_reccord_path,
                      char *bad_code_path, char *deliverable_file_path, char *status, int student_fk) {

    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into deliverable (due_date, subject, status, student_fk) values (?, ?, ?, ?);";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, due_date, -1, 0);
    sqlite3_bind_text(pStmt, 2, subject, -1, 0);
    sqlite3_bind_text(pStmt, 3, status, -1, 0);
    sqlite3_bind_int(pStmt, 4, student_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    int last_id = sqlite3_last_insert_rowid(db);

    if (audio_record_path != NULL && strlen(audio_record_path) > 0) {
        returnCode = insertDeliverableFile(dbname, "audio_record", last_id, student_fk, audio_record_path);
        if (returnCode) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
    }

    if (video_reccord_path != NULL && strlen(video_reccord_path) > 0) {
        returnCode = insertDeliverableFile(dbname, "video_record", last_id, student_fk, video_reccord_path);
        if (returnCode) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
    }

    if (bad_code_path != NULL && strlen(bad_code_path) > 0) {
        returnCode = insertDeliverableFile(dbname, "bad_code", last_id, student_fk, bad_code_path);
        if (returnCode) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
    }

    if (deliverable_file_path != NULL && strlen(deliverable_file_path) > 0) {
        returnCode = insertDeliverableFile(dbname, "deliverable_file", last_id, student_fk, deliverable_file_path);
        if (returnCode) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
    }

    return 0;
}

int updateDeliverable(char *dbname, int id, char *due_date, char *subject, char *status, int student_fk) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update deliverable set due_date = ?, subject = ?, status = ?, student_fk = ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, due_date, -1, 0);
    sqlite3_bind_text(pStmt, 2, subject, -1, 0);
    sqlite3_bind_text(pStmt, 3, status, -1, 0);
    sqlite3_bind_int(pStmt, 4, student_fk);
    sqlite3_bind_int(pStmt, 5, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int deleteDeliverable(char *dbname, int id) {
    //////////////////// DELETE FILES //////////////////////////////////////////////////////////////////////////////////

    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = malloc(60);

    char *column[4][16] = {{"audio_record"},
                           {"video_record"},
                           {"bad_code"},
                           {"deliverable_file"}};
    for (int i = 0; i < 4; ++i) {
        sprintf(sqlRequest, "select %s from deliverable WHERE id = ?", *column[i]);

        int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
        if (returnCode != SQLITE_OK) {
            fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
            exit(1);
        }

        sqlite3_bind_int(pStmt, 1, id);
        returnCode = sqlite3_step(pStmt);
        if (returnCode != SQLITE_ROW) {
            fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
            return 1;
        }

        char *filePathBuffer = malloc(sqlite3_column_bytes(pStmt, 0));
        strcpy(filePathBuffer, sqlite3_column_text(pStmt, 0) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 0));
        if (strlen(filePathBuffer) > 0) {
            remove(filePathBuffer);
        }
        free(filePathBuffer);
    }

    //////////////////// DELETE SQL LINE ///////////////////////////////////////////////////////////////////////////////

    strcpy(sqlRequest, "delete from deliverable where id = ?;");

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name listDeliverables
 * @param dbname
 * @param data
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listDeliverables(char *dbname, char **data) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select deliverable.id,\n"
                       "       due_date,\n"
                       "       subject,\n"
                       "       audio_record,\n"
                       "       video_record,\n"
                       "       bad_code,\n"
                       "       deliverable_file,\n"
                       "       status,\n"
                       "       s.first_name || ' ' || s.last_name as student,\n"
                       "       student_fk\n"
                       "from deliverable\n"
                       "         left join student s on deliverable.student_fk = s.id;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    char intBuffer[10];
    while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
        returnCode = sqlite3_step(pStmt);
        if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {

            //Colonne 0
            itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, "|");

            //Colonne 2
            rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
            strcat(result, "|");

            //Colonne 3
            rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
            strcat(result, "|");

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, "|");

            //Colonne 5
            rowStringSize += sqlite3_column_bytes(pStmt, 5) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
            strcat(result, "|");

            //Colonne 6
            rowStringSize += sqlite3_column_bytes(pStmt, 6) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 6) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 6));
            strcat(result, "|");

            //Colonne 7
            rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
            strcat(result, "|");

            //Colonne 8
            rowStringSize += sqlite3_column_bytes(pStmt, 8) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 8) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 8));
            strcat(result, "|");

            //Colonne 9
            itoa(sqlite3_column_int(pStmt, 9), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 2;
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ";\n"));
        }
    }
    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

/**
 * @name listStudentDeliverables
 * @param dbname
 * @param data
 * @param studentId
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listStudentDeliverables(char *dbname, char **data, int studentId) {
    sqlite3 *db = connectDB(dbname);
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select deliverable.id,\n"
                       "       due_date,\n"
                       "       subject,\n"
                       "       audio_record,\n"
                       "       video_record,\n"
                       "       bad_code,\n"
                       "       deliverable_file,\n"
                       "       status,\n"
                       "       s.first_name || ' ' || s.last_name as student,\n"
                       "       student_fk\n"
                       "from deliverable\n"
                       "         left join student s on deliverable.student_fk = s.id\n"
                       "where student_fk = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(pStmt, 1, studentId);
    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    char intBuffer[10];
    while (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {
        returnCode = sqlite3_step(pStmt);
        if (returnCode == SQLITE_OK || returnCode == SQLITE_ROW) {

            //Colonne 0
            itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le "|"
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, "|"));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, "|");

            //Colonne 2
            rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
            strcat(result, "|");

            //Colonne 3
            rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
            strcat(result, "|");

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, "|");

            //Colonne 5
            rowStringSize += sqlite3_column_bytes(pStmt, 5) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
            strcat(result, "|");

            //Colonne 6
            rowStringSize += sqlite3_column_bytes(pStmt, 6) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 6) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 6));
            strcat(result, "|");

            //Colonne 7
            rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
            strcat(result, "|");

            //Colonne 8
            rowStringSize += sqlite3_column_bytes(pStmt, 8) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 8) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 8));
            strcat(result, "|");

            //Colonne 9
            itoa(sqlite3_column_int(pStmt, 9), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 2;
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ";\n"));
        }
    }
    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

//TODO getDeliverable(id){}

#endif //BAD_CODE_SQLITEFUNCTIONS_H
