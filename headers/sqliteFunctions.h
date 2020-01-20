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
    if (returnCode) {
        return 1;
    }

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

//TODO set student class_fk to null
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

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name listClass
 * @param dbname
 * @param data
 *
 * @data = "id, name, year, apprenticeship, major, user(first_name + last_name), user_fk, sanction(name), sanction_fk;\n..."
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
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, ",");

            //Colonne 2
            itoa(sqlite3_column_int(pStmt, 2), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 3
            itoa(sqlite3_column_int(pStmt, 3), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, ",");

            //Colonne 5
            rowStringSize += sqlite3_column_bytes(pStmt, 5) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
            strcat(result, ",");

            //Colonne 6
            itoa(sqlite3_column_int(pStmt, 6), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));


            //Colonne 7
            rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
            strcat(result, ",");

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

//TODO remove deliverables
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
 * @data = "id, first_name, last_name, photo, email, bad_code(count), nb_bottles, class(name), class_fk;\n..."
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
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, ",");

            //Colonne 2
            rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
            strcat(result, ",");

            //Colonne 3
            rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
            strcat(result, ",");

            //Colonne 4
            rowStringSize += sqlite3_column_bytes(pStmt, 4) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 4) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 4));
            strcat(result, ",");

            //Colonne 5
            itoa(sqlite3_column_int(pStmt, 5), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 6
            itoa(sqlite3_column_int(pStmt, 6), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 7
            rowStringSize += sqlite3_column_bytes(pStmt, 7) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 7) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 7));
            strcat(result, ",");

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

//TODO set class sanction_fk to null
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

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name  listSanction
 * @param dbname
 * @param data
 *
 * @data = "id, name, description, user(first_name + last_name), user_fk;\n"
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
            rowStringSize += strlen(intBuffer) + 1;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ","));

            //Colonne 1
            rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1));
            strcat(result, ",");

            //Colonne 2
            rowStringSize += sqlite3_column_bytes(pStmt, 2) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 2) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 2));
            strcat(result, ",");

            //Colonne 3
            rowStringSize += sqlite3_column_bytes(pStmt, 3) + 1;
            result = realloc(result, rowStringSize);
            strcat(result, sqlite3_column_text(pStmt, 3) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 3));
            strcat(result, ",");

            //Colonne 4
            itoa(sqlite3_column_int(pStmt, 4), intBuffer, 10);
            rowStringSize += strlen(intBuffer) + 2;// pour le ","
            result = realloc(result, rowStringSize);
            strcat(result, strcat(intBuffer, ";\n"));
        }
    }
    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

int insertDeliverable(char *dbname, char *due_date, char *subject, char *audio_record_path, char *video_reccord_path,
                      char *bad_code_path, char *deliverable_file_path, char *status, int student_fk) {
    return 0;
}


#endif //BAD_CODE_SQLITEFUNCTIONS_H
