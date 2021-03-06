#ifndef BAD_CODE_GTKFUNCTIONS_H
#define BAD_CODE_GTKFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <math.h>
#include <windows.h>

// Get builder and glade file from main.c
extern GtkBuilder *builder;
extern char *gladeFile;

// Structure holding all the widgets related to the students
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
    GtkButton *student_edit_return_button;
    GtkButton *student_edit_submit_button;
    GtkButton *student_create_return_button;
    GtkButton *student_create_submit_button;
    GtkLabel *edit_student_id;
    GtkEntry *edit_student_first_name;
    GtkEntry *edit_student_last_name;
    GtkEntry *edit_student_email;
    GtkSpinButton *edit_student_bottles;
    GtkComboBoxText *edit_student_class;
    GtkImage *edit_student_image;
    GtkFileChooserButton *edit_student_image_file_picker;
    GtkEntry *create_student_first_name;
    GtkEntry *create_student_last_name;
    GtkEntry *create_student_email;
    GtkComboBoxText *create_student_class;
    GtkFileChooserButton *create_student_image_file_picker;
    GtkButton *create_student_image_clear_button;
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

// Structure holding all the widgets related to the classes
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
    GtkLabel *edit_class_id;
    GtkEntry *edit_class_name;
    GtkEntry *edit_class_major;
    GtkComboBoxText *edit_class_sanction;
    GtkComboBoxText *edit_class_user;
    GtkCheckButton *edit_class_apprenticeship;
    GtkSpinButton *edit_class_year;
    GtkEntry *create_class_name;
    GtkEntry *create_class_major;
    GtkComboBoxText *create_class_sanction;
    GtkComboBoxText *create_class_user;
    GtkCheckButton *create_class_apprenticeship;
    GtkSpinButton *create_class_year;
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

// Structure holding all the widgets related to the sanctions
typedef struct {
    GtkStack *view_sanctions_stack;
    GtkWidget *view_sanctions_fixed;
    GtkWidget *edit_sanction_fixed;
    GtkWidget *create_sanction_fixed;
    GtkWidget *view_sanctions_view;
    GtkButton *sanctions_view_delete_button;
    GtkButton *sanctions_view_create_button;
    GtkButton *sanctions_view_refresh_button;
    GtkButton *sanction_edit_return_button;
    GtkButton *sanction_edit_submit_button;
    GtkButton *sanction_create_return_button;
    GtkButton *sanction_create_submit_button;
    GtkLabel *edit_sanction_id;
    GtkEntry *edit_sanction_name;
    GtkTextView *edit_sanction_description;
    GtkComboBoxText *edit_sanction_user;
    GtkEntry *create_sanction_name;
    GtkTextView *create_sanction_description;
    GtkComboBoxText *create_sanction_user;
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

// Structure holding all the widgets related to the user
typedef struct {
    GtkStack *view_user_stack;
    GtkWidget *view_user_fixed;
    GtkWidget *edit_user_fixed;
    GtkButton *user_view_edit_button;
    GtkButton *user_edit_submit_button;
    GtkButton *user_edit_return_button;
    GtkImage *view_user_image;
    GtkLabel *view_user_first_name;
    GtkLabel *view_user_last_name;
    GtkLabel *view_user_birthdate;
    GtkLinkButton *view_user_email;
    GtkEntry *edit_user_first_name;
    GtkEntry *edit_user_last_name;
    GtkEntry *edit_user_email;
    GtkLabel *edit_user_birthdate;
    GtkFileChooserButton *view_user_image_file_picker;
} User;

// Structure holding all the widgets related to the deliverables
typedef struct {
    GtkStack *view_deliverables_stack;
    GtkWidget *view_deliverables_fixed;
    GtkWidget *edit_deliverable_fixed;
    GtkWidget *create_deliverable_fixed;
    GtkWidget *view_deliverables_view;
    GtkButton *deliverables_view_delete_button;
    GtkButton *deliverables_view_create_button;
    GtkButton *deliverables_view_refresh_button;
    GtkButton *deliverable_edit_return_button;
    GtkButton *deliverable_edit_submit_button;
    GtkButton *deliverable_create_return_button;
    GtkButton *deliverable_create_submit_button;
    GtkLabel *edit_deliverable_id;
    GtkLabel *edit_deliverable_student_fk;
    GtkLabel *edit_deliverable_sanction_name;
    GtkLabel *edit_deliverable_student_name;
    GtkTextView *edit_deliverable_sanction_description;
    GtkEntry *edit_deliverable_subject;
    GtkComboBoxText *edit_deliverable_status;
    GtkCalendar *edit_deliverable_due_date;
    GtkFileChooserButton *edit_deliverable_audio;
    GtkFileChooserButton *edit_deliverable_video;
    GtkFileChooserButton *edit_deliverable_bad_code;
    GtkFileChooserButton *edit_deliverable_deliverable_file;
    GtkButton *edit_deliverable_audio_download;
    GtkButton *edit_deliverable_video_download;
    GtkButton *edit_deliverable_bad_code_download;
    GtkButton *edit_deliverable_deliverable_file_download;
    GtkLabel *create_deliverable_student_fk;
    GtkLabel *create_deliverable_sanction_name;
    GtkLabel *create_deliverable_student_name;
    GtkTextView *create_deliverable_sanction_description;
    GtkEntry *create_deliverable_subject;
    GtkComboBoxText *create_deliverable_status;
    GtkCalendar *create_deliverable_due_date;
    GtkFileChooserButton *create_deliverable_audio;
    GtkFileChooserButton *create_deliverable_video;
    GtkFileChooserButton *create_deliverable_bad_code;
    GtkFileChooserButton *create_deliverable_deliverable_file;
    GtkButton *create_deliverable_audio_clear;
    GtkButton *create_deliverable_video_clear;
    GtkButton *create_deliverable_bad_code_clear;
    GtkButton *create_deliverable_deliverable_file_clear;
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

// Structure holding all the widgets related to the settings
typedef struct {
    GtkSwitch *view_settings_switch_theme_button;
    GtkSwitch *view_settings_show_terminal_button;
    GtkFileChooserButton *settings_database_file_chooser;
    GtkButton *settings_database_refresh;
    GtkFileChooserButton *settings_storage_folder_chooser;
    GtkButton *settings_storage_refresh;
    GtkFileChooserButton *settings_glade_file_chooser;
    GtkButton *settings_glade_refresh;
    GtkFileChooserButton *settings_default_theme_file_chooser;
    GtkButton *settings_default_theme_refresh;
    GtkFileChooserButton *settings_dark_theme_file_chooser;
    GtkButton *settings_dark_theme_refresh;
    GtkButton *view_settings_submit_button;
} Settings;

// Structure holding all the widgets of the app
typedef struct {
    GtkWidget *window_dashboard;
    GtkFixed *gtk_fixed;
    GtkStack *menu_stack;
    GtkStackSwitcher *menu_stack_switcher;
    GtkSearchEntry *search_entry;
    Students *view_students;
    Classes *view_classes;
    Sanctions *view_sanctions;
    Deliverables *view_deliverables;
    User *view_user;
    Settings *view_settings;
} App_widgets;

App_widgets *widgets;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Event listeners
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

G_MODULE_EXPORT void on_student_edit_return_button_clicked();

G_MODULE_EXPORT void on_student_edit_submit_button_clicked();

G_MODULE_EXPORT void on_create_student_image_clear_button_clicked();

G_MODULE_EXPORT void on_student_create_return_button_clicked();

G_MODULE_EXPORT void on_student_create_submit_button_clicked();

G_MODULE_EXPORT void on_edit_student_image_file_picker_file_set();

G_MODULE_EXPORT void on_sanctions_view_refresh_button_clicked();

G_MODULE_EXPORT void on_sanctions_view_delete_button_clicked();

G_MODULE_EXPORT void on_sanctions_view_create_button_clicked();

G_MODULE_EXPORT void on_sanction_edit_return_button_clicked();

G_MODULE_EXPORT void on_sanction_edit_submit_button_clicked();

G_MODULE_EXPORT void on_sanction_create_return_button_clicked();

G_MODULE_EXPORT void on_sanction_create_submit_button_clicked();

G_MODULE_EXPORT void on_deliverables_view_refresh_button_clicked();

G_MODULE_EXPORT void on_deliverables_view_delete_button_clicked();

G_MODULE_EXPORT void on_deliverables_view_create_button_clicked();

G_MODULE_EXPORT void on_deliverable_edit_return_button_clicked();

G_MODULE_EXPORT void on_deliverable_edit_submit_button_clicked();

G_MODULE_EXPORT void on_deliverable_create_return_button_clicked();

G_MODULE_EXPORT void on_deliverable_create_submit_button_clicked();

G_MODULE_EXPORT void on_edit_deliverable_video_file_set();

G_MODULE_EXPORT void on_edit_deliverable_bad_code_file_set();

G_MODULE_EXPORT void on_edit_deliverable_deliverable_file_file_set();

G_MODULE_EXPORT void on_edit_deliverable_audio_file_set();

G_MODULE_EXPORT void on_edit_deliverable_audio_download_clicked();

G_MODULE_EXPORT void on_edit_deliverable_video_download_clicked();

G_MODULE_EXPORT void on_edit_deliverable_bad_code_download_clicked();

G_MODULE_EXPORT void on_edit_deliverable_deliverable_file_download_clicked();

G_MODULE_EXPORT void on_create_deliverable_audio_clear_clicked();

G_MODULE_EXPORT void on_create_deliverable_video_clear_clicked();

G_MODULE_EXPORT void on_create_deliverable_bad_code_clear_clicked();

G_MODULE_EXPORT void on_create_deliverable_deliverable_file_clear_clicked();

G_MODULE_EXPORT void on_create_deliverable_video_file_set();

G_MODULE_EXPORT void on_create_deliverable_bad_code_file_set();

G_MODULE_EXPORT void on_create_deliverable_deliverable_file_file_set();

G_MODULE_EXPORT void on_create_deliverable_audio_file_set();

G_MODULE_EXPORT void on_view_user_image_file_picker_file_set();

G_MODULE_EXPORT void on_user_view_edit_button_clicked();

G_MODULE_EXPORT void on_user_edit_submit_button_clicked();

G_MODULE_EXPORT void on_user_edit_return_button_clicked();

G_MODULE_EXPORT void on_view_settings_switch_theme_button_state_set();

G_MODULE_EXPORT void on_view_settings_show_terminal_button_state_set();

G_MODULE_EXPORT void on_view_settings_submit_button_clicked();

G_MODULE_EXPORT void on_settings_storage_refresh_clicked();

G_MODULE_EXPORT void on_settings_dark_theme_refresh_clicked();

G_MODULE_EXPORT void on_settings_default_theme_refresh_clicked();

G_MODULE_EXPORT void on_settings_glade_refresh_clicked();

G_MODULE_EXPORT void on_settings_database_refresh_clicked();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions prototypes
guint get_id_row_activated(GtkTreeView *tree_view, GtkTreePath *path);

guint get_id_row_selected(GtkTreeSelection *selection);

void on_destroy();

void fillUserComboList(GtkComboBoxText *comboBoxText);

void fillClassComboList(GtkComboBoxText *comboBoxText);

void fillSanctionComboList(GtkComboBoxText *comboBoxText);

void fillStatusComboList(GtkComboBoxText *comboBoxText, char *status);

void GTKListStudents();

void GTKEditStudent(int id);

void GTKEditStudentSubmit();

int GTKEditStudentSubmitCheckRequiredField();

void GTKStudentGetData(int id, char **first_name, char **last_name, char **photo, char **email, char **bottles,
                       char **class, char **class_fk);

void GTKEditStudentImage(char *path);

int GTKEditStudentSetImage(char *path);

void GTKCreateStudent();

void GTKCreateStudentSubmit();

int GTKCreateStudentSubmitCheckRequiredField();

void GTKListClasses();

void GTKEditClass(int id);

void GTKEditClassSubmit();

int GTKEditClassSubmitCheckRequiredField();

void GTKClassGetData(int id, char **name, char **year, int *apprenticeship, char **major, char **user, char **user_fk,
                     char **sanction, char **sanction_fk);

void GTKCreateClass();

void GTKCreateClassSubmit();

int GTKCreateClassSubmitCheckRequiredField();

void GTKListSanctions();

void GTKEditSanction(int id);

void GTKEditSanctionSubmit();

int GTKEditSanctionSubmitCheckRequiredField(char *textIter);

void GTKSanctionGetData(int id, char **name, char **description, char **user, char **user_fk);

void GTKSanctionGetDataStudentId(int student_id, char **name, char **description, char **student, char **student_fk);

void GTKCreateSanction();

void GTKCreateSanctionSubmit();

int GTKCreateSanctionSubmitCheckRequiredField(char *textIter);

void GTKListDeliverables();

void GTKEditDeliverables(int id);

void GTKEditDelivreablesSubmit();

int GTKEditDeliverableSubmitCheckRequiredField();

void GTKDelivreablesGetData(int id, char **due_date, char **subject, char **audio_record, char **video_record,
                            char **bad_code, char **deliverable_file, char **status, char **student,
                            char **student_fk,
                            char **sanction_name, char **sanction_description);

void GTKEditDeliverableSetDueDate(char *date);

int GTKDeliverableSetAudio(char *path);

int GTKDeliverableSetVideo(char *path);

int GTKDeliverableSetBadCode(char *path);

int GTKDeliverableSetDeliverable(char *path);

void GTKCreateDelivreables(int student_fk);

void GTKCreateDelivreablesSubmit();

int GTKCreateDeliverableSubmitCheckRequiredField();

void GTKViewUser();

void GTKEditUser();

void GTKEditUserSubmit();

int GTKEditUserSubmitCheckRequiredField();

void GTKUserGetData(int *id, char **email, char **first_name, char **last_name, char **photo, char **birthdate);

void GTKUserImage(char *path);

int GTKUserSetImage(char *path);

void GTKViewSettings();

void GTKViewSettingsSubmit();

void GTKSetTheme();

void GTKShowConsole();

void connectWidgets();

void setSearchEntry(gboolean visible, GtkTreeView *treeView, const char *placeholder);

void GTKSaveFile(char *path);

void dashboardGTK(int *argc, char ***argv);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //BAD_CODE_GTKFUNCTIONS_H
