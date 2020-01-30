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

void checkVersion();

sqlite3 *connectDB(char *dbname);

int insertTableImage(char *dbname, char *table, int id, char *photo_location);

int insertUser(char *dbname, char *email, char *first_name, char *last_name, char *photo_location, char *birthdate);

int updateUser(char *dbname, int id, char *email, char *first_name, char *last_name, char *birthdate);

/**
 * @name getUser
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|email|first_name|last_name|photo|birthdate;\n"
 */
void getUser(char *dbname, char **data, int id);

int insertClass(char *dbname, char *name, int year, int apprenticeship, int sanction_fk, int user_fk);

int updateClass(char *dbname, int id, char *name, int year, int apprenticeship, int user_fk, int sanction_fk);

int studentNullClass(char *dbname, int class_fk);

int deleteClass(char *dbname, int id);

/**
 * @name listClass
 * @param dbname
 * @param data
 *
 * @data = "id| name| year| apprenticeship| major| user(first_name + last_name)| user_fk| sanction(name)| sanction_fk;\n..."
 */
void listClasses(char *dbname, char **data);

/**
 * @name getClass
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|name|year|apprenticeship|major|user|user_fk|sanction|sanction_fk;\n"
 */
void getClass(char *dbname, char **data, int id);

int insertStudent(char *dbname, char *first_name, char *last_name, char *photo_location, char *email, int class_fk);

int addStudentBottle(char *dbname, int id, int count);

int updateStudent(char *dbname, int id, char *first_name, char *last_name, char *email, int class_fk);

int deleteStudentDeliverables(char *dbname, int student_fk);

int deleteStudent(char *dbname, int id);

/**
 * @name listStudents
 * @param dbname
 * @param data
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listStudents(char *dbname, char **data);

/**
 * @name listClassStudents
 * @param dbname
 * @param data
 * @param class_fk
 *
 * @data = "id| first_name| last_name| photo| email| bad_code(count)| nb_bottles| class(name)| class_fk;\n..."
 */
void listClassStudents(char *dbname, char **data, int class_fk);

/**
 * @name getStudent
 *
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|first_name|last_name|photo|email|bad_code(count)|nb_bottles|class|class_fk;\n"
 */
void getStudent(char *dbname, char **data, int id);

int insertSanction(char *dbname, char *name, char *description, int user_fk);

int updateSanction(char *dbname, int id, char *name, char *description, int user_fk);

int classNullSanction(char *dbname, int sanction_fk);

int deleteSanction(char *dbname, int id);

/**
 * @name  listSanctions
 * @param dbname
 * @param data
 *
 * @data = "id| name| description| user(first_name + last_name)| user_fk;\n"
 */
void listSanctions(char *dbname, char **data);

/**
 * @name  getSanction
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|name|description|user(first_name + last_name)|user_fk;\n"
 */
void getSanction(char *dbname, char **data, int id);

int insertDeliverableFile(char *dbname, char *column, int id, int student_fk, char *file_location);

int insertDeliverable(char *dbname, char *due_date, char *subject, char *audio_record_path, char *video_reccord_path,
                      char *bad_code_path, char *deliverable_file_path, char *status, int student_fk);

int updateDeliverable(char *dbname, int id, char *due_date, char *subject, char *status, int student_fk);

int deleteDeliverable(char *dbname, int id);

/**
 * @name listDeliverables
 * @param dbname
 * @param data
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listDeliverables(char *dbname, char **data);

/**
 * @name listStudentDeliverables
 * @param dbname
 * @param data
 * @param studentId
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n..."
 */
void listStudentDeliverables(char *dbname, char **data, int studentId);

/**
 * @name getDeliverable
 * @param dbname
 * @param data
 * @param id
 *
 * @data = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n"
 */
void getDeliverable(char *dbname, char **data, int id);

#endif //BAD_CODE_SQLITEFUNCTIONS_H
