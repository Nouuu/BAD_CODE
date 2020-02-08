#include "sqliteFunctions.h"
#include "functions.h"

void checkVersion() {
    printf("SQLite version: %s\n", sqlite3_version);
}

// Opens a connexion to the data base
sqlite3 *connectDB() {
    sqlite3 *db; // Connexion pointer, allows to open and close the database
    int returnCode = sqlite3_open(dbname, &db); // Opening the database
    if (returnCode) { // Check if the database has been correctly open
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stdout, "Opened/Created database '%s' successfully\n", dbname);
    }
    // Returns the pointer to the database structure
    return db;
}

int insertTableImage(char *table, int id, const char *photo_location) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    SUPPRESSION DE L'EVENTUELLE PHOTO //////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = malloc(50); // 50 because whatever the name of the table, the request n'a pas plus de 50 caractères
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

    char *filePathBuffer = malloc(sqlite3_column_bytes(pStmt, 0) + 1);
    strcpy(filePathBuffer, sqlite3_column_text(pStmt, 0) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 0));

    if (strlen(filePathBuffer) > 0) { // Si c'est supérieur à 0 : il y a photo
        remove(filePathBuffer); // donc on remove la photo
    }
    free(filePathBuffer);
    sqlite3_finalize(pStmt); // On finalise parce qu'après on va devoir faire une insertion

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////    COPIE DU CHEMIN ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Static array: duplication du chemin de la photo source passé en argument
    char fileName[strlen(photo_location) + 1];
    strcpy(fileName, photo_location);

    // Chemin de destination de l'image : taille du nom du fichier + taille du dossier qui va héberger
    // + 60 : caractères pour l'arborescence intermédiaire
    char *targetFileBuffer = malloc(strlen(fileName) + strlen(storageFolder) + 60);
    // Création du chemin final
    sprintf(targetFileBuffer, "%s/%s/%d/image.%s", storageFolder, table, id, get_filename_ext(basename(fileName)));

    // copie du fichier source à la destination
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

int insertUser(char *email, char *first_name, char *last_name, char *photo_location, char *birthdate) {
    sqlite3 *db = connectDB();
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
        returnCode = insertTableImage("user", last_id, photo_location);
        if (returnCode) {
            fprintf(stderr, "adding profil picture failed");
            return 1;
        }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int updateUser(int id, const char *email, const char *first_name, const char *last_name, const char *birthdate) {
    sqlite3 *db = connectDB();
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
 * @param 
 * @param data
 * @param id
 *
 * @data = "id|email|first_name|last_name|photo|birthdate;\n"
 */
void getUser(char **data, int id) {
    sqlite3 *db = connectDB(); // Connection to the database
    sqlite3_stmt *pStmt; // Creation of a statement : stores the state of the request
    char *sqlRequest = "select * from user where id = ?;"; // Preparing the request string
    size_t rowStringSize = 1; // 1 pour '\0'
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, ""); // Sur le retour, on aura le \0

    //Preparing the request
    // data base / string de requête / taille de la string de la requête / adresse du statement
    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        *data = result; // Si ça n'a pas fonctionné, chaîne de caractères vide
        return;
    }

    // Binding: grâce au statetement, on prend le premier ? auquel on bind l'id
    sqlite3_bind_int(pStmt, 1, id);

    //Execution de la requête
    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) { // Vérification qu'il y a au moins un retour à la requête
        *data = result; // Si il n'y a pas de résultat, chaîne de caractères vide
        return;
    }

    // Variable temporaire qui va contenir les "chiffres"
    char intBuffer[6];

    //Colonne 0 (id)
    itoa(sqlite3_column_int(pStmt, 0), intBuffer, 10); // on récupère la première colonne, qu'on transforme en string et qu'on colle dans intBuffer
    rowStringSize += strlen(intBuffer) + 1; // on agrandit la taille de rowstringsize de la taille de la string de l'id, +1 pour le "|" (délimiteur)
    result = realloc(result, rowStringSize); // on réalloue la taille du result avec la nouvelle taille
    strcat(result, strcat(intBuffer, "|")); // on concatène le résult + le pipe
    //Colonne 1 (email)
    rowStringSize += sqlite3_column_bytes(pStmt, 1) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 1) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 1)); //Si c'est nul, on concatène avec rien
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
    rowStringSize += sqlite3_column_bytes(pStmt, 5) + 2; // +2 car on finit la string avec ;\n
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 5) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 5));
    strcat(result, ";\n");

    *data = result; // "Renvoie" le result en modifiant, grâce pointeur
    sqlite3_finalize(pStmt); // finalisation de la requête
    sqlite3_close(db); // fermeture de la base de données
}

int insertClass(const char *name, const char *major, int year, int apprenticeship, int user_fk, int sanction_fk) {
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into class (name, year, apprenticeship, major, user_fk, sanction_fk) VALUES (?, ?, ?, ?, ?, ?)";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, name, -1, 0);
    sqlite3_bind_int(pStmt, 2, year);
    sqlite3_bind_int(pStmt, 3, apprenticeship);
    sqlite3_bind_text(pStmt, 4, major, -1, 0);
    sqlite3_bind_int(pStmt, 5, user_fk);
    sqlite3_bind_int(pStmt, 6, sanction_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int
updateClass(int id, const char *name, const char *major, int year, int apprenticeship, int user_fk, int sanction_fk) {
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update class set name = ?, year = ?, apprenticeship = ?, major = ?, user_fk = ?, sanction_fk = ? where id = ?";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, name, -1, 0);
    sqlite3_bind_int(pStmt, 2, year);
    sqlite3_bind_int(pStmt, 3, apprenticeship);
    sqlite3_bind_text(pStmt, 4, major, -1, 0);
    sqlite3_bind_int(pStmt, 5, user_fk);
    sqlite3_bind_int(pStmt, 6, sanction_fk);
    sqlite3_bind_int(pStmt, 7, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int studentNullClass(int class_fk) {
    sqlite3 *db = connectDB();
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

int deleteClass(int id) {
    sqlite3 *db = connectDB();
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

    // Tous les étudiants qui avaient la classe supprimée ont une classe à NULL
    studentNullClass(id);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name listClass
 * @param 
 * @param data
 *
 * @data = "id| name| year| apprenticeship| major| user(first_name + last_name)| user_fk| sanction(name)| sanction_fk;\n..."
 */
void listClasses(char **data) {
    sqlite3 *db = connectDB();
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
 * @param 
 * @param data
 * @param id
 *
 * @data = "id|name|year|apprenticeship|major|user|user_fk|sanction|sanction_fk;\n"
 */
void getClass(char **data, int id) {
    sqlite3 *db = connectDB();
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

int insertStudent(const char *first_name, const char *last_name, const char *photo_location, const char *email,
                  int class_fk) {
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = "insert into student (first_name, last_name, email, nb_bottles, class_fk) VALUES (?, ?, ?, 0, ?);";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, first_name, -1, 0); //-1 et 0 : permet de bind correctement
    sqlite3_bind_text(pStmt, 2, last_name, -1, 0);
    sqlite3_bind_text(pStmt, 3, email, -1, 0);
    sqlite3_bind_int(pStmt, 4, class_fk);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }


    if (photo_location != NULL && strlen(photo_location) > 0) {
        int last_id = sqlite3_last_insert_rowid(db); // Récupère le dernier ID (celui qu'on vient de créer)
        returnCode = insertTableImage("student", last_id, photo_location);
        if (returnCode) {
            fprintf(stderr, "adding profil picture failed");
            return 1;
        }
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int addStudentBottle(int id, int count) {
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update student set nb_bottles = nb_bottles + ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_int(pStmt, 1, count);
    sqlite3_bind_int(pStmt, 2, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

int updateStudent(int id, const char *first_name, const char *last_name, const char *email, int bottles, int class_fk) {
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = "update student set first_name = ?, last_name= ?, email = ?, nb_bottles = ?, class_fk = ? where id = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(pStmt, 1, first_name, -1, 0);
    sqlite3_bind_text(pStmt, 2, last_name, -1, 0);
    sqlite3_bind_text(pStmt, 3, email, -1, 0);
    sqlite3_bind_int(pStmt, 4, bottles);
    sqlite3_bind_int(pStmt, 5, class_fk);
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

int deleteStudentDeliverables(int student_fk) {
    sqlite3 *db = connectDB();
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

int deleteStudent(int id) {
    sqlite3 *db = connectDB();
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

    // Removing the student's deliverable
    deleteStudentDeliverables(id);

    // Transforming the int ID to string
    char *studentIdBuffer = malloc(5);
    itoa(id, studentIdBuffer, 10);
    // Creating a string path storage/student/id_student
    char *studentStoragePathBuffer = malloc(10 + strlen(studentIdBuffer) + strlen(storageFolder));
    strcat(strcat(strcat(strcpy(studentStoragePathBuffer, storageFolder), "/student/"), studentIdBuffer), "/");
    // Removing the directory
    removeDirectory(studentStoragePathBuffer);

    free(studentIdBuffer);
    free(studentStoragePathBuffer);

    sqlite3_finalize(pStmt);
    sqlite3_close(db);

    return 0;
}

/**
 * @name listStudents
 * @param 
 * @param data
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listStudents(char **data) {
    sqlite3 *db = connectDB();
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
                       "order by last_name ASC;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    char intBuffer[10];

    // First passage : return code = sqliteOK
    // Then = SQLITE_ROW : check there's a line
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

/**
 * @name listClassStudents
 * @param 
 * @param data
 * @param class_fk
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listClassStudents(char **data, int class_fk) {
    sqlite3 *db = connectDB();
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
                       "where class_fk = ?;";

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    sqlite3_bind_int(pStmt, 1, class_fk);
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

/**
 * @name getStudent
 *
 * @param 
 * @param data
 * @param id
 *
 * @data = "id|first_name|last_name|photo|email|bad_code(count)|nb_bottles|class|class_fk;\n"
 */
void getStudent(char **data, int id) {
    sqlite3 *db = connectDB();
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
    rowStringSize += strlen(intBuffer) + 2;// pour le ";\n"
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, ";\n"));

    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}


int insertSanction(const char *description, const char *name, int user_fk) {
    sqlite3 *db = connectDB();
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

int updateSanction(int id, const char *name, const char *description, int user_fk) {
    sqlite3 *db = connectDB();
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

int classNullSanction(int sanction_fk) {
    sqlite3 *db = connectDB();
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

int deleteSanction(int id) {
    sqlite3 *db = connectDB();
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

    classNullSanction(id);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name  listSanctions
 * @param 
 * @param data
 *
 * @data = "id| name| description| user(first_name + last_name)| user_fk;\n"
 */
void listSanctions(char **data) {
    sqlite3 *db = connectDB();
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
 * @param 
 * @param data
 * @param id
 *
 * @data = "id|name|description|user(first_name + last_name)|user_fk;\n"
 */
void getSanction(char **data, int id) {
    sqlite3 *db = connectDB();
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

void getSanctionStudentId(char **data, int student_id) {
    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = "select sanction.id, sanction.name, description, s.first_name || ' ' || s.last_name as student, s.id as student_id\n"
                       "from sanction\n"
                       "         left join class c on sanction.id = c.sanction_fk\n"
                       "         left join student s on c.id = s.class_fk\n"
                       "where s.id = ?;";

    size_t rowStringSize = 1;
    char *result = malloc(rowStringSize * sizeof(char));
    strcpy(result, "");

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        *data = result;
        return;
    }

    sqlite3_bind_int(pStmt, 1, student_id);

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
    //Colonne 3 (student (name))
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

char *insertDeliverableFile(const char *column, int id, int student_fk, const char *file_location) {

    /////////////// DELETE OLDER FILE //////////////////////////////////////////////////////////////////////////////////

    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = malloc(100);
    sprintf(sqlRequest, "select %s from deliverable WHERE id = ?", column);

    int returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return "";
    }

    sqlite3_bind_int(pStmt, 1, id);
    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_ROW) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return "";
    }

    char *filePathBuffer = malloc(sqlite3_column_bytes(pStmt, 0) + 1);
    strcpy(filePathBuffer, sqlite3_column_text(pStmt, 0) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 0));
    if (strlen(filePathBuffer) > 0) {
        remove(filePathBuffer);
    }

    sqlite3_finalize(pStmt);
    free(filePathBuffer);

    /////////////////// COPY NEW FILE //////////////////////////////////////////////////////////////////////////////////

    char fileName[strlen(file_location)];
    strcpy(fileName, file_location);

    char *targetFileBuffer = malloc(strlen(basename(fileName)) + strlen(column) + strlen(storageFolder) + 50);
    // 11 for "deliverable", 3 for "_" * 3
    sprintf(targetFileBuffer, "%s/student/%d/deliverable_%d_%s.%s", storageFolder, student_fk, id, column,
            get_filename_ext(basename(fileName)));

    returnCode = copyFile(file_location, targetFileBuffer);
    if (returnCode)
        return "";

    /////////////////// UPDATE SQL WITH LOCATION ///////////////////////////////////////////////////////////////////////

    sprintf(sqlRequest, "update deliverable set %s = ? where id = ?;", column);

    returnCode = sqlite3_prepare_v2(db, sqlRequest, (int) strlen(sqlRequest), &pStmt, NULL);
    if (returnCode != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare sql request statement: %s\n", sqlite3_errmsg(db));
        return "";
    }

    sqlite3_bind_text(pStmt, 1, targetFileBuffer, -1, 0);
    sqlite3_bind_int(pStmt, 2, id);

    returnCode = sqlite3_step(pStmt);
    if (returnCode != SQLITE_DONE) {
        fprintf(stderr, "execution failed: %s", sqlite3_errmsg(db));
        return "";
    }

    free(sqlRequest);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);

    return targetFileBuffer;
}

int insertDeliverable(const char *due_date, const char *subject, const char *audio_record_path,
                      const char *video_reccord_path,
                      const char *bad_code_path, const char *deliverable_file_path, const char *status,
                      int student_fk) {

    sqlite3 *db = connectDB();
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
    char *returnCode_c;
    if (audio_record_path != NULL && strlen(audio_record_path) > 0) {
        returnCode_c = insertDeliverableFile("audio_record", last_id, student_fk, audio_record_path);
        if (!strcmp(returnCode_c, "")) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
        free(returnCode_c);
    }

    if (video_reccord_path != NULL && strlen(video_reccord_path) > 0) {
        returnCode_c = insertDeliverableFile("video_record", last_id, student_fk, video_reccord_path);
        if (!strcmp(returnCode_c, "")) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
        free(returnCode_c);
    }

    if (bad_code_path != NULL && strlen(bad_code_path) > 0) {
        returnCode_c = insertDeliverableFile("bad_code", last_id, student_fk, bad_code_path);
        if (!strcmp(returnCode_c, "")) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
        free(returnCode_c);
    }

    if (deliverable_file_path != NULL && strlen(deliverable_file_path) > 0) {
        returnCode_c = insertDeliverableFile("deliverable_file", last_id, student_fk, deliverable_file_path);
        if (!strcmp(returnCode_c, "")) {
            fprintf(stderr, "adding deliverable audio record failed");
            return 1;
        }
        free(returnCode_c);
    }

    return 0;
}

int updateDeliverable(int id, const char *due_date, const char *subject, const char *status, int student_fk) {
    sqlite3 *db = connectDB();
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

int deleteDeliverable(int id) {
    //////////////////// DELETE FILES //////////////////////////////////////////////////////////////////////////////////

    sqlite3 *db = connectDB();
    sqlite3_stmt *pStmt;
    char *sqlRequest = malloc(100);

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

        char *filePathBuffer = malloc(sqlite3_column_bytes(pStmt, 0) + 1);
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

    free(sqlRequest);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return 0;
}

/**
 * @name listDeliverables
 * @param 
 * @param data
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listDeliverables(char **data) {
    sqlite3 *db = connectDB();
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
 * @param 
 * @param data
 * @param studentId
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listStudentDeliverables(char **data, int studentId) {
    sqlite3 *db = connectDB();
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

/**
 * @name getDeliverable
 * @param 
 * @param data
 * @param id
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk|sanction_name|sanction_description;\n"
 */
void getDeliverable(char **data, int id) {
    sqlite3 *db = connectDB();
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
                       "       student_fk,\n"
                       "       s2.name                            as sanction_name,\n"
                       "       s2.description                     as sanction_description\n"
                       "from deliverable\n"
                       "         left join student s on deliverable.student_fk = s.id\n"
                       "         left join class c on s.class_fk = c.id\n"
                       "         left join sanction s2 on c.sanction_fk = s2.id\n"
                       "where deliverable.id = ?;";
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

    char intBuffer[10];

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
    rowStringSize += strlen(intBuffer) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, strcat(intBuffer, "|"));

    //Colonne 10
    rowStringSize += sqlite3_column_bytes(pStmt, 10) + 1;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 10) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 10));
    strcat(result, "|");

    //Colonne 11
    rowStringSize += sqlite3_column_bytes(pStmt, 11) + 2;
    result = realloc(result, rowStringSize);
    strcat(result, sqlite3_column_text(pStmt, 11) == NULL ? "" : (char *) sqlite3_column_text(pStmt, 11));
    strcat(result, ";\n");


    *data = result;
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}