#ifndef BAD_CODE_SQLITEFUNCTIONS_H
#define BAD_CODE_SQLITEFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

// Global variables from main.c / config file
extern char *dbname;
extern char *storageFolder;
extern char *darkThemePath;
extern char *defaultThemePath;
extern int darkTheme;

void checkVersion();

sqlite3 *connectDB();

int insertTableImage(char *table, int id, const char *photo_location);

int insertUser(char *email, char *first_name, char *last_name, char *photo_location, char *birthdate);

int updateUser(int id, const char *email, const char *first_name, const char *last_name, const char *birthdate);

/**
 * @name getUser
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|email|first_name|last_name|photo|birthdate;\n"
 */
void getUser(char **data, int id);

int insertClass(const char *name, const char *major, int year, int apprenticeship, int user_fk, int sanction_fk);

int
updateClass(int id, const char *name, const char *major, int year, int apprenticeship, int user_fk, int sanction_fk);

int studentNullClass(int class_fk);

int deleteClass(int id);

/**
 * @name listClass
 * @param dbname
 * @param data
 *
 * @data = "id| name| year| apprenticeship| major| user(first_name + last_name)| user_fk| sanction(name)| sanction_fk;\n..."
 */
void listClasses(char **data);

/**
 * @name getClass
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|name|year|apprenticeship|major|user|user_fk|sanction|sanction_fk;\n"
 */
void getClass(char **data, int id);

int insertStudent(const char *first_name, const char *last_name, const char *photo_location, const char *email,
                  int class_fk);

int addStudentBottle(int id, int count);

int updateStudent(int id, const char *first_name, const char *last_name, const char *email, int bottles, int class_fk);

int deleteStudentDeliverables(int student_fk);

int deleteStudent(int id);

/**
 * @name listStudents
 * @param dbname
 * @param data
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listStudents(char **data);

/**
 * @name listClassStudents
 * @param dbname
 * @param data
 * @param class_fk
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listClassStudents(char **data, int class_fk);

/**
 * @name getStudent
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|first_name|last_name|photo|email|bad_code(count)|nb_bottles|class|class_fk;\n"
 */
void getStudent(char **data, int id);

int insertSanction(const char *description, const char *name, int user_fk);

int updateSanction(int id, const char *name, const char *description, int user_fk);

int classNullSanction(int sanction_fk);

int deleteSanction(int id);

/**
 * @name  listSanctions
 * @param dbname
 * @param data
 *
 * @data = "id| name| description| user(first_name + last_name)| user_fk;\n"
 */
void listSanctions(char **data);

/**
 * @name  getSanction
 * @param data
 * @param id
 *
 * @data = "id|name|description|user(first_name + last_name)|user_fk;\n"
 */
void getSanction(char **data, int id);

/**
 * @name getSanctionStudentId
 * @param data
 * @param student_id
 *
 * @data = "id|name|description|student(first_name + last_name)|student_fk;\n
 */
void getSanctionStudentId(char **data, int student_id);

char *insertDeliverableFile(const char *column, int id, int student_fk, const char *file_location);

int insertDeliverable(const char *due_date, const char *subject, const char *audio_record_path,
                      const char *video_reccord_path,
                      const char *bad_code_path, const char *deliverable_file_path, const char *status, int student_fk);

int updateDeliverable(int id, const char *due_date, const char *subject, const char *status, int student_fk);

int deleteDeliverable(int id);

/**
 * @name listDeliverables
 * @param data
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listDeliverables(char **data);

/**
 * @name listStudentDeliverables
 * @param data
 * @param studentId
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listStudentDeliverables(char **data, int studentId);

/**
 * @name getDeliverable
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk|sanction_name|sanction_description;\n"
 */
void getDeliverable(char **data, int id);

#endif //BAD_CODE_SQLITEFUNCTIONS_H
