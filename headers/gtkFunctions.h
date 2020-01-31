//
// Created by MaleWhere on 15/01/2020.
//

#ifndef BAD_CODE_GTKFUNCTIONS_H
#define BAD_CODE_GTKFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <math.h>

// link builder to the one in main.c
extern GtkBuilder *builder;
extern char *dbname;

typedef struct {
    GtkStack *view_students_stack;
    GtkWidget *view_students_fixed;
    GtkWidget *edit_student_fixed;
    GtkWidget *create_student_fixed;
    GtkWidget *view_students_view;
    GtkButton *students_view_delete_button;
    GtkButton *students_view_create_button;
    GtkButton *students_view_refresh_button;
    GtkButton *students_view_remove_bottle_button;
    GtkButton *students_view_add_bottle_button;
    GtkTreeStore *students_tree_store;
    GtkTreeView *students_tree_view;
    GtkTreeSelection *students_tree_selection;
    GtkTreeViewColumn *students_cx_1;
    GtkTreeViewColumn *students_cx_2;
    GtkTreeViewColumn *students_cx_3;
    GtkTreeViewColumn *students_cx_4;
    GtkTreeViewColumn *students_cx_5;
    GtkTreeViewColumn *students_cx_6;
    GtkTreeViewColumn *students_cx_7;
    GtkTreeViewColumn *students_cx_8;
    GtkTreeViewColumn *students_cx_9;
    GtkCellRenderer *students_cr_1;
    GtkCellRenderer *students_cr_2;
    GtkCellRenderer *students_cr_3;
    GtkCellRenderer *students_cr_4;
    GtkCellRenderer *students_cr_5;
    GtkCellRenderer *students_cr_6;
    GtkCellRenderer *students_cr_7;
    GtkCellRenderer *students_cr_8;
    GtkCellRenderer *students_cr_9;
} Students;

typedef struct {
    GtkStack *view_classes_stack;
    GtkWidget *view_classes_fixed;
    GtkWidget *edit_class_fixed;
    GtkWidget *create_class_fixed;
    GtkWidget *view_classes_view;
    GtkButton *classes_view_delete_button;
    GtkButton *classes_view_create_button;
    GtkButton *classes_view_refresh_button;
    GtkButton *class_edit_return_button;
    GtkButton *class_edit_submit_button;
    GtkButton *class_create_return_button;
    GtkButton *class_create_submit_button;
    GtkTreeStore *classes_tree_store;
    GtkTreeView *classes_tree_view;
    GtkTreeSelection *classes_tree_selection;
    GtkTreeViewColumn *classes_cx_1;
    GtkTreeViewColumn *classes_cx_2;
    GtkTreeViewColumn *classes_cx_3;
    GtkTreeViewColumn *classes_cx_4;
    GtkTreeViewColumn *classes_cx_5;
    GtkTreeViewColumn *classes_cx_6;
    GtkTreeViewColumn *classes_cx_7;
    GtkTreeViewColumn *classes_cx_8;
    GtkTreeViewColumn *classes_cx_9;
    GtkCellRenderer *classes_cr_1;
    GtkCellRenderer *classes_cr_2;
    GtkCellRenderer *classes_cr_3;
    GtkCellRenderer *classes_cr_4;
    GtkCellRenderer *classes_cr_5;
    GtkCellRenderer *classes_cr_6;
    GtkCellRenderer *classes_cr_7;
    GtkCellRenderer *classes_cr_8;
    GtkCellRenderer *classes_cr_9;
} Classes;

typedef struct {
    GtkStack *view_sanctions_stack;
    GtkWidget *view_sanctions_fixed;
    GtkWidget *edit_sanction_fixed;
    GtkWidget *create_sanction_fixed;
    GtkWidget *view_sanctions_view;
    GtkButton *sanctions_view_delete_button;
    GtkButton *sanctions_view_create_button;
    GtkButton *sanctions_view_refresh_button;
    GtkTreeStore *sanctions_tree_store;
    GtkTreeView *sanctions_tree_view;
    GtkTreeSelection *sanctions_tree_selection;
    GtkTreeViewColumn *sanctions_cx_1;
    GtkTreeViewColumn *sanctions_cx_2;
    GtkTreeViewColumn *sanctions_cx_3;
    GtkTreeViewColumn *sanctions_cx_4;
    GtkTreeViewColumn *sanctions_cx_5;
    GtkCellRenderer *sanctions_cr_1;
    GtkCellRenderer *sanctions_cr_2;
    GtkCellRenderer *sanctions_cr_3;
    GtkCellRenderer *sanctions_cr_4;
    GtkCellRenderer *sanctions_cr_5;
} Sanctions;

typedef struct {
    GtkStack *view_user_stack;
    GtkWidget *view_user_fixed;
    GtkWidget *edit_user_fixed;
    GtkImage *view_user_image;
    GtkFileChooserButton *view_user_image_file_picker;
} User;

typedef struct {
    GtkStack *view_deliverables_stack;
    GtkWidget *view_deliverables_fixed;
    GtkWidget *edit_deliverable_fixed;
    GtkWidget *create_deliverable_fixed;
    GtkWidget *view_deliverables_view;
    GtkButton *deliverables_view_delete_button;
    GtkButton *deliverables_view_create_button;
    GtkButton *deliverables_view_refresh_button;
    GtkTreeStore *deliverables_tree_store;
    GtkTreeView *deliverables_tree_view;
    GtkTreeSelection *deliverables_tree_selection;
    GtkTreeViewColumn *deliverables_cx_1;
    GtkTreeViewColumn *deliverables_cx_2;
    GtkTreeViewColumn *deliverables_cx_3;
    GtkTreeViewColumn *deliverables_cx_4;
    GtkTreeViewColumn *deliverables_cx_5;
    GtkTreeViewColumn *deliverables_cx_6;
    GtkTreeViewColumn *deliverables_cx_7;
    GtkTreeViewColumn *deliverables_cx_8;
    GtkTreeViewColumn *deliverables_cx_9;
    GtkTreeViewColumn *deliverables_cx_10;
    GtkCellRenderer *deliverables_cr_1;
    GtkCellRenderer *deliverables_cr_2;
    GtkCellRenderer *deliverables_cr_3;
    GtkCellRenderer *deliverables_cr_4;
    GtkCellRenderer *deliverables_cr_5;
    GtkCellRenderer *deliverables_cr_6;
    GtkCellRenderer *deliverables_cr_7;
    GtkCellRenderer *deliverables_cr_8;
    GtkCellRenderer *deliverables_cr_9;
    GtkCellRenderer *deliverables_cr_10;
} Deliverables;

typedef struct {
    GtkWidget *window_dashboard;
    GtkStack *menu_stack;
    Students *view_students;
    Classes *view_classes;
    Sanctions *view_sanctions;
    Deliverables *view_deliverables;
    User *view_user;
} App_widgets;

App_widgets *widgets;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Event listeners
G_MODULE_EXPORT void on_select_changed(GtkWidget *widget);

G_MODULE_EXPORT void on_menu_stack_visible_child_notify(GtkStack *stack);

G_MODULE_EXPORT void on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher);

G_MODULE_EXPORT void on_sanctions_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path);

G_MODULE_EXPORT void on_classes_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path);

G_MODULE_EXPORT void on_students_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path);

G_MODULE_EXPORT void on_deliverables_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path);

G_MODULE_EXPORT void on_classes_view_refresh_button_clicked();

G_MODULE_EXPORT void on_classes_view_delete_button_clicked();

G_MODULE_EXPORT void on_classes_view_create_button_clicked();

G_MODULE_EXPORT void on_class_edit_return_button_clicked();

G_MODULE_EXPORT void on_class_edit_submit_button_clicked();

G_MODULE_EXPORT void on_class_create_return_button_clicked();

G_MODULE_EXPORT void on_class_create_submit_button_clicked();

G_MODULE_EXPORT void on_students_view_refresh_button_clicked();

G_MODULE_EXPORT void on_students_view_delete_button_clicked();

G_MODULE_EXPORT void on_students_view_create_button_clicked();

G_MODULE_EXPORT void on_students_view_remove_bottle_button_clicked();

G_MODULE_EXPORT void on_students_view_add_bottle_button_clicked();

G_MODULE_EXPORT void on_sanctions_view_refresh_button_clicked();

G_MODULE_EXPORT void on_sanctions_view_delete_button_clicked();

G_MODULE_EXPORT void on_sanctions_view_create_button_clicked();

G_MODULE_EXPORT void on_deliverables_view_refresh_button_clicked();

G_MODULE_EXPORT void on_deliverables_view_delete_button_clicked();

G_MODULE_EXPORT void on_deliverables_view_create_button_clicked();

G_MODULE_EXPORT void on_view_user_image_file_picker_file_set();

//Functions prototype
guint get_id_row_activated(GtkTreeView *tree_view, GtkTreePath *path);

guint get_id_row_selected(GtkTreeSelection *selection);

void on_destroy();

void GTKListStudents();

void GTKListClasses();

void GTKListSanctions();

void GTKListDeliverables();

void GTKUser();

void GTKUserGetData(int *id, char **email, char **first_name, char **last_name, char **photo, char **birthdate);

void GTKUserImage(char *path);

void connectWidgets();

void dashboardGTK(int *argc, char ***argv);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //BAD_CODE_GTKFUNCTIONS_H
