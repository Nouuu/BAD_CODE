//
// Created by MaleWhere on 06/01/2020.
//

#ifndef BAD_CODE_SQLITEFUNCTIONS_H
#define BAD_CODE_SQLITEFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

extern char *dbname;

void checkVersion();

sqlite3 *connectDB();

int insertTableImage(char *table, int id, char *photo_location);

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

int insertClass(char *name, int year, int apprenticeship, const char *major, int sanction_fk, int user_fk);

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

int insertStudent(char *first_name, char *last_name, char *photo_location, char *email, int class_fk);

int addStudentBottle(int id, int count);

int updateStudent(int id, char *first_name, char *last_name, char *email, int class_fk);

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

int insertSanction(char *name, char *description, int user_fk);

int updateSanction(int id, char *name, char *description, int user_fk);

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
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|name|description|user(first_name + last_name)|user_fk;\n"
 */
void getSanction(char **data, int id);

int insertDeliverableFile(char *column, int id, int student_fk, char *file_location);

int insertDeliverable(char *due_date, char *subject, char *audio_record_path, char *video_reccord_path,
                      char *bad_code_path, char *deliverable_file_path, char *status, int student_fk);

int updateDeliverable(int id, char *due_date, char *subject, char *status, int student_fk);

int deleteDeliverable(int id);

/**
 * @name listDeliverables
 * @param dbname
 * @param data
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listDeliverables(char **data);

/**
 * @name listStudentDeliverables
 * @param dbname
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
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n"
 */
void getDeliverable(char **data, int id);

#endif //BAD_CODE_SQLITEFUNCTIONS_H
