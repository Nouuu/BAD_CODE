//
// Created by MaleWhere on 30/01/2020.
//

#include "gtkFunctions.h"
#include "sqliteFunctions.h"
#include "functions.h"

//TODO check strlen on all submit to avoid some empty required field

void on_destroy() {
    gtk_main_quit();
}

void on_sanctions_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("SANCTION ID: %d\n", id);
    GTKEditSanction(id);
}

void on_classes_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("CLASS ID: %d\n", id);
    GTKEditClass(id);
}

void on_students_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("STUDENT ID: %d\n", id);
    GTKEditStudent(id);
}

void on_deliverables_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("DELIVERABLE ID: %d\n", id);
    GTKEditDelivreables(id);
}

void on_menu_stack_visible_child_notify(GtkStack *stack) {
    if (gtk_stack_get_visible_child_name(stack) != NULL) {
        const gchar *menu = gtk_stack_get_visible_child_name(widgets->menu_stack);
        if (!strcmp(menu, "view_classes")) {
            printf("Classes view\n");
            setSearchEntry(FALSE, NULL, NULL);
            GTKListClasses();
        } else if (!strcmp(menu, "view_students")) {
            printf("Students view\n");
            setSearchEntry(TRUE, widgets->view_students->students_tree_view, "Search by first name");
            GTKListStudents();
        } else if (!strcmp(menu, "view_sanctions")) {
            printf("Sanctions view\n");
            setSearchEntry(FALSE, NULL, NULL);
            GTKListSanctions();
        } else if (!strcmp(menu, "view_deliverables")) {
            printf("Deliverables view\n");
            setSearchEntry(TRUE, widgets->view_deliverables->deliverables_tree_view, "Search by student name");
            GTKListDeliverables();
        } else if (!strcmp(menu, "view_user")) {
            printf("User view\n");
            setSearchEntry(FALSE, NULL, NULL);
            GTKViewUser();
        } else if (!strcmp(menu, "view_settings")) {
            printf("Settings view\n");
            setSearchEntry(FALSE, NULL, NULL);
            GTKViewSettings();
        }
    }
}

void on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher) {
    if (gtk_stack_switcher_get_stack(stackSwitcher) != NULL)
        on_menu_stack_visible_child_notify(gtk_stack_switcher_get_stack(stackSwitcher));
}

void on_classes_view_refresh_button_clicked() {
    printf("Refresh classes\n");
    GTKListClasses();
}

void on_classes_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_classes->classes_tree_selection);
    if (int_data) {
        deleteClass(int_data);
        GTKListClasses();
    }
    printf("Delete class ID: %d\n", int_data);
}

void on_classes_view_create_button_clicked() {
    printf("Create class\n");
    GTKCreateClass();
}

void on_class_edit_return_button_clicked() {
    printf("Return to classes view\n");
    GTKListClasses();
}

void on_class_edit_submit_button_clicked() {
    printf("Submit class edit\n");
    GTKEditClassSubmit();
}

void on_class_create_return_button_clicked() {
    printf("Return to classes view\n");
    GTKListClasses();
}

void on_class_create_submit_button_clicked() {
    printf("Submit class create\n");
    GTKCreateClassSubmit();
}

void on_students_view_refresh_button_clicked() {
    printf("Refresh students\n");
    GTKListStudents();
}

void on_students_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        deleteStudent(int_data);
        GTKListStudents();
    }
    printf("Delete student ID: %d\n", int_data);
}

void on_students_view_create_button_clicked() {
    printf("Create student\n");
    GTKCreateStudent();
}

void on_students_view_remove_bottle_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        addStudentBottle(int_data, -1);
        GTKListStudents();
    }
    printf("Remove bottle student ID: %d\n", int_data);
}

void on_students_view_add_bottle_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        addStudentBottle(int_data, 1);
        GTKListStudents();
    }
    printf("Add bottle student ID: %d\n", int_data);
}

void on_student_edit_return_button_clicked() {
    printf("Return to students view\n");
    GTKListStudents();
}

void on_student_edit_submit_button_clicked() {
    printf("Submit student edit\n");
    GTKEditStudentSubmit();
}

void on_student_create_return_button_clicked() {
    printf("Return to students view\n");
    GTKListStudents();
}

void on_student_create_submit_button_clicked() {
    printf("Submit student create\n");
    GTKCreateStudentSubmit();
}

void on_sanctions_view_refresh_button_clicked() {
    printf("Refresh sanctions\n");
    GTKListSanctions();
}

void on_edit_student_image_file_picker_file_set() {
    char *path = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_students->edit_student_image_file_picker));
    printf("Choose file! : %s\n", path);

    int returnCode = GTKEditStudentSetImage(path);

    if (returnCode)
        fprintf(stderr, "Can't use this image\n");
    else {
        printf("Image changed\n");
    }
}

void on_sanctions_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_sanctions->sanctions_tree_selection);
    if (int_data) {
        deleteSanction(int_data);
        GTKListSanctions();
    }
    printf("Delete sanction ID: %d\n", int_data);
}

void on_sanctions_view_create_button_clicked() {
    printf("Create sanction\n");
    GTKCreateSanction();
}

void on_sanction_edit_return_button_clicked() {
    printf("Return to sanctions view\n");
    GTKListSanctions();
}

void on_sanction_edit_submit_button_clicked() {
    printf("Submit sanction edit\n");
    GTKEditSanctionSubmit();
}

void on_sanction_create_return_button_clicked() {
    printf("Return to sanctions view\n");
    GTKListSanctions();
}

void on_sanction_create_submit_button_clicked() {
    printf("Submit sanction create\n");
    GTKCreateSanctionSubmit();
}

void on_deliverables_view_refresh_button_clicked() {
    printf("Refresh deliverables\n");
    GTKListDeliverables();
}

void on_deliverables_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_deliverables->deliverables_tree_selection);
    if (int_data) {
        deleteDeliverable(int_data);
        GTKListDeliverables();
    }
    printf("Delete deliverable ID: %d\n", int_data);
}

void on_deliverables_view_create_button_clicked() {

    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {

        printf("Create deliverable student id: %d\n", int_data);
        GTKCreateDelivreables(int_data);
    }
}

void on_deliverable_edit_return_button_clicked() {
    printf("Return to deliverables view\n");
    GTKListDeliverables();
}

void on_deliverable_edit_submit_button_clicked() {
    printf("Submit deliverable edit\n");
    GTKEditDelivreablesSubmit();
}

void on_create_student_image_clear_button_clicked() {
    printf("Clear student create image");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_students->create_student_image_file_picker));
}

void on_deliverable_create_return_button_clicked() {
    printf("Return to deliverables view\n");
    GTKListDeliverables();
}

void on_deliverable_create_submit_button_clicked() {
    printf("Submit deliverable create\n");
    GTKCreateDelivreablesSubmit();
}

void on_edit_deliverable_video_file_set() {
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_video));
    printf("Choose video file! : %s\n", path);
    if (GTKDeliverableSetVideo(path))
        fprintf(stderr, "Error while adding video\n");
}

void on_edit_deliverable_bad_code_file_set() {
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_bad_code));
    printf("Choose bad code file! : %s\n", path);
    if (GTKDeliverableSetBadCode(path))
        fprintf(stderr, "Error while adding bad code\n");

}

void on_edit_deliverable_deliverable_file_file_set() {
    char *path = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_deliverable_file));
    printf("Choose deliverable file! : %s\n", path);
    if (GTKDeliverableSetDeliverable(path))
        fprintf(stderr, "Error while adding deliverable file\n");
}

void on_edit_deliverable_audio_file_set() {
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_audio));
    printf("Choose audio file! : %s\n", path);
    if (GTKDeliverableSetAudio(path))
        fprintf(stderr, "Error while adding audio\n");
}

void on_edit_deliverable_audio_download_clicked() {
    char *path = gtk_widget_get_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download));

    printf("Download audio: %s\n", path);
    GTKSaveFile(path);
}

void on_edit_deliverable_video_download_clicked() {
    char *path = gtk_widget_get_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download));

    printf("Download video\n");
    GTKSaveFile(path);
}

void on_edit_deliverable_bad_code_download_clicked() {
    char *path = gtk_widget_get_tooltip_text(
            GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download));

    printf("Download bad code\n");
    GTKSaveFile(path);
}

void on_edit_deliverable_deliverable_file_download_clicked() {
    char *path = gtk_widget_get_tooltip_text(
            GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download));

    printf("Download deliverable\n");
    GTKSaveFile(path);
}

void on_create_deliverable_audio_clear_clicked() {
    printf("Clear audio\n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_audio));
}

void on_create_deliverable_video_clear_clicked() {
    printf("Clear video\n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_video));
}

void on_create_deliverable_bad_code_clear_clicked() {
    printf("Clear bad_code\n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_bad_code));
}

void on_create_deliverable_deliverable_file_clear_clicked() {
    printf("Clear deliverable\n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_deliverable_file));
}

void on_create_deliverable_video_file_set() {

}

void on_create_deliverable_bad_code_file_set() {

}

void on_create_deliverable_deliverable_file_file_set() {

}

void on_create_deliverable_audio_file_set() {

}

void on_view_user_image_file_picker_file_set() {
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_user->view_user_image_file_picker));
    printf("Choose file! : %s\n", path);

    int returnCode = GTKUserSetImage(path);

    if (returnCode)
        fprintf(stderr, "Can't use this image\n");
    else {
        printf("Image changed\n");
        GTKViewUser();
    }
}

void on_user_view_edit_button_clicked() {
    printf("Edit user\n");
    GTKEditUser();
}

void on_user_edit_submit_button_clicked() {
    printf("User edit submit\n");
    GTKEditUserSubmit();
}

void on_user_edit_return_button_clicked() {
    printf("User return to view\n");
    GTKViewUser();
}

void on_view_settings_switch_theme_button_state_set() {
    printf("Switch theme button !\n");

    darkTheme = gtk_switch_get_active(widgets->view_settings->view_settings_switch_theme_button);

    GTKSetTheme();

    writeConf();
}

void on_view_settings_submit_button_clicked() {
    printf("Submit settings \n");
    GTKViewSettingsSubmit();
}

void on_settings_storage_refresh_clicked() {
    printf("storage refresh \n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser));
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser),
                                        storageFolder);
}

void on_settings_dark_theme_refresh_clicked() {
    printf("dark theme refresh \n");
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser),
                                  darkThemePath);
}

void on_settings_default_theme_refresh_clicked() {
    printf("default theme refresh \n");
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser),
                                  defaultThemePath);
}

void on_settings_glade_refresh_clicked() {
    printf("glade refresh \n");
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser), gladeFile);
}

void on_settings_database_refresh_clicked() {
    printf("database refresh \n");
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser), dbname);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

guint get_id_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint int_data;

    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &int_data, -1);
        return int_data;
    } else {
        fprintf(stderr, "Error! selected column not found!\n");
        return EXIT_FAILURE;
    }
}

guint get_id_row_selected(GtkTreeSelection *selection) {
    guint int_data = 0;
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &int_data, -1);
    }
    return int_data;
}

void fillUserComboList(GtkComboBoxText *comboBoxText) {
    char *user;
    getUser(&user, 1);
    size_t columnSize, nameSize;
    char *firstAdress = user, *nameBuffer, *idBuffer;

    //ID
    columnSize = strchr(user, '|') - user;
    idBuffer = malloc(columnSize + 1);
    strncpy(idBuffer, user, columnSize);
    idBuffer[columnSize] = '\0';

    user += columnSize + 1;

    //EMAIL
    user += strchr(user, '|') - user + 1;

    //FIRST_NAME
    columnSize = strchr(user, '|') - user;
    nameBuffer = malloc(columnSize + 1);
    strncpy(nameBuffer, user, columnSize);
    nameBuffer[columnSize] = '\0';

    user += columnSize + 1;

    //LAST_NAME
    columnSize = strchr(user, '|') - user;
    nameBuffer = realloc(nameBuffer, strlen(nameBuffer) + columnSize + 1);
    strcat(nameBuffer, " ");
    nameSize = strlen(nameBuffer);
    strncat(nameBuffer, user, columnSize);
    nameBuffer[columnSize + nameSize] = '\0';

    gtk_combo_box_text_remove_all(comboBoxText);
    gtk_combo_box_text_append(comboBoxText, idBuffer, nameBuffer);

    free(idBuffer);
    free(nameBuffer);
    free(firstAdress);
}

void fillClassComboList(GtkComboBoxText *comboBoxText) {
    char *classList;
    listClasses(&classList);

    int nbSanctions = 0, i;
    char *result = classList, *firstAdress = classList, *nameBuffer, *idBuffer;
    size_t columnSize;


    gtk_combo_box_text_remove_all(comboBoxText);


    while ((result = strstr(result, ";\n"))) {
        nbSanctions++;
        result++;
    }

    for (i = 0; i < nbSanctions; ++i) {
        //ID
        result = strchr(classList, '|');
        columnSize = result - classList;
        idBuffer = malloc(columnSize + 1);

        strncpy(idBuffer, classList, columnSize);
        idBuffer[columnSize] = '\0';
        classList += columnSize + 1;

        //NAME
        result = strchr(classList, '|');
        columnSize = result - classList;
        nameBuffer = malloc(columnSize + 1);

        strncpy(nameBuffer, classList, columnSize);
        nameBuffer[columnSize] = '\0';

        gtk_combo_box_text_append(comboBoxText, idBuffer, nameBuffer);

        classList = strstr(classList, ";\n") + 2;
        free(idBuffer);
        free(nameBuffer);
    }

    free(firstAdress);
}

void fillSanctionComboList(GtkComboBoxText *comboBoxText) {
    char *sanctionsList;
    listSanctions(&sanctionsList);

    int nbSanctions = 0, i;
    char *result = sanctionsList, *firstAdress = sanctionsList, *nameBuffer, *idBuffer;
    size_t columnSize;


    gtk_combo_box_text_remove_all(comboBoxText);
    gtk_combo_box_text_append(comboBoxText, "0", "None");


    while ((result = strstr(result, ";\n"))) {
        nbSanctions++;
        result++;
    }

    for (i = 0; i < nbSanctions; ++i) {
        //ID
        result = strchr(sanctionsList, '|');
        columnSize = result - sanctionsList;
        idBuffer = malloc(columnSize + 1);

        strncpy(idBuffer, sanctionsList, columnSize);
        idBuffer[columnSize] = '\0';
        sanctionsList += columnSize + 1;

        //NAME
        result = strchr(sanctionsList, '|');
        columnSize = result - sanctionsList;
        nameBuffer = malloc(columnSize + 1);

        strncpy(nameBuffer, sanctionsList, columnSize);
        nameBuffer[columnSize] = '\0';

        gtk_combo_box_text_append(comboBoxText, idBuffer, nameBuffer);

        sanctionsList = strstr(sanctionsList, ";\n") + 2;
        free(idBuffer);
        free(nameBuffer);
    }

    free(firstAdress);
}

void fillStatusComboList(GtkComboBoxText *comboBoxText, char *status) {
    gtk_combo_box_text_remove_all(comboBoxText);

    gtk_combo_box_text_append(comboBoxText, "1", "To do");
    if (!strcmp(status, "To do"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "1");

    gtk_combo_box_text_append(comboBoxText, "2", "Completed");
    if (!strcmp(status, "Completed"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "2");

    gtk_combo_box_text_append(comboBoxText, "3", "On hold");
    if (!strcmp(status, "On hold"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "3");

    gtk_combo_box_text_append(comboBoxText, "4", "Checking");
    if (!strcmp(status, "Checking"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "4");

    gtk_combo_box_text_append(comboBoxText, "5", "Waiting deliverable");
    if (!strcmp(status, "Waiting deliverable"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "5");

}

void GTKListStudents() {
    gtk_stack_set_visible_child(widgets->view_students->view_students_stack,
                                widgets->view_students->view_students_fixed);
    char *students, *result, *firstAddress;
    int nbStudents = 0;
    listStudents(&students);
    firstAddress = students;
    result = students;

    while ((result = strstr(result, ";\n"))) {
        nbStudents++;
        result++;
    }

    GtkTreeIter iter;
    gtk_tree_store_clear(widgets->view_students->students_tree_store);

    for (int i = 0; i < nbStudents; ++i) {
        gtk_tree_store_append(widgets->view_students->students_tree_store, &iter, NULL);

        //ID
        result = strchr(students, '|');
        size_t columnSize = result - students;
        char *buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 0, atoi(buffer), -1);
        students += columnSize + 1;

        //FIRST_NAME
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 1, buffer, -1);
        students += columnSize + 1;

        //LAST_NAME
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 2, buffer, -1);
        students += columnSize + 1;

        students += strchr(students, '|') - students + 1;

        //EMAIL
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 4, buffer, -1);
        students += columnSize + 1;

        //BAD_CODE
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 5, atoi(buffer), -1);
        students += columnSize + 1;

        //NB_BOTTLES
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 6, atoi(buffer), -1);
        students += columnSize + 1;

        //CLASS
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 7, buffer, -1);
        students += columnSize + 1;

        //CLASS_FK
        free(buffer);
        result = strstr(students, ";\n");
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 8, atoi(buffer), -1);
        students += columnSize + 2;

    }


    free(firstAddress);
}

void GTKEditStudent(int id) {
    gtk_stack_set_visible_child(widgets->view_students->view_students_stack,
                                widgets->view_students->edit_student_fixed);
    char *first_name, *last_name, *photo, *email, *bottles, *class, *class_fk, idBuffer[6];

    fillClassComboList(widgets->view_students->edit_student_class);
    GTKStudentGetData(id, &first_name, &last_name, &photo, &email, &bottles, &class, &class_fk);
    itoa(id, idBuffer, 10);

    GTKEditStudentImage(photo);

    gtk_label_set_text(widgets->view_students->edit_student_id, idBuffer);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_students->edit_student_id), FALSE);
    gtk_entry_set_text(widgets->view_students->edit_student_first_name, first_name);
    gtk_entry_set_text(widgets->view_students->edit_student_last_name, last_name);
    gtk_entry_set_text(widgets->view_students->edit_student_email, email);
    gtk_entry_set_text(GTK_ENTRY(widgets->view_students->edit_student_bottles), bottles);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_students->edit_student_class), class_fk);
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_students->edit_student_image_file_picker));

    free(first_name);
    free(last_name);
    free(photo);
    free(email);
    free(bottles);
    free(class);
    free(class_fk);
}

void GTKEditStudentSubmit() {

    int returnCode = GTKEditStudentSubmitCheckRequiredField() ||
                     updateStudent(atoi(gtk_label_get_text(widgets->view_students->edit_student_id)),
                                   gtk_entry_get_text(widgets->view_students->edit_student_first_name),
                                   gtk_entry_get_text(widgets->view_students->edit_student_last_name),
                                   gtk_entry_get_text(widgets->view_students->edit_student_email),
                                   atoi(gtk_entry_get_text(GTK_ENTRY(widgets->view_students->edit_student_bottles))),
                                   atoi(gtk_combo_box_get_active_id(
                                           GTK_COMBO_BOX(widgets->view_students->edit_student_class))));
    if (returnCode)
        fprintf(stderr, "Error, could not update student\n");
    else {
        printf("Student update successful\n");
        GTKListStudents();
    }
}

int GTKEditStudentSubmitCheckRequiredField() {
    int returnCode = 0;
    if (atoi(gtk_entry_get_text(GTK_ENTRY(widgets->view_students->edit_student_bottles))) < 0)
        gtk_entry_set_text(GTK_ENTRY(widgets->view_students->edit_student_bottles), "0");
    if (strlen(gtk_entry_get_text(widgets->view_students->edit_student_first_name)) == 0) {
        fprintf(stderr, "First name empty !\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->edit_student_last_name)) == 0) {
        fprintf(stderr, "Last name empty !\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->edit_student_email)) == 0) {
        fprintf(stderr, "Email empty !\n");
        returnCode = 1;
    }

    return returnCode;
}

void GTKStudentGetData(int id, char **first_name, char **last_name, char **photo, char **email, char **bottles,
                       char **class, char **class_fk) {
    char *studentData, *firstAdress;
    size_t columnSize;
    getStudent(&studentData, id);
    firstAdress = studentData;

    //ID
    studentData += strchr(studentData, '|') - studentData + 1;

    //FIRST_NAME
    columnSize = strchr(studentData, '|') - studentData;
    *first_name = malloc(columnSize + 1);
    strncpy(*first_name, studentData, columnSize);
    (*first_name)[columnSize] = '\0';
    studentData += columnSize + 1;

    //LAST_NAME
    columnSize = strchr(studentData, '|') - studentData;
    *last_name = malloc(columnSize + 1);
    strncpy(*last_name, studentData, columnSize);
    (*last_name)[columnSize] = '\0';
    studentData += columnSize + 1;

    //PHOTO
    columnSize = strchr(studentData, '|') - studentData;
    *photo = malloc(columnSize + 1);
    strncpy(*photo, studentData, columnSize);
    (*photo)[columnSize] = '\0';
    studentData += columnSize + 1;

    //EMAIL
    columnSize = strchr(studentData, '|') - studentData;
    *email = malloc(columnSize + 1);
    strncpy(*email, studentData, columnSize);
    (*email)[columnSize] = '\0';
    studentData += columnSize + 1;

    //BADCODE COUNT
    studentData += strchr(studentData, '|') - studentData + 1;

    //BOTTLES
    columnSize = strchr(studentData, '|') - studentData;
    *bottles = malloc(columnSize + 1);
    strncpy(*bottles, studentData, columnSize);
    (*bottles)[columnSize] = '\0';
    studentData += columnSize + 1;

    //CLASS
    columnSize = strchr(studentData, '|') - studentData;
    *class = malloc(columnSize + 1);
    strncpy(*class, studentData, columnSize);
    (*class)[columnSize] = '\0';
    studentData += columnSize + 1;

    //CLASS_FK
    columnSize = strstr(studentData, ";\n") - studentData;
    *class_fk = malloc(columnSize + 1);
    strncpy(*class_fk, studentData, columnSize);
    (*class_fk)[columnSize] = '\0';
    studentData += columnSize + 1;

    free(firstAdress);
}

void GTKEditStudentImage(char *path) {
    gtk_image_clear(widgets->view_students->edit_student_image);
    GdkPixbuf *pixbuf;
    if (strlen(path) < 1) {
        char *defaultProfilPicture = malloc(strlen(storageFolder) + 12);
        sprintf(defaultProfilPicture, "%s/profil.png", storageFolder);
        if ((pixbuf = gdk_pixbuf_new_from_file(defaultProfilPicture, NULL)) == NULL) {
            fprintf(stderr, "Error while loading default user profil picture\n");
        } else {
            printf("loaded!\n");
            int width = gdk_pixbuf_get_width(pixbuf);
            int height = gdk_pixbuf_get_height(pixbuf);
            double ratio = (250. / width);
            printf("width: %d, height: %d, ratio: %lf\n", width, height, ratio);
            gtk_image_set_from_pixbuf(widgets->view_students->edit_student_image,
                                      gdk_pixbuf_scale_simple(pixbuf, floor(width * ratio), floor(height * ratio),
                                                              GDK_INTERP_BILINEAR));
        }
        free(defaultProfilPicture);
    } else {
        if ((pixbuf = gdk_pixbuf_new_from_file(path, NULL)) == NULL) {
            fprintf(stderr, "Error while loading user profil picture\n");
        } else {
            printf("loaded!\n");
            int width = gdk_pixbuf_get_width(pixbuf);
            int height = gdk_pixbuf_get_height(pixbuf);
            double ratio = (250. / width);
            printf("width: %d, height: %d, ratio: %lf\n", width, height, ratio);
            gtk_image_set_from_pixbuf(widgets->view_students->edit_student_image,
                                      gdk_pixbuf_scale_simple(pixbuf, floor(width * ratio), floor(height * ratio),
                                                              GDK_INTERP_BILINEAR));
        }
    }
}

int GTKEditStudentSetImage(char *path) {
    if (!checkImageExtension(path))
        return 1;

    int id = atoi(gtk_label_get_text(widgets->view_students->edit_student_id));
    int returnCode = insertTableImage("student", id, path);
    if (returnCode)
        return 1;

    GTKEditStudent(id);
    return 0;
}

void GTKCreateStudent() {
    gtk_stack_set_visible_child(widgets->view_students->view_students_stack,
                                widgets->view_students->create_student_fixed);

    fillClassComboList(widgets->view_students->create_student_class);

    gtk_entry_set_text(widgets->view_students->create_student_first_name, "");
    gtk_entry_set_text(widgets->view_students->create_student_last_name, "");
    gtk_entry_set_text(widgets->view_students->create_student_email, "");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_students->create_student_class), "1");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_students->create_student_image_file_picker));
}

void GTKCreateStudentSubmit() {
    char *photo_path = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_students->create_student_image_file_picker));
    int returnCode;

    if (photo_path != NULL && strlen(photo_path) > 0 && checkImageExtension(photo_path))
        returnCode = insertStudent(gtk_entry_get_text(widgets->view_students->create_student_first_name),
                                   gtk_entry_get_text(widgets->view_students->create_student_last_name),
                                   photo_path,
                                   gtk_entry_get_text(widgets->view_students->create_student_email),
                                   atoi(gtk_combo_box_get_active_id(
                                           GTK_COMBO_BOX(widgets->view_students->create_student_class))));
    else
        returnCode = GTKCreateStudentSubmitCheckRequiredField() ||
                     insertStudent(gtk_entry_get_text(widgets->view_students->create_student_first_name),
                                   gtk_entry_get_text(widgets->view_students->create_student_last_name),
                                   NULL,
                                   gtk_entry_get_text(widgets->view_students->create_student_email),
                                   atoi(gtk_combo_box_get_active_id(
                                           GTK_COMBO_BOX(widgets->view_students->create_student_class))));

    if (returnCode)
        fprintf(stderr, "Error, could not create student\n");
    else {
        printf("Student create successful\n");
        GTKListStudents();
    }
}

int GTKCreateStudentSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_students->create_student_first_name)) == 0) {
        fprintf(stderr, "First name empty !\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->create_student_last_name)) == 0) {
        fprintf(stderr, "Last name empty !\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->create_student_email)) == 0) {
        fprintf(stderr, "Email empty !\n");
        returnCode = 1;
    }

    return returnCode;
}

void GTKListClasses() {
    gtk_stack_set_visible_child(widgets->view_classes->view_classes_stack, widgets->view_classes->view_classes_fixed);
    char *classes, *result, *firstAddress;
    int nbClasses = 0;
    listClasses(&classes);
    firstAddress = classes;
    result = classes;

    while ((result = strstr(result, ";\n"))) {
        nbClasses++;
        result++;
    }

    GtkTreeIter iter;
    gtk_tree_store_clear(widgets->view_classes->classes_tree_store);

    for (int i = 0; i < nbClasses; ++i) {
        gtk_tree_store_append(widgets->view_classes->classes_tree_store, &iter, NULL);

        //ID
        result = strchr(classes, '|');
        size_t columnSize = result - classes;
        char *buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 0, atoi(buffer), -1);
        classes += columnSize + 1;

        //NAME
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 1, buffer, -1);
        classes += columnSize + 1;

        //YEAR
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 2, atoi(buffer), -1);
        classes += columnSize + 1;

        //APPRENTICESHIP
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        if (!strcmp(buffer, "1")) {
            buffer = realloc(buffer, 4 * sizeof(char));
            strcpy(buffer, "Yes");
        } else {
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "No");
        }

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 3, buffer, -1);
        classes += columnSize + 1;

        //MAJOR
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 4, buffer, -1);
        classes += columnSize + 1;

        //USER
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 5, buffer, -1);
        classes += columnSize + 1;

        //USER_FK
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 6, atoi(buffer), -1);
        classes += columnSize + 1;

        //SANCTION
        free(buffer);
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 7, buffer, -1);
        classes += columnSize + 1;

        //SANCTION_FK
        free(buffer);
        result = strstr(classes, ";\n");
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 8, atoi(buffer), -1);
        classes += columnSize + 2;

    }

    free(firstAddress);
}

void GTKEditClass(int id) {
    gtk_stack_set_visible_child(widgets->view_classes->view_classes_stack, widgets->view_classes->edit_class_fixed);
    char *name, *major, *user, *sanction, *year, *sanction_fk, *user_fk, idBuffer[4];
    int apprenticeship;

    fillSanctionComboList(widgets->view_classes->edit_class_sanction);
    fillUserComboList(widgets->view_classes->edit_class_user);

    GTKClassGetData(id, &name, &year, &apprenticeship, &major, &user, &user_fk, &sanction, &sanction_fk);
    itoa(id, idBuffer, 10);

    gtk_label_set_text(widgets->view_classes->edit_class_id, idBuffer);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_classes->edit_class_id), FALSE);
    gtk_entry_set_text(widgets->view_classes->edit_class_name, name);
    gtk_entry_set_text(widgets->view_classes->edit_class_major, major);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_classes->edit_class_sanction), sanction_fk);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_classes->edit_class_user), user_fk);
    if (apprenticeship)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->view_classes->edit_class_apprenticeship), TRUE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->view_classes->edit_class_apprenticeship), FALSE);
    gtk_entry_set_text(GTK_ENTRY(widgets->view_classes->edit_class_year), year);

    free(name);
    free(major);
    free(user);
    free(sanction);
    free(year);
    free(sanction_fk);
    free(user_fk);
}

void GTKClassGetData(int id, char **name, char **year, int *apprenticeship, char **major, char **user, char **user_fk,
                     char **sanction, char **sanction_fk) {
    char *classData, *intBuffer, *firstAdress;
    size_t columnSize;
    getClass(&classData, id);
    firstAdress = classData;

    //ID
    classData += strchr(classData, '|') - classData + 1;

    //NAME
    columnSize = strchr(classData, '|') - classData;
    *name = malloc(columnSize + 1);
    strncpy(*name, classData, columnSize);
    (*name)[columnSize] = '\0';
    classData += columnSize + 1;

    //YEAR
    columnSize = strchr(classData, '|') - classData;
    *year = malloc(columnSize + 1);
    strncpy(*year, classData, columnSize);
    (*year)[columnSize] = '\0';
    classData += columnSize + 1;

    //APPRENTICESHIP
    columnSize = strchr(classData, '|') - classData;
    intBuffer = malloc(columnSize + 1);
    strncpy(intBuffer, classData, columnSize);
    intBuffer[columnSize] = '\0';
    *apprenticeship = atoi(intBuffer);
    free(intBuffer);
    classData += columnSize + 1;

    //MAJOR
    columnSize = strchr(classData, '|') - classData;
    *major = malloc(columnSize + 1);
    strncpy(*major, classData, columnSize);
    (*major)[columnSize] = '\0';
    classData += columnSize + 1;

    //USER
    columnSize = strchr(classData, '|') - classData;
    *user = malloc(columnSize + 1);
    strncpy(*user, classData, columnSize);
    (*user)[columnSize] = '\0';
    classData += columnSize + 1;

    //USER_FK
    columnSize = strchr(classData, '|') - classData;
    *user_fk = malloc(columnSize + 1);
    strncpy(*user_fk, classData, columnSize);
    (*user_fk)[columnSize] = '\0';
    classData += columnSize + 1;

    //SANCTION
    columnSize = strchr(classData, '|') - classData;
    *sanction = malloc(columnSize + 1);
    strncpy(*sanction, classData, columnSize);
    (*sanction)[columnSize] = '\0';
    classData += columnSize + 1;

    //SANCTION_FK
    columnSize = strstr(classData, ";\n") - classData;
    *sanction_fk = malloc(columnSize + 1);
    strncpy(*sanction_fk, classData, columnSize);
    (*sanction_fk)[columnSize] = '\0';
    classData += columnSize + 1;

    free(firstAdress);
}

void GTKEditClassSubmit() {

    int returnCode = GTKEditClassSubmitCheckRequiredField() ||
                     updateClass(atoi(gtk_label_get_text(widgets->view_classes->edit_class_id)),
                                 gtk_entry_get_text(widgets->view_classes->edit_class_name),
                                 gtk_entry_get_text(widgets->view_classes->edit_class_major),
                                 atoi(gtk_entry_get_text(GTK_ENTRY(widgets->view_classes->edit_class_year))),
                                 gtk_toggle_button_get_active(
                                         GTK_TOGGLE_BUTTON(widgets->view_classes->edit_class_apprenticeship)) ? 1 : 0,
                                 atoi(gtk_combo_box_get_active_id(
                                         GTK_COMBO_BOX(widgets->view_classes->edit_class_user))),
                                 atoi(gtk_combo_box_get_active_id(
                                         GTK_COMBO_BOX(widgets->view_classes->edit_class_sanction)))
                     );

    if (returnCode)
        fprintf(stderr, "Error, could not update class\n");
    else {
        printf("Class update successful\n");
        GTKListClasses();
    }
}

int GTKEditClassSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_classes->edit_class_name)) == 0) {
        fprintf(stderr, "Name empty !\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(widgets->view_classes->edit_class_year))) != 4) {
        fprintf(stderr, "Wrong year !\n");
        returnCode = 1;
    }
    return returnCode;
}

void GTKCreateClass() {
    gtk_stack_set_visible_child(widgets->view_classes->view_classes_stack, widgets->view_classes->create_class_fixed);

    fillSanctionComboList(widgets->view_classes->create_class_sanction);
    fillUserComboList(widgets->view_classes->create_class_user);

    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_classes->create_class_sanction), "0");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_classes->create_class_user), "1");

    gtk_entry_set_text(widgets->view_classes->create_class_name, "");
    gtk_entry_set_text(widgets->view_classes->create_class_major, "");
    gtk_entry_set_text(GTK_ENTRY(widgets->view_classes->create_class_year), "2020");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->view_classes->create_class_apprenticeship), FALSE);

}

void GTKCreateClassSubmit() {

    int returnCode = GTKCreateClassSubmitCheckRequiredField() ||
                     insertClass(gtk_entry_get_text(widgets->view_classes->create_class_name),
                                 gtk_entry_get_text(widgets->view_classes->create_class_major),
                                 atoi(gtk_entry_get_text(GTK_ENTRY(widgets->view_classes->create_class_year))),
                                 gtk_toggle_button_get_active(
                                         GTK_TOGGLE_BUTTON(widgets->view_classes->create_class_apprenticeship)) ? 1 : 0,
                                 atoi(gtk_combo_box_get_active_id(
                                         GTK_COMBO_BOX(widgets->view_classes->create_class_user))),
                                 atoi(gtk_combo_box_get_active_id(
                                         GTK_COMBO_BOX(widgets->view_classes->create_class_sanction))));

    if (returnCode)
        fprintf(stderr, "Error, could not create class\n");
    else {
        printf("Class create successful\n");
        GTKListClasses();
    }
}

int GTKCreateClassSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_classes->create_class_name)) == 0) {
        fprintf(stderr, "Name empty !\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(widgets->view_classes->create_class_year))) != 4) {
        fprintf(stderr, "Wrong year !\n");
        returnCode = 1;
    }
    return returnCode;
}

void GTKListSanctions() {
    gtk_stack_set_visible_child(widgets->view_sanctions->view_sanctions_stack,
                                widgets->view_sanctions->view_sanctions_fixed);
    char *sanctions, *result, *firstAddress;
    int nbSanctions = 0, i;
    listSanctions(&sanctions);
    firstAddress = sanctions;
    result = sanctions;

    while ((result = strstr(result, ";\n"))) {
        nbSanctions++;
        result++;
    }

    GtkTreeIter iter;
    gtk_tree_store_clear(widgets->view_sanctions->sanctions_tree_store);

    for (i = 0; i < nbSanctions; ++i) {
        gtk_tree_store_append(widgets->view_sanctions->sanctions_tree_store, &iter, NULL);

        //ID
        result = strchr(sanctions, '|');
        size_t columnSize = result - sanctions;
        char *buffer = malloc(columnSize + 1);

        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 0, atoi(buffer), -1);
        sanctions += columnSize + 1;

        //NAME
        free(buffer);
        result = strchr(sanctions, '|');
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 1, buffer, -1);
        sanctions += columnSize + 1;

        //DESCRIPTION
        free(buffer);
        result = strchr(sanctions, '|');
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 2, buffer, -1);
        sanctions += columnSize + 1;

        //USER
        free(buffer);
        result = strchr(sanctions, '|');
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 3, buffer, -1);
        sanctions += columnSize + 1;

        //USER_FK
        free(buffer);
        result = strstr(sanctions, ";\n");
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 4, atoi(buffer), -1);
        sanctions += columnSize + 2;
    }
    free(firstAddress);
}

void GTKEditSanction(int id) {
    gtk_stack_set_visible_child(widgets->view_sanctions->view_sanctions_stack,
                                widgets->view_sanctions->edit_sanction_fixed);
    char *name, *description, *user, *user_fk, idBuffer[6];
    itoa(id, idBuffer, 10);
    GTKSanctionGetData(id, &name, &description, &user, &user_fk);
    fillUserComboList(widgets->view_sanctions->edit_sanction_user);

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->edit_sanction_description);
    gtk_text_buffer_set_text(textBuffer, description, strlen(description));
    gtk_entry_set_text(widgets->view_sanctions->edit_sanction_name, name);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_sanctions->edit_sanction_user), user_fk);
    gtk_label_set_text(widgets->view_sanctions->edit_sanction_id, idBuffer);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_sanctions->edit_sanction_id), FALSE);

    free(name);
    free(description);
    free(user);
    free(user_fk);
}

void GTKEditSanctionSubmit() {

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->edit_sanction_description);
    GtkTextIter startIter;
    GtkTextIter endIter;
    gtk_text_buffer_get_start_iter(textBuffer, &startIter);
    gtk_text_buffer_get_end_iter(textBuffer, &endIter);

    int returnCode = GTKEditSanctionSubmitCheckRequiredField(
            gtk_text_buffer_get_text(textBuffer, &startIter, &endIter, FALSE)) ||
                     updateSanction(atoi(gtk_label_get_text(widgets->view_sanctions->edit_sanction_id)),
                                    gtk_entry_get_text(widgets->view_sanctions->edit_sanction_name),
                                    gtk_text_buffer_get_text(textBuffer, &startIter, &endIter, FALSE),
                                    atoi(gtk_combo_box_get_active_id(
                                            GTK_COMBO_BOX(widgets->view_sanctions->edit_sanction_user))));

    if (returnCode)
        fprintf(stderr, "Error, could not edit sanction\n");
    else {
        printf("Sanction edit successful\n");
        GTKListSanctions();
    }
}

int GTKEditSanctionSubmitCheckRequiredField(char *textIter) {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_sanctions->edit_sanction_name)) == 0) {
        fprintf(stderr, "Name empty !\n");
        returnCode = 1;
    }
    if (strlen(textIter) == 0) {
        fprintf(stderr, "Description empty !\n");
        returnCode = 1;
    }
    return returnCode;
}


void GTKSanctionGetData(int id, char **name, char **description, char **user, char **user_fk) {
    char *sanctionData, *firstAdress;
    size_t columnSize;
    getSanction(&sanctionData, id);
    firstAdress = sanctionData;

    //ID
    sanctionData += strchr(sanctionData, '|') - sanctionData + 1;

    //NAME
    columnSize = strchr(sanctionData, '|') - sanctionData;
    *name = malloc(columnSize + 1);
    strncpy(*name, sanctionData, columnSize);
    (*name)[columnSize] = '\0';
    sanctionData += columnSize + 1;

    //DESCRIPTION
    columnSize = strchr(sanctionData, '|') - sanctionData;
    *description = malloc(columnSize + 1);
    strncpy(*description, sanctionData, columnSize);
    (*description)[columnSize] = '\0';
    sanctionData += columnSize + 1;

    //USER
    columnSize = strchr(sanctionData, '|') - sanctionData;
    *user = malloc(columnSize + 1);
    strncpy(*user, sanctionData, columnSize);
    (*user)[columnSize] = '\0';
    sanctionData += columnSize + 1;

    //USER_FK
    columnSize = strstr(sanctionData, ";\n") - sanctionData;
    *user_fk = malloc(columnSize + 1);
    strncpy(*user_fk, sanctionData, columnSize);
    (*user_fk)[columnSize] = '\0';

    free(firstAdress);
}

void GTKSanctionGetDataStudentId(int student_id, char **name, char **description, char **student, char **student_fk) {
    char *sanctionData, *firstAdress;
    size_t columnSize;
    getSanctionStudentId(&sanctionData, student_id);
    firstAdress = sanctionData;

    //ID
    sanctionData += strchr(sanctionData, '|') - sanctionData + 1;

    //NAME
    columnSize = strchr(sanctionData, '|') - sanctionData;
    *name = malloc(columnSize + 1);
    strncpy(*name, sanctionData, columnSize);
    (*name)[columnSize] = '\0';
    sanctionData += columnSize + 1;

    //DESCRIPTION
    columnSize = strchr(sanctionData, '|') - sanctionData;
    *description = malloc(columnSize + 1);
    strncpy(*description, sanctionData, columnSize);
    (*description)[columnSize] = '\0';
    sanctionData += columnSize + 1;

    //STUDENT
    columnSize = strchr(sanctionData, '|') - sanctionData;
    *student = malloc(columnSize + 1);
    strncpy(*student, sanctionData, columnSize);
    (*student)[columnSize] = '\0';
    sanctionData += columnSize + 1;

    //STUDENT_FK
    columnSize = strstr(sanctionData, ";\n") - sanctionData;
    *student_fk = malloc(columnSize + 1);
    strncpy(*student_fk, sanctionData, columnSize);
    (*student_fk)[columnSize] = '\0';

    free(firstAdress);

}

void GTKCreateSanction() {
    gtk_stack_set_visible_child(widgets->view_sanctions->view_sanctions_stack,
                                widgets->view_sanctions->create_sanction_fixed);

    fillUserComboList(widgets->view_sanctions->create_sanction_user);

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->create_sanction_description);
    gtk_text_buffer_set_text(textBuffer, "", 0);
    gtk_entry_set_text(widgets->view_sanctions->create_sanction_name, "");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_sanctions->create_sanction_user), "1");
}

void GTKCreateSanctionSubmit() {

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->create_sanction_description);
    GtkTextIter startIter;
    GtkTextIter endIter;
    gtk_text_buffer_get_start_iter(textBuffer, &startIter);
    gtk_text_buffer_get_end_iter(textBuffer, &endIter);

    int returnCode = insertSanction(gtk_text_buffer_get_text(textBuffer, &startIter, &endIter, FALSE),
                                    gtk_entry_get_text(widgets->view_sanctions->create_sanction_name),
                                    atoi(gtk_combo_box_get_active_id(
                                            GTK_COMBO_BOX(widgets->view_sanctions->create_sanction_user))));

    if (returnCode)
        fprintf(stderr, "Error, could not create sanction\n");
    else {
        printf("Sanction create successful\n");
        GTKListSanctions();
    }
}

void GTKListDeliverables() {
    gtk_stack_set_visible_child(widgets->view_deliverables->view_deliverables_stack,
                                widgets->view_deliverables->view_deliverables_fixed);
    char *deliverables, *result, *firstAddress;
    int nbdeliverables = 0;
    listDeliverables(&deliverables);
    firstAddress = deliverables;
    result = deliverables;

    while ((result = strstr(result, ";\n"))) {
        nbdeliverables++;
        result++;
    }

    GtkTreeIter iter;
    gtk_tree_store_clear(widgets->view_deliverables->deliverables_tree_store);

    for (int i = 0; i < nbdeliverables; ++i) {
        gtk_tree_store_append(widgets->view_deliverables->deliverables_tree_store, &iter, NULL);

        //ID
        result = strchr(deliverables, '|');
        size_t columnSize = result - deliverables;
        char *buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 0, atoi(buffer), -1);
        deliverables += columnSize + 1;

        //DUE_DATE
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 1, buffer, -1);
        deliverables += columnSize + 1;

        //SUBJECT
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 2, buffer, -1);
        deliverables += columnSize + 1;

        //AUDIO_RECORD
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        if (strlen(buffer) > 0) {
            buffer = realloc(buffer, 4 * sizeof(char));
            strcpy(buffer, "Yes");
        } else {
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "No");
        }

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 3, buffer, -1);
        deliverables += columnSize + 1;

        //VIDEO_RECORD
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        if (strlen(buffer) > 0) {
            buffer = realloc(buffer, 4 * sizeof(char));
            strcpy(buffer, "Yes");
        } else {
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "No");
        }

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 4, buffer, -1);
        deliverables += columnSize + 1;

        //BAD_CODE
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        if (strlen(buffer) > 0) {
            buffer = realloc(buffer, 4 * sizeof(char));
            strcpy(buffer, "Yes");
        } else {
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "No");
        }

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 5, buffer, -1);
        deliverables += columnSize + 1;

        //DELIVERABLE_FILE
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        if (strlen(buffer) > 0) {
            buffer = realloc(buffer, 4 * sizeof(char));
            strcpy(buffer, "Yes");
        } else {
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "No");
        }

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 6, buffer, -1);
        deliverables += columnSize + 1;

        //STATUS
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 7, buffer, -1);
        deliverables += columnSize + 1;

        //STUDENT
        free(buffer);
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 8, buffer, -1);
        deliverables += columnSize + 1;

        //STUDENT_FK
        free(buffer);
        result = strstr(deliverables, ";\n");
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 9, atoi(buffer), -1);
        deliverables += columnSize + 2;
    }

    free(firstAddress);
}

void GTKEditDelivreables(int id) {
    gtk_stack_set_visible_child(widgets->view_deliverables->view_deliverables_stack,
                                widgets->view_deliverables->edit_deliverable_fixed);
    char *due_date, *subject, *audio_record, *video_record, *bad_code, *deliverable_file, *status, *student, *student_fk, *sanction_name, *sanction_description, idBuffer[6];
    itoa(id, idBuffer, 10);
    GTKDelivreablesGetData(id, &due_date, &subject, &audio_record, &video_record, &bad_code, &deliverable_file, &status,
                           &student, &student_fk, &sanction_name, &sanction_description);


    GTKEditDeliverableSetDueDate(due_date);
    fillStatusComboList(widgets->view_deliverables->edit_deliverable_status, status);

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(
            widgets->view_deliverables->edit_deliverable_sanction_description);
    gtk_text_buffer_set_text(textBuffer, sanction_description, strlen(sanction_description));

    gtk_label_set_text(widgets->view_deliverables->edit_deliverable_id, idBuffer);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_id), FALSE);
    gtk_label_set_text(widgets->view_deliverables->edit_deliverable_student_fk, student_fk);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_student_fk), FALSE);
    gtk_label_set_text(widgets->view_deliverables->edit_deliverable_sanction_name, sanction_name);
    gtk_label_set_text(widgets->view_deliverables->edit_deliverable_student_name, student);
    gtk_entry_set_text(widgets->view_deliverables->edit_deliverable_subject, subject);
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_video));
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_audio));
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_deliverable_file));
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_bad_code));


    if (audio_record != NULL && !strlen(audio_record)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), TRUE);
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download),
                                    audio_record);
    }
    if (video_record != NULL && !strlen(video_record)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), TRUE);
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download),
                                    video_record);

    }
    if (deliverable_file != NULL && !strlen(deliverable_file)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                               FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                               TRUE);
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                                    deliverable_file);

    }
    if (bad_code != NULL && !strlen(bad_code)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download), FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download), TRUE);
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download),
                                    bad_code);

    }


    free(due_date);
    free(subject);
    free(audio_record);
    free(video_record);
    free(bad_code);
    free(deliverable_file);
    free(status);
    free(student);
    free(student_fk);
    free(sanction_name);
    free(sanction_description);
}

void GTKDelivreablesGetData(int id, char **due_date, char **subject, char **audio_record, char **video_record,
                            char **bad_code, char **deliverable_file, char **status, char **student, char **student_fk,
                            char **sanction_name, char **sanction_description) {
    char *deliverableData, *firstAdress;
    size_t columnSize;
    getDeliverable(&deliverableData, id);
    firstAdress = deliverableData;

    //ID
    deliverableData += strchr(deliverableData, '|') - deliverableData + 1;

    //DUE_DATE
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *due_date = malloc(columnSize + 1);
    strncpy(*due_date, deliverableData, columnSize);
    (*due_date)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //SUBJECT
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *subject = malloc(columnSize + 1);
    strncpy(*subject, deliverableData, columnSize);
    (*subject)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //AUDIO_RECORD
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *audio_record = malloc(columnSize + 1);
    strncpy(*audio_record, deliverableData, columnSize);
    (*audio_record)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //VIDEO_RECORD
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *video_record = malloc(columnSize + 1);
    strncpy(*video_record, deliverableData, columnSize);
    (*video_record)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //BAD_CODE
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *bad_code = malloc(columnSize + 1);
    strncpy(*bad_code, deliverableData, columnSize);
    (*bad_code)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //DELIVERABLE_FILE
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *deliverable_file = malloc(columnSize + 1);
    strncpy(*deliverable_file, deliverableData, columnSize);
    (*deliverable_file)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //STATUS
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *status = malloc(columnSize + 1);
    strncpy(*status, deliverableData, columnSize);
    (*status)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //STUDENT
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *student = malloc(columnSize + 1);
    strncpy(*student, deliverableData, columnSize);
    (*student)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //STUDENT_FK
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *student_fk = malloc(columnSize + 1);
    strncpy(*student_fk, deliverableData, columnSize);
    (*student_fk)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //SANCTION_NAME
    columnSize = strchr(deliverableData, '|') - deliverableData;
    *sanction_name = malloc(columnSize + 1);
    strncpy(*sanction_name, deliverableData, columnSize);
    (*sanction_name)[columnSize] = '\0';
    deliverableData += columnSize + 1;

    //SANCTION_DESCRIPTION
    columnSize = strstr(deliverableData, ";\n") - deliverableData;
    *sanction_description = malloc(columnSize + 1);
    strncpy(*sanction_description, deliverableData, columnSize);
    (*sanction_description)[columnSize] = '\0';

    free(firstAdress);
}

void GTKEditDeliverableSetDueDate(char *date) {
    // date = YYYY/MM/DD
    guint day, month, year, columnSize;
    char *buffer;
    //YEAR
    columnSize = strchr(date, '/') - date;
    buffer = malloc(columnSize + 1);
    strncpy(buffer, date, columnSize);
    buffer[columnSize] = '\0';
    year = atoi(buffer);
    date += columnSize + 1;
    free(buffer);

    //MONTH
    columnSize = strchr(date, '/') - date;
    buffer = malloc(columnSize + 1);
    strncpy(buffer, date, columnSize);
    buffer[columnSize] = '\0';
    month = atoi(buffer);
    date += columnSize + 1;
    free(buffer);

    //DAY
    columnSize = strchr(date, '\0') - date;
    buffer = malloc(columnSize + 1);
    strncpy(buffer, date, columnSize);
    buffer[columnSize] = '\0';
    day = atoi(buffer);
    free(buffer);

    //a month number between 0 and 11.
    gtk_calendar_select_month(widgets->view_deliverables->edit_deliverable_due_date, month - 1, year);
    gtk_calendar_select_day(widgets->view_deliverables->edit_deliverable_due_date, day);
}

void GTKEditDelivreablesSubmit() {
    guint day, month, year;
    char dateBuffer[9];
    gtk_calendar_get_date(widgets->view_deliverables->edit_deliverable_due_date, &year, &month, &day);
    sprintf(dateBuffer, "%d/%d/%d", year, month + 1, day);
    //year decimal number (e.g. 2011), or NULL.
    //month number (between 0 and 11), or NULL.
    //day number (between 1 and 31), or NULL.

    int returnCode = updateDeliverable(atoi(gtk_label_get_text(widgets->view_deliverables->edit_deliverable_id)),
                                       dateBuffer,
                                       gtk_entry_get_text(widgets->view_deliverables->edit_deliverable_subject),
                                       gtk_combo_box_text_get_active_text(
                                               widgets->view_deliverables->edit_deliverable_status),
                                       atoi(gtk_label_get_text(
                                               widgets->view_deliverables->edit_deliverable_student_fk)));

    if (returnCode)
        fprintf(stderr, "Error, could not update deliverable\n");
    else {
        printf("Deliverable update successful\n");
        GTKListDeliverables();
    }
}

int GTKDeliverableSetAudio(char *path) {
    if (!checkAudioExtension(path)) {
        fprintf(stderr, "Wrong audio file extension !\n");
        return 1;
    }

    char *newPath;

    if (!strlen(newPath = insertDeliverableFile("audio_record",
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_id)),
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_student_fk)),
                                                path))) {
        fprintf(stderr, "Error while adding %s audio record", path);
        return 1;
    }

    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), newPath);
    free(newPath);
    return 0;
}

int GTKDeliverableSetVideo(char *path) {
    if (!checkVideoExtension(path)) {
        fprintf(stderr, "Wrong video file extension !\n");
        return 1;
    }

    char *newPath;

    if (!strlen(newPath = insertDeliverableFile("video_record",
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_id)),
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_student_fk)),
                                                path))) {
        fprintf(stderr, "Error while adding %s video record", path);
        return 1;
    }
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), newPath);
    free(newPath);
    return 0;
}

int GTKDeliverableSetBadCode(char *path) {
    char *newPath;

    if (!strlen(newPath = insertDeliverableFile("bad_code",
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_id)),
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_student_fk)),
                                                path))) {
        fprintf(stderr, "Error while adding %s bad code", path);
        return 1;
    }
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download), newPath);
    free(newPath);
    return 0;
}

int GTKDeliverableSetDeliverable(char *path) {
    char *newPath;

    if (!strlen(newPath = insertDeliverableFile("deliverable_file",
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_id)),
                                                atoi(gtk_label_get_text(
                                                        widgets->view_deliverables->edit_deliverable_student_fk)),
                                                path))) {
        fprintf(stderr, "Error while adding %s audio record", path);
        return 1;
    }
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download), TRUE);
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                                newPath);
    free(newPath);
    return 0;
}

void GTKCreateDelivreables(int student_fk) {
    gtk_stack_set_visible_child_name(widgets->menu_stack, "view_deliverables");
    gtk_stack_set_visible_child(widgets->view_deliverables->view_deliverables_stack,
                                widgets->view_deliverables->create_deliverable_fixed);

    char *sanction_name, *sanction_description, *student, *student_fk_c;
    GTKSanctionGetDataStudentId(student_fk, &sanction_name, &sanction_description, &student, &student_fk_c);
    fillStatusComboList(widgets->view_deliverables->create_deliverable_status, "To do");
    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(
            widgets->view_deliverables->create_deliverable_sanction_description);
    gtk_text_buffer_set_text(textBuffer, sanction_description, strlen(sanction_description));

    gtk_label_set_text(widgets->view_deliverables->create_deliverable_student_fk, student_fk_c);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->create_deliverable_student_fk), FALSE);
    gtk_label_set_text(widgets->view_deliverables->create_deliverable_sanction_name, sanction_name);
    gtk_label_set_text(widgets->view_deliverables->create_deliverable_student_name, student);
    gtk_entry_set_text(widgets->view_deliverables->edit_deliverable_subject, "");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_video));
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_audio));
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_deliverable_file));
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_bad_code));

    free(sanction_name);
    free(sanction_description);
    free(student);
    free(student_fk_c);
}

void GTKCreateDelivreablesSubmit() {
    guint day, month, year;
    char dateBuffer[9];
    gtk_calendar_get_date(widgets->view_deliverables->create_deliverable_due_date, &year, &month, &day);
    sprintf(dateBuffer, "%d/%d/%d", year, month + 1, day);

    char *audioPath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_audio));
    char *videoPath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_video));
    char *bcPath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_bad_code));
    char *deliverablePath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_deliverable_file));
    int audioGood = checkAudioExtension(audioPath);
    int videoGood = checkVideoExtension(videoPath);

    int returnCode = insertDeliverable(dateBuffer,
                                       gtk_entry_get_text(widgets->view_deliverables->create_deliverable_subject),
                                       audioGood ? audioPath : NULL,
                                       videoGood ? videoPath : NULL,
                                       bcPath,
                                       deliverablePath,
                                       gtk_combo_box_text_get_active_text(
                                               widgets->view_deliverables->create_deliverable_status),
                                       atoi(gtk_label_get_text(
                                               widgets->view_deliverables->create_deliverable_student_fk)));

    if (returnCode)
        fprintf(stderr, "Error, could not update deliverable\n");
    else {
        printf("Deliverable update successful\n");
        GTKListDeliverables();
    }
}

void GTKViewUser() {
    gtk_stack_set_visible_child(widgets->view_user->view_user_stack, widgets->view_user->view_user_fixed);
    char *email, *first_name, *last_name, *photo, *birthdate, *emailURI;
    int id;
    GTKUserGetData(&id, &email, &first_name, &last_name, &photo, &birthdate);
    emailURI = malloc(strlen(email) + 8);
    strcat(strcpy(emailURI, "mailto:"), email);

    GTKUserImage(photo);

    gtk_label_set_text(widgets->view_user->view_user_first_name, first_name);
    gtk_label_set_text(widgets->view_user->view_user_last_name, last_name);
    gtk_label_set_text(widgets->view_user->view_user_birthdate, birthdate);
    gtk_button_set_label(GTK_BUTTON(widgets->view_user->view_user_email), email);
    gtk_link_button_set_uri(widgets->view_user->view_user_email, emailURI);

    free(email);
    free(emailURI);
    free(first_name);
    free(last_name);
    free(photo);
    free(birthdate);
}

void GTKEditUser() {
    gtk_stack_set_visible_child(widgets->view_user->view_user_stack, widgets->view_user->edit_user_fixed);
    char *email, *first_name, *last_name, *photo, *birthdate;
    int id;
    GTKUserGetData(&id, &email, &first_name, &last_name, &photo, &birthdate);

    gtk_entry_set_text(widgets->view_user->edit_user_first_name, first_name);
    gtk_entry_set_text(widgets->view_user->edit_user_last_name, last_name);
    gtk_entry_set_text(widgets->view_user->edit_user_email, email);
    gtk_label_set_text(widgets->view_user->edit_user_birthdate, birthdate);

    free(email);
    free(first_name);
    free(last_name);
    free(photo);
    free(birthdate);
}

void GTKEditUserSubmit() {
    int returnCode = updateUser(1,
                                gtk_entry_get_text(widgets->view_user->edit_user_email),
                                gtk_entry_get_text(widgets->view_user->edit_user_first_name),
                                gtk_entry_get_text(widgets->view_user->edit_user_last_name),
                                gtk_label_get_text(widgets->view_user->edit_user_birthdate));
    if (returnCode)
        fprintf(stderr, "Error, could not update user\n");
    else {
        printf("User update successful\n");
        GTKViewUser();
    }
}

void GTKUserGetData(int *id, char **email, char **first_name, char **last_name, char **photo, char **birthdate) {
    char *intBuffer, *data, *firstAdress;
    size_t columnSize;
    getUser(&data, 1);
    firstAdress = data;

    columnSize = strchr(data, '|') - data;
    intBuffer = malloc(columnSize + 1);
    strncpy(intBuffer, data, columnSize);
    intBuffer[columnSize] = '\0';
    *id = atoi(intBuffer);
    data += columnSize + 1;

    columnSize = strchr(data, '|') - data;
    *email = malloc(columnSize + 1);
    strncpy(*email, data, columnSize);
    (*email)[columnSize] = '\0';
    data += columnSize + 1;

    columnSize = strchr(data, '|') - data;
    *first_name = malloc(columnSize + 1);
    strncpy(*first_name, data, columnSize);
    (*first_name)[columnSize] = '\0';
    data += columnSize + 1;

    columnSize = strchr(data, '|') - data;
    *last_name = malloc(columnSize + 1);
    strncpy(*last_name, data, columnSize);
    (*last_name)[columnSize] = '\0';
    data += columnSize + 1;

    columnSize = strchr(data, '|') - data;
    *photo = malloc(columnSize + 1);
    strncpy(*photo, data, columnSize);
    (*photo)[columnSize] = '\0';
    data += columnSize + 1;

    columnSize = strstr(data, ";\n") - data;
    *birthdate = malloc(columnSize + 1);
    strncpy(*birthdate, data, columnSize);
    (*birthdate)[columnSize] = '\0';
    data += columnSize + 1;

    free(intBuffer);
    free(firstAdress);
}

void GTKUserImage(char *path) {
    gtk_image_clear(widgets->view_user->view_user_image);

    GdkPixbuf *pixbuf;
    if ((pixbuf = gdk_pixbuf_new_from_file(path, NULL)) == NULL) {
        fprintf(stderr, "Error while loading user profil picture\n");
    } else {
        printf("loaded!\n");
        int width = gdk_pixbuf_get_width(pixbuf);
        int height = gdk_pixbuf_get_height(pixbuf);
        double ratio = (250. / width);
        printf("width: %d, height: %d, ratio: %lf\n", width, height, ratio);
        gtk_image_set_from_pixbuf(widgets->view_user->view_user_image,
                                  gdk_pixbuf_scale_simple(pixbuf, floor(width * ratio), floor(height * ratio),
                                                          GDK_INTERP_BILINEAR));
    }
}

int GTKUserSetImage(char *path) {
    if (!checkImageExtension(path))
        return 1;

    return insertTableImage("user", 1, path);
}

void GTKViewSettings() {
    gtk_switch_set_active(widgets->view_settings->view_settings_switch_theme_button, darkTheme ? TRUE : FALSE);
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser));
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser),
                                        storageFolder);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser), dbname);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser), gladeFile);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser),
                                  defaultThemePath);
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser),
                                  darkThemePath);
}

void GTKViewSettingsSubmit() {
    storageFolder = realloc(storageFolder,
                            strlen(gtk_file_chooser_get_filename(
                                    GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser)) + 1));
    strcpy(storageFolder,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser)));

    dbname = realloc(dbname,
                     strlen(gtk_file_chooser_get_filename(
                             GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser)) + 1));
    strcpy(dbname,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser)));

    gladeFile = realloc(gladeFile,
                        strlen(gtk_file_chooser_get_filename(
                                GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser))) + 1);
    strcpy(gladeFile,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser)));

    defaultThemePath = realloc(defaultThemePath,
                               strlen(gtk_file_chooser_get_filename(
                                       GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser))) +
                               1);
    strcpy(defaultThemePath, gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser)));

    darkThemePath = realloc(darkThemePath,
                            strlen(gtk_file_chooser_get_filename(
                                    GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser)) + 1));
    strcpy(darkThemePath,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser)));
    writeConf();
}

void GTKSetTheme() {
    GtkCssProvider *pCssProvider = NULL;
    pCssProvider = gtk_css_provider_new();
    GError *error = NULL;

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(pCssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(pCssProvider);

    if (darkTheme) {
        if (!gtk_css_provider_load_from_path(pCssProvider, darkThemePath, &error)) {
            fprintf(stderr, "%s\n", error->message);

            exit(error->code);
        }
    } else {
        if (!gtk_css_provider_load_from_path(pCssProvider, defaultThemePath, &error)) {
            fprintf(stderr, "%s\n", error->message);

            exit(error->code);
        }
    }
}

void connectWidgets() {

    widgets->window_dashboard = GTK_WIDGET(gtk_builder_get_object(builder, "window_dashboard"));
    widgets->menu_stack = GTK_STACK(gtk_builder_get_object(builder, "menu_stack"));
    widgets->gtk_fixed = GTK_FIXED(gtk_builder_get_object(builder, "gtk_fixed"));
    widgets->menu_stack_switcher = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "menu_stack_switcher"));
    widgets->search_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "search_entry"));

    //Connect view_students
    widgets->view_students = g_slice_new(Students);
    widgets->view_students->view_students_stack = GTK_STACK(gtk_builder_get_object(builder, "view_students_stack"));
    widgets->view_students->view_students_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_students"));
    widgets->view_students->edit_student_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "edit_student"));
    widgets->view_students->create_student_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "create_student"));
    widgets->view_students->view_students_view = GTK_WIDGET(gtk_builder_get_object(builder, "students_view"));
    widgets->view_students->students_view_delete_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "students_view_delete_button"));
    widgets->view_students->students_view_create_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "students_view_create_button"));
    widgets->view_students->students_view_refresh_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "students_view_refresh_button"));
    widgets->view_students->students_view_add_bottle_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "students_view_add_bottle_bcreaten"));
    widgets->view_students->students_view_remove_bottle_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "students_view_remove_bottlcreatetton"));
    widgets->view_students->student_edit_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "student_edit_submit_button"));
    widgets->view_students->student_edit_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "student_edit_return_button"));
    widgets->view_students->student_create_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "student_create_submit_button"));
    widgets->view_students->student_create_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "student_create_return_button"));
    widgets->view_students->edit_student_id = GTK_LABEL(gtk_builder_get_object(builder, "edit_student_id"));
    widgets->view_students->edit_student_first_name = GTK_ENTRY(
            gtk_builder_get_object(builder, "edit_student_first_name"));
    widgets->view_students->edit_student_last_name = GTK_ENTRY(
            gtk_builder_get_object(builder, "edit_student_last_name"));
    widgets->view_students->edit_student_email = GTK_ENTRY(gtk_builder_get_object(builder, "edit_student_email"));
    widgets->view_students->edit_student_bottles = GTK_SPIN_BUTTON(
            gtk_builder_get_object(builder, "edit_student_bottles"));
    widgets->view_students->edit_student_class = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "edit_student_class"));
    widgets->view_students->edit_student_image = GTK_IMAGE(gtk_builder_get_object(builder, "edit_student_image"));
    widgets->view_students->edit_student_image_file_picker = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "edit_student_image_file_picker"));
    widgets->view_students->create_student_image_clear_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "create_student_image_clear_button"));
    widgets->view_students->create_student_first_name = GTK_ENTRY(
            gtk_builder_get_object(builder, "create_student_first_name"));
    widgets->view_students->create_student_last_name = GTK_ENTRY(
            gtk_builder_get_object(builder, "create_student_last_name"));
    widgets->view_students->create_student_email = GTK_ENTRY(gtk_builder_get_object(builder, "create_student_email"));
    widgets->view_students->create_student_class = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "create_student_class"));
    widgets->view_students->create_student_image_file_picker = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "create_student_image_file_picker"));
    widgets->view_students->students_tree_store = GTK_TREE_STORE(
            gtk_builder_get_object(builder, "students_tree_store"));
    widgets->view_students->students_tree_view = GTK_TREE_VIEW(gtk_builder_get_object(builder, "students_tree_view"));
    widgets->view_students->students_tree_selection = GTK_TREE_SELECTION(
            gtk_builder_get_object(builder, "students_tree_selection"));
    widgets->view_students->students_cx_1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_1"));
    widgets->view_students->students_cx_2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_2"));
    widgets->view_students->students_cx_3 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_3"));
    widgets->view_students->students_cx_4 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_4"));
    widgets->view_students->students_cx_5 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_5"));
    widgets->view_students->students_cx_6 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_6"));
    widgets->view_students->students_cx_7 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_7"));
    widgets->view_students->students_cx_8 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_8"));
    widgets->view_students->students_cx_9 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "students_cx_9"));
    widgets->view_students->students_cr_1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_1"));
    widgets->view_students->students_cr_2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_2"));
    widgets->view_students->students_cr_3 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_3"));
    widgets->view_students->students_cr_4 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_4"));
    widgets->view_students->students_cr_5 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_5"));
    widgets->view_students->students_cr_6 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_6"));
    widgets->view_students->students_cr_7 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_7"));
    widgets->view_students->students_cr_8 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_8"));
    widgets->view_students->students_cr_9 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "students_cr_9"));

    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_1, widgets->view_students->students_cr_1,
                                       "text", 0);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_2, widgets->view_students->students_cr_2,
                                       "text", 1);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_3, widgets->view_students->students_cr_3,
                                       "text", 2);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_4, widgets->view_students->students_cr_4,
                                       "pixbuf", 3);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_5, widgets->view_students->students_cr_5,
                                       "text", 4);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_6, widgets->view_students->students_cr_6,
                                       "text", 5);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_7, widgets->view_students->students_cr_7,
                                       "text", 6);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_8, widgets->view_students->students_cr_8,
                                       "text", 7);
    gtk_tree_view_column_add_attribute(widgets->view_students->students_cx_9, widgets->view_students->students_cr_9,
                                       "text", 8);

    //Connect view_classes
    widgets->view_classes = g_slice_new(Classes);
    widgets->view_classes->view_classes_stack = GTK_STACK(gtk_builder_get_object(builder, "view_classes_stack"));
    widgets->view_classes->view_classes_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_classes"));
    widgets->view_classes->edit_class_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "edit_class"));
    widgets->view_classes->create_class_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "create_class"));
    widgets->view_classes->view_classes_view = GTK_WIDGET(gtk_builder_get_object(builder, "classes_view"));
    widgets->view_classes->classes_view_delete_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "classes_view_delete_button"));
    widgets->view_classes->classes_view_create_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "classes_view_create_button"));
    widgets->view_classes->classes_view_refresh_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "classes_view_refresh_button"));
    widgets->view_classes->class_edit_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "class_edit_return_button"));
    widgets->view_classes->class_edit_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "class_edit_submit_button"));
    widgets->view_classes->class_create_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "class_create_return_button"));
    widgets->view_classes->class_create_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "class_create_submit_button"));
    widgets->view_classes->edit_class_id = GTK_LABEL(gtk_builder_get_object(builder, "edit_class_id"));
    widgets->view_classes->edit_class_name = GTK_ENTRY(gtk_builder_get_object(builder, "edit_class_name"));
    widgets->view_classes->edit_class_major = GTK_ENTRY(gtk_builder_get_object(builder, "edit_class_major"));
    widgets->view_classes->edit_class_sanction = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "edit_class_sanction"));
    widgets->view_classes->edit_class_user = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "edit_class_user"));
    widgets->view_classes->edit_class_apprenticeship = GTK_CHECK_BUTTON(
            gtk_builder_get_object(builder, "edit_class_apprenticeship"));
    widgets->view_classes->edit_class_year = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "edit_class_year"));
    widgets->view_classes->create_class_name = GTK_ENTRY(gtk_builder_get_object(builder, "create_class_name"));
    widgets->view_classes->create_class_major = GTK_ENTRY(gtk_builder_get_object(builder, "create_class_major"));
    widgets->view_classes->create_class_sanction = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "create_class_sanction"));
    widgets->view_classes->create_class_user = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "create_class_user"));
    widgets->view_classes->create_class_apprenticeship = GTK_CHECK_BUTTON(
            gtk_builder_get_object(builder, "create_class_apprenticeship"));
    widgets->view_classes->create_class_year = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "create_class_year"));
    widgets->view_classes->classes_tree_store = GTK_TREE_STORE(
            gtk_builder_get_object(builder, "classes_tree_store"));
    widgets->view_classes->classes_tree_view = GTK_TREE_VIEW(gtk_builder_get_object(builder, "classes_tree_view"));
    widgets->view_classes->classes_tree_selection = GTK_TREE_SELECTION(
            gtk_builder_get_object(builder, "classes_tree_selection"));
    widgets->view_classes->classes_cx_1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_1"));
    widgets->view_classes->classes_cx_2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_2"));
    widgets->view_classes->classes_cx_3 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_3"));
    widgets->view_classes->classes_cx_4 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_4"));
    widgets->view_classes->classes_cx_5 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_5"));
    widgets->view_classes->classes_cx_6 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_6"));
    widgets->view_classes->classes_cx_7 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_7"));
    widgets->view_classes->classes_cx_8 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_8"));
    widgets->view_classes->classes_cx_9 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "classes_cx_9"));
    widgets->view_classes->classes_cr_1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_1"));
    widgets->view_classes->classes_cr_2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_2"));
    widgets->view_classes->classes_cr_3 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_3"));
    widgets->view_classes->classes_cr_4 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_4"));
    widgets->view_classes->classes_cr_5 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_5"));
    widgets->view_classes->classes_cr_6 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_6"));
    widgets->view_classes->classes_cr_7 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_7"));
    widgets->view_classes->classes_cr_8 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_8"));
    widgets->view_classes->classes_cr_9 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "classes_cr_9"));

    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_1, widgets->view_classes->classes_cr_1,
                                       "text", 0);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_2, widgets->view_classes->classes_cr_2,
                                       "text", 1);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_3, widgets->view_classes->classes_cr_3,
                                       "text", 2);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_4, widgets->view_classes->classes_cr_4,
                                       "text", 3);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_5, widgets->view_classes->classes_cr_5,
                                       "text", 4);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_6, widgets->view_classes->classes_cr_6,
                                       "text", 5);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_7, widgets->view_classes->classes_cr_7,
                                       "text", 6);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_8, widgets->view_classes->classes_cr_8,
                                       "text", 7);
    gtk_tree_view_column_add_attribute(widgets->view_classes->classes_cx_9, widgets->view_classes->classes_cr_9,
                                       "text", 8);

    //Connect view_sanctions
    widgets->view_sanctions = g_slice_new(Sanctions);
    widgets->view_sanctions->view_sanctions_stack = GTK_STACK(gtk_builder_get_object(builder, "view_sanctions_stack"));
    widgets->view_sanctions->view_sanctions_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_sanctions"));
    widgets->view_sanctions->edit_sanction_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "edit_sanction"));
    widgets->view_sanctions->create_sanction_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "create_sanction"));
    widgets->view_sanctions->view_sanctions_view = GTK_WIDGET(gtk_builder_get_object(builder, "sanctions_view"));
    widgets->view_sanctions->sanctions_view_delete_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanctions_view_delete_button"));
    widgets->view_sanctions->sanctions_view_create_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanctions_view_create_button"));
    widgets->view_sanctions->sanctions_view_refresh_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanctions_view_refresh_button"));
    widgets->view_sanctions->sanction_edit_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanction_edit_return_button"));
    widgets->view_sanctions->sanction_edit_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanction_edit_submit_button"));
    widgets->view_sanctions->sanction_create_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanction_create_return_button"));
    widgets->view_sanctions->sanction_create_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "sanction_create_submit_button"));
    widgets->view_sanctions->edit_sanction_name = GTK_ENTRY(gtk_builder_get_object(builder, "edit_sanction_name"));
    widgets->view_sanctions->edit_sanction_id = GTK_LABEL(gtk_builder_get_object(builder, "edit_sanction_id"));
    widgets->view_sanctions->edit_sanction_description = GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "edit_sanction_description"));
    widgets->view_sanctions->edit_sanction_user = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "edit_sanction_user"));
    widgets->view_sanctions->create_sanction_user = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "create_sanction_user"));
    widgets->view_sanctions->create_sanction_name = GTK_ENTRY(gtk_builder_get_object(builder, "create_sanction_name"));
    widgets->view_sanctions->create_sanction_description = GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "create_sanction_description"));
    widgets->view_sanctions->sanctions_tree_store = GTK_TREE_STORE(
            gtk_builder_get_object(builder, "sanctions_tree_store"));
    widgets->view_sanctions->sanctions_tree_view = GTK_TREE_VIEW(
            gtk_builder_get_object(builder, "sanctions_tree_view"));
    widgets->view_sanctions->sanctions_tree_selection = GTK_TREE_SELECTION(
            gtk_builder_get_object(builder, "sanctions_tree_selection"));
    widgets->view_sanctions->sanctions_cx_1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "sanctions_cx_1"));
    widgets->view_sanctions->sanctions_cx_2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "sanctions_cx_2"));
    widgets->view_sanctions->sanctions_cx_3 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "sanctions_cx_3"));
    widgets->view_sanctions->sanctions_cx_4 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "sanctions_cx_4"));
    widgets->view_sanctions->sanctions_cx_5 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "sanctions_cx_5"));
    widgets->view_sanctions->sanctions_cr_1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "sanctions_cr_1"));
    widgets->view_sanctions->sanctions_cr_2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "sanctions_cr_2"));
    widgets->view_sanctions->sanctions_cr_3 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "sanctions_cr_3"));
    widgets->view_sanctions->sanctions_cr_4 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "sanctions_cr_4"));
    widgets->view_sanctions->sanctions_cr_5 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "sanctions_cr_5"));

    gtk_tree_view_column_add_attribute(widgets->view_sanctions->sanctions_cx_1, widgets->view_sanctions->sanctions_cr_1,
                                       "text", 0);
    gtk_tree_view_column_add_attribute(widgets->view_sanctions->sanctions_cx_2, widgets->view_sanctions->sanctions_cr_2,
                                       "text", 1);
    gtk_tree_view_column_add_attribute(widgets->view_sanctions->sanctions_cx_3, widgets->view_sanctions->sanctions_cr_3,
                                       "text", 2);
    gtk_tree_view_column_add_attribute(widgets->view_sanctions->sanctions_cx_4, widgets->view_sanctions->sanctions_cr_4,
                                       "text", 3);
    gtk_tree_view_column_add_attribute(widgets->view_sanctions->sanctions_cx_5, widgets->view_sanctions->sanctions_cr_5,
                                       "text", 4);

    //Connect view_deliverables
    widgets->view_deliverables = g_slice_new(Deliverables);
    widgets->view_deliverables->view_deliverables_stack = GTK_STACK(
            gtk_builder_get_object(builder, "view_deliverables_stack"));
    widgets->view_deliverables->view_deliverables_fixed = GTK_WIDGET(
            gtk_builder_get_object(builder, "view_deliverables"));
    widgets->view_deliverables->edit_deliverable_fixed = GTK_WIDGET(
            gtk_builder_get_object(builder, "edit_deliverable"));
    widgets->view_deliverables->create_deliverable_fixed = GTK_WIDGET(
            gtk_builder_get_object(builder, "create_deliverable"));
    widgets->view_deliverables->view_deliverables_view = GTK_WIDGET(
            gtk_builder_get_object(builder, "deliverables_view"));
    widgets->view_deliverables->deliverables_view_delete_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverables_view_delete_button"));
    widgets->view_deliverables->deliverables_view_create_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverables_view_create_button"));
    widgets->view_deliverables->deliverables_view_refresh_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverables_view_refresh_button"));
    widgets->view_deliverables->deliverable_edit_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverable_edit_return_button"));
    widgets->view_deliverables->deliverable_edit_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverable_edit_submit_button"));
    widgets->view_deliverables->deliverable_create_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverable_create_return_button"));
    widgets->view_deliverables->deliverable_create_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "deliverable_create_submit_button"));
    widgets->view_deliverables->edit_deliverable_id = GTK_LABEL(gtk_builder_get_object(builder, "edit_deliverable_id"));
    widgets->view_deliverables->edit_deliverable_student_fk = GTK_LABEL(
            gtk_builder_get_object(builder, "edit_deliverable_student_fk"));
    widgets->view_deliverables->edit_deliverable_sanction_name = GTK_LABEL(
            gtk_builder_get_object(builder, "edit_deliverable_sanction_name"));
    widgets->view_deliverables->edit_deliverable_sanction_description = GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "edit_deliverable_sanction_description"));
    widgets->view_deliverables->edit_deliverable_subject = GTK_ENTRY(
            gtk_builder_get_object(builder, "edit_deliverable_subject"));
    widgets->view_deliverables->edit_deliverable_status = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "edit_deliverable_status"));
    widgets->view_deliverables->edit_deliverable_due_date = GTK_CALENDAR(
            gtk_builder_get_object(builder, "edit_deliverable_due_date"));
    widgets->view_deliverables->edit_deliverable_student_name = GTK_LABEL(
            gtk_builder_get_object(builder, "edit_deliverable_student_name"));
    widgets->view_deliverables->edit_deliverable_audio = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_audio"));
    widgets->view_deliverables->edit_deliverable_video = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_video"));
    widgets->view_deliverables->edit_deliverable_bad_code = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_bad_code"));
    widgets->view_deliverables->edit_deliverable_deliverable_file = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_deliverable_file"));
    widgets->view_deliverables->edit_deliverable_audio_download = GTK_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_audio_download"));
    widgets->view_deliverables->edit_deliverable_video_download = GTK_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_video_download"));
    widgets->view_deliverables->edit_deliverable_bad_code_download = GTK_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_bad_code_download"));
    widgets->view_deliverables->edit_deliverable_deliverable_file_download = GTK_BUTTON(
            gtk_builder_get_object(builder, "edit_deliverable_deliverable_file_download"));
    widgets->view_deliverables->create_deliverable_student_fk = GTK_LABEL(
            gtk_builder_get_object(builder, "create_deliverable_student_fk"));
    widgets->view_deliverables->create_deliverable_sanction_name = GTK_LABEL(
            gtk_builder_get_object(builder, "create_deliverable_sanction_name"));
    widgets->view_deliverables->create_deliverable_student_name = GTK_LABEL(
            gtk_builder_get_object(builder, "create_deliverable_student_name"));
    widgets->view_deliverables->create_deliverable_sanction_description = GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "create_deliverable_sanction_description"));
    widgets->view_deliverables->create_deliverable_subject = GTK_ENTRY(
            gtk_builder_get_object(builder, "create_deliverable_subject"));
    widgets->view_deliverables->create_deliverable_status = GTK_COMBO_BOX_TEXT(
            gtk_builder_get_object(builder, "create_deliverable_status"));
    widgets->view_deliverables->create_deliverable_due_date = GTK_CALENDAR(
            gtk_builder_get_object(builder, "create_deliverable_due_date"));
    widgets->view_deliverables->create_deliverable_audio = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_audio"));
    widgets->view_deliverables->create_deliverable_video = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_video"));
    widgets->view_deliverables->create_deliverable_bad_code = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_bad_code"));
    widgets->view_deliverables->create_deliverable_deliverable_file = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_deliverable_file"));
    widgets->view_deliverables->create_deliverable_audio_clear = GTK_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_audio_clear"));
    widgets->view_deliverables->create_deliverable_video_clear = GTK_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_video_clear"));
    widgets->view_deliverables->create_deliverable_bad_code_clear = GTK_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_bad_code_clear"));
    widgets->view_deliverables->create_deliverable_deliverable_file_clear = GTK_BUTTON(
            gtk_builder_get_object(builder, "create_deliverable_deliverable_file_clear"));
    widgets->view_deliverables->deliverables_tree_store = GTK_TREE_STORE(
            gtk_builder_get_object(builder, "deliverables_tree_store"));
    widgets->view_deliverables->deliverables_tree_view = GTK_TREE_VIEW(
            gtk_builder_get_object(builder, "deliverables_tree_view"));
    widgets->view_deliverables->deliverables_tree_selection = GTK_TREE_SELECTION(
            gtk_builder_get_object(builder, "deliverables_tree_selection"));
    widgets->view_deliverables->deliverables_cx_1 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_1"));
    widgets->view_deliverables->deliverables_cx_2 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_2"));
    widgets->view_deliverables->deliverables_cx_3 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_3"));
    widgets->view_deliverables->deliverables_cx_4 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_4"));
    widgets->view_deliverables->deliverables_cx_5 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_5"));
    widgets->view_deliverables->deliverables_cx_6 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_6"));
    widgets->view_deliverables->deliverables_cx_7 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_7"));
    widgets->view_deliverables->deliverables_cx_8 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_8"));
    widgets->view_deliverables->deliverables_cx_9 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_9"));
    widgets->view_deliverables->deliverables_cx_10 = GTK_TREE_VIEW_COLUMN(
            gtk_builder_get_object(builder, "deliverables_cx_10"));
    widgets->view_deliverables->deliverables_cr_1 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_1"));
    widgets->view_deliverables->deliverables_cr_2 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_2"));
    widgets->view_deliverables->deliverables_cr_3 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_3"));
    widgets->view_deliverables->deliverables_cr_4 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_4"));
    widgets->view_deliverables->deliverables_cr_5 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_5"));
    widgets->view_deliverables->deliverables_cr_6 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_6"));
    widgets->view_deliverables->deliverables_cr_7 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_7"));
    widgets->view_deliverables->deliverables_cr_8 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_8"));
    widgets->view_deliverables->deliverables_cr_9 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_9"));
    widgets->view_deliverables->deliverables_cr_10 = GTK_CELL_RENDERER(
            gtk_builder_get_object(builder, "deliverables_cr_10"));

    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_1,
                                       widgets->view_deliverables->deliverables_cr_1,
                                       "text", 0);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_2,
                                       widgets->view_deliverables->deliverables_cr_2,
                                       "text", 1);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_3,
                                       widgets->view_deliverables->deliverables_cr_3,
                                       "text", 2);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_4,
                                       widgets->view_deliverables->deliverables_cr_4,
                                       "text", 3);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_5,
                                       widgets->view_deliverables->deliverables_cr_5,
                                       "text", 4);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_6,
                                       widgets->view_deliverables->deliverables_cr_6,
                                       "text", 5);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_7,
                                       widgets->view_deliverables->deliverables_cr_7,
                                       "text", 6);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_8,
                                       widgets->view_deliverables->deliverables_cr_8,
                                       "text", 7);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_9,
                                       widgets->view_deliverables->deliverables_cr_9,
                                       "text", 8);
    gtk_tree_view_column_add_attribute(widgets->view_deliverables->deliverables_cx_10,
                                       widgets->view_deliverables->deliverables_cr_10,
                                       "text", 9);

    //Connect view_user
    widgets->view_user = g_slice_new(User);
    widgets->view_user->view_user_stack = GTK_STACK(gtk_builder_get_object(builder, "view_user_stack"));
    widgets->view_user->view_user_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_user"));
    widgets->view_user->edit_user_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "edit_user"));
    widgets->view_user->user_view_edit_button = GTK_BUTTON(gtk_builder_get_object(builder, "user_view_edit_button"));
    widgets->view_user->user_edit_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "user_edit_submit_button"));
    widgets->view_user->user_edit_return_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "user_edit_return_button"));
    widgets->view_user->view_user_image = GTK_IMAGE(gtk_builder_get_object(builder, "view_user_image"));
    widgets->view_user->view_user_first_name = GTK_LABEL(gtk_builder_get_object(builder, "view_user_first_name"));
    widgets->view_user->view_user_last_name = GTK_LABEL(gtk_builder_get_object(builder, "view_user_last_name"));
    widgets->view_user->view_user_birthdate = GTK_LABEL(gtk_builder_get_object(builder, "view_user_birthdate"));
    widgets->view_user->edit_user_birthdate = GTK_LABEL(gtk_builder_get_object(builder, "edit_user_birthdate"));
    widgets->view_user->edit_user_first_name = GTK_ENTRY(gtk_builder_get_object(builder, "edit_user_first_name"));
    widgets->view_user->edit_user_last_name = GTK_ENTRY(gtk_builder_get_object(builder, "edit_user_last_name"));
    widgets->view_user->edit_user_email = GTK_ENTRY(gtk_builder_get_object(builder, "edit_user_email"));
    widgets->view_user->view_user_email = GTK_LINK_BUTTON(gtk_builder_get_object(builder, "view_user_email"));
    widgets->view_user->view_user_image_file_picker = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "view_user_image_file_picker"));

    //Connect view_settings
    widgets->view_settings = g_slice_new(Settings);
    widgets->view_settings->view_settings_switch_theme_button = GTK_SWITCH(
            gtk_builder_get_object(builder, "view_settings_switch_theme_button"));
    widgets->view_settings->settings_database_file_chooser = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "settings_database_file_chooser"));
    widgets->view_settings->settings_database_refresh = GTK_BUTTON(
            gtk_builder_get_object(builder, "settings_database_refresh"));
    widgets->view_settings->settings_storage_folder_chooser = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "settings_storage_folder_chooser"));
    widgets->view_settings->settings_storage_refresh = GTK_BUTTON(
            gtk_builder_get_object(builder, "settings_storage_refresh"));
    widgets->view_settings->settings_glade_file_chooser = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "settings_glade_file_chooser"));
    widgets->view_settings->settings_glade_refresh = GTK_BUTTON(
            gtk_builder_get_object(builder, "settings_glade_refresh"));
    widgets->view_settings->settings_default_theme_file_chooser = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "settings_default_theme_file_chooser"));
    widgets->view_settings->settings_default_theme_refresh = GTK_BUTTON(
            gtk_builder_get_object(builder, "settings_default_theme_refresh"));
    widgets->view_settings->settings_dark_theme_file_chooser = GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "settings_dark_theme_file_chooser"));
    widgets->view_settings->settings_dark_theme_refresh = GTK_BUTTON(
            gtk_builder_get_object(builder, "settings_dark_theme_refresh"));
    widgets->view_settings->view_settings_submit_button = GTK_BUTTON(
            gtk_builder_get_object(builder, "view_settings_submit_button"));

}

void setSearchEntry(gboolean visible, GtkTreeView *treeView, const char *placeholder) {
    if (visible) {
        gtk_fixed_move(widgets->gtk_fixed, GTK_WIDGET(widgets->menu_stack_switcher), 162, 161);
        gtk_widget_set_visible(GTK_WIDGET(widgets->search_entry), TRUE);
        gtk_tree_view_set_search_entry(treeView, GTK_ENTRY(widgets->search_entry));
        gtk_entry_set_text(GTK_ENTRY(widgets->search_entry), "");
        gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->search_entry), placeholder);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->search_entry), FALSE);
        gtk_fixed_move(widgets->gtk_fixed, GTK_WIDGET(widgets->menu_stack_switcher), 85, 161);
    }
}

void GTKSaveFile(char *path) {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;


    dialog = gtk_file_chooser_dialog_new("Save File",
                                         GTK_WINDOW(widgets->window_dashboard),
                                         action,
                                         "Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
    chooser = GTK_FILE_CHOOSER (dialog);

    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);


    gtk_file_chooser_set_filename(chooser, path);

    res = gtk_dialog_run(GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;

        filename = gtk_file_chooser_get_filename(chooser);
        copyFile(path, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

}

void dashboardGTK(int *argc, char ***argv) {
    // Déclaration des variables
    widgets = g_slice_new(App_widgets);

    gtk_init(argc, argv);

    builder = gtk_builder_new_from_file(gladeFile); // Chemin absolu à modifier

    connectWidgets();

    g_signal_connect(widgets->window_dashboard, "destroy", G_CALLBACK(on_destroy), NULL);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder); // Decreases the reference count of builder : if count = 0, memory is freed

    GTKSetTheme();

    gtk_widget_show_all(widgets->window_dashboard);

    gtk_main();

    g_slice_free(Students, widgets->view_students);
    g_slice_free(Classes, widgets->view_classes);
    g_slice_free(Sanctions, widgets->view_sanctions);
    g_slice_free(Deliverables, widgets->view_deliverables);
    g_slice_free(User, widgets->view_user);
    g_slice_free(Settings, widgets->view_settings);
    g_slice_free(App_widgets, widgets);
}
