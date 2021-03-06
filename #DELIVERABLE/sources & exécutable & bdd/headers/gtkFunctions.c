#include "gtkFunctions.h"
#include "sqliteFunctions.h"
#include "functions.h"

/////////////////////////////////////////////////// SIGNALS ////////////////////////////////////////////////////////////

// Close the app window and quit the program
void on_destroy() {
    gtk_main_quit();
}

// Called when sanction row is double clicked or selected + press enter
void on_sanctions_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("SANCTION ID: %d\n", id);
    GTKEditSanction(id); // Display the sanction edition page
}

// Called when class row is double clicked or selected + press enter
void on_classes_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("CLASS ID: %d\n", id);
    GTKEditClass(id); // Display the class edition page
}

// Called when student row is double clicked or selected + press enter
void on_students_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("STUDENT ID: %d\n", id);
    GTKEditStudent(id); // Display the student edition page
}

// Called when deliverable row is double clicked or selected + press enter
void on_deliverables_tree_view_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint id = get_id_row_activated(tree_view, path);
    printf("DELIVERABLE ID: %d\n", id);
    GTKEditDeliverables(id); // Display the deliverable edition page
}

// Get the visible stack and display the default view page of the category
void on_menu_stack_visible_child_notify(GtkStack *stack) {
    if (gtk_stack_get_visible_child_name(stack) != NULL) { // Check if the stack has a visible child
        const gchar *menu = gtk_stack_get_visible_child_name(
                widgets->menu_stack); // Returns the name of the visible menu child
        if (!strcmp(menu, "view_classes")) {
            printf("Classes view\n");
            setSearchEntry(FALSE, NULL, NULL);  // Hide the search bar
            GTKListClasses();                   // Display the list of the classes
        } else if (!strcmp(menu, "view_students")) {
            printf("Students view\n");
            setSearchEntry(TRUE, widgets->view_students->students_tree_view,
                           "Search by firstname"); // Display the search bar
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

// Called when clicking on the menu button + force the stack detection when opening the app
void on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher) {
    // check if there's a stack to retrieve
    if (gtk_stack_switcher_get_stack(stackSwitcher) != NULL) {
        // display the default view page of the category
        on_menu_stack_visible_child_notify(gtk_stack_switcher_get_stack(stackSwitcher));
    }
}

// Called when clicking the "refresh" button on the classes list view
void on_classes_view_refresh_button_clicked() {
    printf("Refreshing classes list\n");
    GTKListClasses();
}

// Called when clicking the "delete" button on the classes list view
void on_classes_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_classes->classes_tree_selection);
    if (int_data) {
        deleteClass(int_data);  // delete class and set class = null for students
        GTKListClasses();       // get back to the updated list
    }
    printf("ID of deleted class: %d\n", int_data);
}

// Called when clicking the "create" button on the classes list view
void on_classes_view_create_button_clicked() {
    printf("Create class\n");
    GTKCreateClass(); // display class creation view
}

// Called when clicking the "return" button on the class edition view
void on_class_edit_return_button_clicked() {
    printf("Return to classes view\n");
    GTKListClasses();
}

// Called when clicking the "submit" button on the class edition view
void on_class_edit_submit_button_clicked() {
    printf("Submit class edit\n");
    GTKEditClassSubmit(); // update the class item and get back to the updated list
}

// Called when clicking the "return" button on the class creation view
void on_class_create_return_button_clicked() {
    printf("Return to classes view\n");
    GTKListClasses();
}

// Called when clicking the "submit" button on the class creation view
void on_class_create_submit_button_clicked() {
    printf("Submit class create\n");
    GTKCreateClassSubmit(); // creates the class item and get back to the updated list
}

// Called when clicking the "refresh" button on the students list view
void on_students_view_refresh_button_clicked() {
    printf("Refresh students\n");
    GTKListStudents();
}

// Called when clicking the "delete" button on the students list view
void on_students_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        deleteStudent(int_data);    // remove from database + all related files and folders
        GTKListStudents();          // get back to the updated list
    }
    printf("Delete student ID: %d\n", int_data);
}

// Called when clicking the "create" button on the students list view
void on_students_view_create_button_clicked() {
    printf("Create student\n");
    GTKCreateStudent(); // display the view of student creation
}

// Called when clicking the "- Bottle" button on the students list view
void on_students_view_remove_bottle_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        addStudentBottle(int_data, -1); // remove 1 bottle from student's total
        GTKListStudents();              // get back to the updated list of students
    }
    printf("Remove bottle student ID: %d\n", int_data);
}

// Called when clicking the "+ Bottle" button on the students list view
void on_students_view_add_bottle_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        addStudentBottle(int_data, 1);  // add 1 bottle to student's total
        GTKListStudents();              // get back to the updated list of students
    }
    printf("Add bottle student ID: %d\n", int_data);
}

// Called when clicking the "return" button on the student edition view
void on_student_edit_return_button_clicked() {
    printf("Return to students view\n");
    GTKListStudents();
}

// Called when clicking the "submit" button on the student edition view
void on_student_edit_submit_button_clicked() {
    printf("Submit student edit\n");
    GTKEditStudentSubmit(); // update the student item and get back to the updated list
}

// Called when clicking the "return" button on the student creation view
void on_student_create_return_button_clicked() {
    printf("Return to students view\n");
    GTKListStudents();
}

// Called when clicking the "submit" button on the student creation view
void on_student_create_submit_button_clicked() {
    printf("Submit student create\n");
    GTKCreateStudentSubmit(); // create the student item and get back to the updated list
}

// Called when selecting a student picture file
void on_edit_student_image_file_picker_file_set() {
    // get filename for the selected file in the file selector
    char *path = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_students->edit_student_image_file_picker));
    printf("Chosen file: %s\n", path);

    // update the image (database and file)
    int returnCode = GTKEditStudentSetImage(path);
    if (returnCode)
        fprintf(stderr, "Can't use this image.\n");
    else {
        printf("Image updated.\n");
    }
}

// Called when clicking the "refresh" button on the sanctions list view
void on_sanctions_view_refresh_button_clicked() {
    printf("Refresh sanctions\n");
    GTKListSanctions();
}

// Called when clicking the "delete" button on the sanctions list view
void on_sanctions_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_sanctions->sanctions_tree_selection);
    if (int_data) {
        deleteSanction(int_data); // deleting sanction and setting sanction = null for the classes
        GTKListSanctions();       // get back to the updated list
    }
    printf("ID of deleted sanction: %d\n", int_data);
}

// Called when clicking the "create" button on the sanctions list view
void on_sanctions_view_create_button_clicked() {
    printf("Create sanction\n");
    GTKCreateSanction(); // display the sanction creation view
}

// Called when clicking the "return" button on the sanction edition view
void on_sanction_edit_return_button_clicked() {
    printf("Return to sanctions view\n");
    GTKListSanctions();
}

// Called when clicking the "submit" button on the sanction edition view
void on_sanction_edit_submit_button_clicked() {
    printf("Submit sanction edit\n");
    GTKEditSanctionSubmit(); // edit the sanction and get back to the sanction list view
}

// Called when clicking the "return" button on the sanction creation view
void on_sanction_create_return_button_clicked() {
    printf("Return to sanctions view\n");
    GTKListSanctions();
}

// Called when clicking the "submit" button on the sanction creation view
void on_sanction_create_submit_button_clicked() {
    printf("Submit sanction create\n");
    GTKCreateSanctionSubmit(); // create new sanction and get back to the updated list view
}

// Called when clicking the "refresh" button on the deliverable list view
void on_deliverables_view_refresh_button_clicked() {
    printf("Refresh deliverables\n");
    GTKListDeliverables();
}

// Called when clicking the "delete" button on the deliverable list view
void on_deliverables_view_delete_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_deliverables->deliverables_tree_selection);
    if (int_data) {
        deleteDeliverable(int_data);    // deleting all the related files + sql updated
        GTKListDeliverables();          // get back to updated list
    }
    printf("ID of deleted deliverable: %d\n", int_data);
}

// Called when clicking the "create" button on the deliverables list view
void on_deliverables_view_create_button_clicked() {
    guint int_data = get_id_row_selected(widgets->view_students->students_tree_selection);
    if (int_data) {
        printf("Creating deliverable for student with id: %d\n", int_data);
        GTKCreateDelivreables(int_data); // display the deliverable creation view
    }
}

// Called when clicking the "return" button on the class edition view
void on_deliverable_edit_return_button_clicked() {
    printf("Return to deliverables view\n");
    GTKListDeliverables();
}

// Called when clicking the "submit" button on the deliverable edition view
void on_deliverable_edit_submit_button_clicked() {
    printf("Submit deliverable edit\n");
    GTKEditDelivreablesSubmit(); // create new deliverable and get back to the updated list view
}

// Called when clicking on the cancel button next to the student picture file chooser
void on_create_student_image_clear_button_clicked() {
    printf("Clearing student image selection.\n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_students->create_student_image_file_picker));
}

// Called when clicking the "return" button on the deliverable creation view
void on_deliverable_create_return_button_clicked() {
    printf("Return to deliverables view\n");
    GTKListDeliverables();
}

// Called when clicking the "submit" button on the deliverable creation view
void on_deliverable_create_submit_button_clicked() {
    printf("Submitting deliverable creation.\n");
    GTKCreateDelivreablesSubmit(); // create the deliverable and get back to the updated list
}

// Called when selecting a deliverable video file
void on_edit_deliverable_video_file_set() {
    // get filename for the selected file in the file selector
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_video));
    printf("Chosen video file: %s\n", path);
    if (GTKDeliverableSetVideo(path)) // Update the deliverable with new video file
        fprintf(stderr, "Error while adding video\n");
}

// Called when selecting a deliverable bad code file
void on_edit_deliverable_bad_code_file_set() {
    // get filename for the selected file in the file selector
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_bad_code));
    printf("Chosen bad code file: %s\n", path);
    if (GTKDeliverableSetBadCode(path)) // Update the deliverable with new bad code file
        fprintf(stderr, "Error while adding bad code file.\n");
}

// Called when selecting a deliverable file
void on_edit_deliverable_deliverable_file_file_set() {
    // get filename for the selected file in the file selector
    char *path = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_deliverable_file));
    printf("Chosen deliverable file: %s\n", path);
    if (GTKDeliverableSetDeliverable(path)) // Update the deliverable with new deliverable file
        fprintf(stderr, "Error while adding deliverable file.\n");
}

// Called when selecting a deliverable audio file
void on_edit_deliverable_audio_file_set() {
    // get filename for the selected file in the file selector
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_deliverables->edit_deliverable_audio));
    printf("Chosen audio file: %s\n", path);
    if (GTKDeliverableSetAudio(path)) // Update the deliverable with new audio file
        fprintf(stderr, "Error while adding audio file.\n");
}

// Called when clicking on the download button next to the audio file file chooser
void on_edit_deliverable_audio_download_clicked() {
    // get the content of the tooltip text, dynamically generated
    char *path = gtk_widget_get_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download));
    printf("Downloading audio: %s\n", path);
    GTKSaveFile(path); // open a dialog window to locally download the file
}

// Called when clicking on the download button next to the video file file chooser
void on_edit_deliverable_video_download_clicked() {
    // get the content of the tooltip text, dynamically generated
    char *path = gtk_widget_get_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download));
    printf("Downloading video\n");
    GTKSaveFile(path); // open a dialog window to locally download the file
}

// Called when clicking on the download button next to the bad code file file chooser
void on_edit_deliverable_bad_code_download_clicked() {
    // get the content of the tooltip text, dynamically generated
    char *path = gtk_widget_get_tooltip_text(
            GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download));
    printf("Downloading bad code\n");
    GTKSaveFile(path); // open a dialog window to locally download the file
}

// Called when clicking on the download button next to the deliverable file file chooser
void on_edit_deliverable_deliverable_file_download_clicked() {
    // get the content of the tooltip text, dynamically generated
    char *path = gtk_widget_get_tooltip_text(
            GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download));
    printf("Downloading deliverable\n");
    GTKSaveFile(path); // open a dialog window to locally download the file
}

// Called when clicking on the cancel button next to the audio file file chooser
void on_create_deliverable_audio_clear_clicked() {
    printf("Clearing audio.\n");
    // empty the file chooser
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_audio));
}

// Called when clicking on the cancel button next to the video file file chooser
void on_create_deliverable_video_clear_clicked() {
    printf("Clearing video.\n");
    // empty the file chooser
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_video));
}

// Called when clicking on the cancel button next to the bad code file file chooser
void on_create_deliverable_bad_code_clear_clicked() {
    printf("Clearing bad_code.\n");
    // empty the file chooser
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_bad_code));
}

// Called when clicking on the cancel button next to the deliverable file file chooser
void on_create_deliverable_deliverable_file_clear_clicked() {
    printf("Clearing deliverable.\n");
    // empty the file chooser
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_deliverable_file));
}

// Called when selecting a user picture file
void on_view_user_image_file_picker_file_set() {
    // get filename for the selected file in the file selector
    char *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_user->view_user_image_file_picker));
    printf("Chosen file: %s\n", path);

    // update the image (database and file)
    int returnCode = GTKUserSetImage(path);
    if (returnCode)
        fprintf(stderr, "Can't use this image\n");
    else {
        printf("Image changed\n");
        GTKViewUser(); // get back to user view with updated image
    }
}

// Called when clicking the "edit" button on the user edition view
void on_user_view_edit_button_clicked() {
    printf("User edition view\n");
    GTKEditUser(); // display user edition view
}

// Called when clicking the "submit" button on the user edition view
void on_user_edit_submit_button_clicked() {
    printf("Submitting new user information...\n");
    GTKEditUserSubmit(); // update user information
}

// Called when clicking the "return" button on the user edition view
void on_user_edit_return_button_clicked() {
    printf("Return to user view.\n");
    GTKViewUser();
}

// Called when switching the "dark theme" toggle button
void on_view_settings_switch_theme_button_state_set() {
    printf("Theme button switched!\n");
    // darkTheme: global variable, get the state of the switch button (1 if active, 0 if not)
    darkTheme = gtk_switch_get_active(widgets->view_settings->view_settings_switch_theme_button);
    GTKSetTheme();     // load the theme depending on the darkTheme global variable value
    writeConf();       // update the configuration file with the new darkTheme value
}

// Called when switching the "show terminal" toggle button
void on_view_settings_show_terminal_button_state_set() {
    printf("Show terminal button switched.\n");
    // showConsole: global variable, get the state of the switch button (1 if active, 0 if not)
    showConsole = gtk_switch_get_active(widgets->view_settings->view_settings_show_terminal_button);
    GTKShowConsole(); // show or hide console depending on the button state
    writeConf();      // update the configuration file with the new showConsole value
}

// Called when clicking the "apply" button on the settings edition view
void on_view_settings_submit_button_clicked() {
    printf("Submitting settings.\n");
    GTKViewSettingsSubmit(); // Update global variables with content of the settings edition view + write new conf file
}

// Called when clicking the "refresh" button next to the storage file chooser
void on_settings_storage_refresh_clicked() {
    printf("Storage folder refreshed: back to default value.\n");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser));
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser),
                                        storageFolder);
}

// Called when clicking the "refresh" button next to the dark theme file chooser
void on_settings_dark_theme_refresh_clicked() {
    printf("Dark theme file refreshed: back to default value.\n");
    // Set the file chooser back to the default value from the conf file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser),
                                  darkThemePath);
}

// Called when clicking the "refresh" button next to the default theme file chooser
void on_settings_default_theme_refresh_clicked() {
    printf("Default theme file refreshed: back to default value.\n");
    // Set the file chooser back to the default value from the conf file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser),
                                  defaultThemePath);
}

// Called when clicking the "refresh" button next to the glade file chooser
void on_settings_glade_refresh_clicked() {
    printf("Glade file refreshed: back to default value.\n");
    // Set the file chooser back to the default value from the conf file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser), gladeFile);
}

// Called when clicking the "refresh" button next to the database file chooser
void on_settings_database_refresh_clicked() {
    printf("Database file refreshed: back to default value.\n");
    // Set the file chooser back to the default value from the conf file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser), dbname);
}

///////////////////////////////////////////////// FUNCTIONS ////////////////////////////////////////////////////////////

// Get the ID of the item whose row is activated (double click or selection + enter)
guint get_id_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    guint int_data;
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);

    if (gtk_tree_model_get_iter(model, &iter, path)) {      // iter = the activated line in the treeview (path)
        gtk_tree_model_get(model, &iter, 0, &int_data, -1); // get the value of the column 0 (id)
        return int_data;                                    // return the id of the item
    } else {
        fprintf(stderr, "Error: selected column not found!\n");
        return EXIT_FAILURE;
    }
}

// Get the ID of the item whose row is selected (used for buttons)
guint get_id_row_selected(GtkTreeSelection *selection) {
    guint int_data = 0;
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) { // iter = the selected line in the treeview
        gtk_tree_model_get(model, &iter, 0, &int_data, -1);          // get the value of the column 0 (id)
    }
    return int_data;
}

// Fill the combo list of the app users
void fillUserComboList(GtkComboBoxText *comboBoxText) {
    char *user;
    getUser(&user, 1); // user = "id|email|first_name|last_name|photo|birthdate;\n" (ID = 1)
    size_t columnSize, nameSize;
    char *firstAdress = user, *nameBuffer, *idBuffer;

    // Get ID string
    columnSize = strchr(user, '|') - user;  // difference between the first character and the first '|'
    idBuffer = malloc(columnSize + 1);      // allocate memory + 1 for \0
    strncpy(idBuffer, user, columnSize);    // copy column content into variable
    idBuffer[columnSize] = '\0';            // add \0 on the last character

    user += columnSize + 1;                 // move the cursor behind the pipe to the next column (email)
    user += strchr(user, '|') - user + 1;   // move the cursor behind the pipe to the next column (first name)

    // Get firstname string
    columnSize = strchr(user, '|') - user;
    nameBuffer = malloc(columnSize + 1);
    strncpy(nameBuffer, user, columnSize);
    nameBuffer[columnSize] = '\0';

    user += columnSize + 1;                 // move the cursor behind the pipe to the next column (last name)

    // Get last name string
    columnSize = strchr(user, '|') - user;
    nameBuffer = realloc(nameBuffer, strlen(nameBuffer) + columnSize + 2); // full size = firstname + ' ' + lastname
    strcat(nameBuffer, " ");                // concatenate space character
    nameSize = strlen(nameBuffer);          // size of the complete name
    strncat(nameBuffer, user, columnSize);  // concatenate the content of column into the name string
    nameBuffer[columnSize + nameSize] = '\0';

    // Flush all previous data from the combo box
    gtk_combo_box_text_remove_all(comboBoxText);
    // Fill the table with user data: name (visible) + id (invisible)
    gtk_combo_box_text_append(comboBoxText, idBuffer, nameBuffer);

    free(idBuffer);
    free(nameBuffer);
    free(firstAdress);
}

// Fill the combo list of the classes
void fillClassComboList(GtkComboBoxText *comboBoxText) {
    char *classList;
    listClasses(
            &classList); // classList = n * "id|name|year|apprenticeship|major|user(first_name + last_name)|user_fk|sanction(name)|sanction_fk;\n"

    int nbClass = 0, i;
    // result: address of the first character of the classList string, used as a cursor
    char *result = classList, *firstAdress = classList, *nameBuffer, *idBuffer;
    size_t columnSize;

    // Flush all previous data from the combo box
    gtk_combo_box_text_remove_all(comboBoxText);

    // Count the number of rows (ends with ";\n")
    while ((result = strstr(result, ";\n"))) {
        nbClass++;
        result++; // move the cursor to the character after ";" to avoid infinite loop
    }

    for (i = 0; i < nbClass; ++i) {
        // Get ID string
        result = strchr(classList, '|');            // result = position of the first | of the row
        columnSize = result - classList;            // difference between the first character and the first '|'
        idBuffer = malloc(columnSize + 1);          // allocate memory for the string
        strncpy(idBuffer, classList, columnSize);   // copy column content into variable
        idBuffer[columnSize] = '\0';                // add \0 on the last character
        classList += columnSize + 1;                // move the cursor behind the pipe to the next column

        // Get name string
        result = strchr(classList, '|');
        columnSize = result - classList;
        nameBuffer = malloc(columnSize + 1);
        strncpy(nameBuffer, classList, columnSize);
        nameBuffer[columnSize] = '\0';

        // Fill the table with class data: name (visible) + id (invisible)
        gtk_combo_box_text_append(comboBoxText, idBuffer, nameBuffer);

        // The other columns are not important: getting to the end of the line + 2 to move to the next line
        classList = strstr(classList, ";\n") + 2;

        // Flush the data in the buffers to restart the process
        free(idBuffer);
        free(nameBuffer);
    }
    free(firstAdress);
}

// Fill the combo list of the sanctions
void fillSanctionComboList(GtkComboBoxText *comboBoxText) {
    char *sanctionsList;
    listSanctions(&sanctionsList); // sanctionsList = n * "id|name|description|user(first_name + last_name)|user_fk;\n"

    int nbSanctions = 0, i;
    // result: address of the first character of the classList string, used as a cursor
    char *result = sanctionsList, *firstAdress = sanctionsList, *nameBuffer, *idBuffer;
    size_t columnSize;

    // Flush all previous data from the combo box
    gtk_combo_box_text_remove_all(comboBoxText);

    // Add first entry to the combo list
    gtk_combo_box_text_append(comboBoxText, "0", "None");

    // Count the number of rows (ends with ";\n")
    while ((result = strstr(result, ";\n"))) {
        nbSanctions++;
        result++;   // moving the cursor behind ";" to avoid infinite loop
    }

    for (i = 0; i < nbSanctions; ++i) {
        // Get ID string
        result = strchr(sanctionsList, '|');            // result = position of the first | of the row
        columnSize = result - sanctionsList;            // difference between the first character and the first '|'
        idBuffer = malloc(columnSize + 1);              // allocating memory for the string
        strncpy(idBuffer, sanctionsList, columnSize);   // copying column content into variable
        idBuffer[columnSize] = '\0';                    // adding \0 on the last character
        sanctionsList += columnSize + 1;                // moving the cursor behind the pipe to the next column

        // Get name string
        result = strchr(sanctionsList, '|');
        columnSize = result - sanctionsList;
        nameBuffer = malloc(columnSize + 1);
        strncpy(nameBuffer, sanctionsList, columnSize);
        nameBuffer[columnSize] = '\0';

        // Fill the table with sanction data: name (visible) + id (invisible)
        gtk_combo_box_text_append(comboBoxText, idBuffer, nameBuffer);

        // The other columns are not important: getting to the end of the line + 2 to move to the next line
        sanctionsList = strstr(sanctionsList, ";\n") + 2;

        free(idBuffer);
        free(nameBuffer);
    }
    free(firstAdress);
}

// Fill the combo list of the deliverable status
void fillStatusComboList(GtkComboBoxText *comboBoxText, char *status) {
    // Flush all previous data from the combo box
    gtk_combo_box_text_remove_all(comboBoxText);

    // Add entries in combo list: ID (invisible) + name (visible)
    gtk_combo_box_text_append(comboBoxText, "1", "To do");
    gtk_combo_box_text_append(comboBoxText, "2", "Completed");
    gtk_combo_box_text_append(comboBoxText, "3", "On hold");
    gtk_combo_box_text_append(comboBoxText, "4", "Checking");
    gtk_combo_box_text_append(comboBoxText, "5", "Waiting deliverable");

    // Display the entry list matching the deliverable status
    if (!strcmp(status, "To do"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "1");
    if (!strcmp(status, "Completed"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "2");
    if (!strcmp(status, "On hold"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "3");
    if (!strcmp(status, "Checking"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "4");
    if (!strcmp(status, "Waiting deliverable"))
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(comboBoxText), "5");
}

// Display the students list view + new database request
void GTKListStudents() {
    // Display the student list view
    gtk_stack_set_visible_child(widgets->view_students->view_students_stack,
                                widgets->view_students->view_students_fixed);

    // Flush the previous data from the tree store
    gtk_tree_store_clear(widgets->view_students->students_tree_store);

    char *students, *result, *firstAddress, *buffer;
    int nbStudents = 0;
    listStudents(
            &students); // students = n * "id|first_name|last_name|photo|email|bad_code(count)|nb_bottles|class(name)|class_fk;\n"
    firstAddress = students;
    result = students;       // result: address of the first character of the students string, used as a cursor
    GtkTreeIter iter;        // row pointer

    // Counting the number of students
    while ((result = strstr(result, ";\n"))) {
        nbStudents++;
        result++;           // move the cursor behind ";" to avoid infinite loop
    }

    // Fill the tree view
    for (int i = 0; i < nbStudents; ++i) {
        // Create a new row, pointed by "iter"
        gtk_tree_store_append(widgets->view_students->students_tree_store, &iter, NULL);

        // ID
        result = strchr(students, '|');             // result = position of the first | of the row
        size_t columnSize = result - students;      // difference between the first character and the first '|'
        buffer = malloc(columnSize + 1);      // allocate memory for the string
        strncpy(buffer, students, columnSize);      // copy column content into variable
        buffer[columnSize] = '\0';                  // add \0 on the last character
        // Fill the first column (0) of the new line the with the ID
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 0, atoi(buffer), -1);
        students += columnSize + 1;                 // move the cursor behind the pipe to the next column
        free(buffer);                               // liberating memory for next column

        // FIRST_NAME
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 1, buffer, -1);
        students += columnSize + 1;
        free(buffer);

        // LAST_NAME
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 2, buffer, -1);
        students += columnSize + 1;
        students += strchr(students, '|') - students + 1; // move behind photo column
        free(buffer);

        // EMAIL
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 4, buffer, -1);
        students += columnSize + 1;
        free(buffer);

        // BAD_CODE
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 5, atoi(buffer), -1);
        students += columnSize + 1;
        free(buffer);

        //NB_BOTTLES
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 6, atoi(buffer), -1);
        students += columnSize + 1;
        free(buffer);

        //CLASS
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 7, buffer, -1);
        students += columnSize + 1;
        free(buffer);

        //CLASS_FK
        result = strstr(students, ";\n");
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_students->students_tree_store, &iter, 8, atoi(buffer), -1);
        students += columnSize + 2; // +2 for ";\n": move to the next row
        free(buffer);
    }
    free(firstAddress);
}

// Display the content of the student edit view
void GTKEditStudent(int id) {
    // Display the student edition view
    gtk_stack_set_visible_child(widgets->view_students->view_students_stack,
                                widgets->view_students->edit_student_fixed);
    char *first_name, *last_name, *photo, *email, *bottles, *class, *class_fk, idBuffer[6];

    // Fill the classes combo list
    fillClassComboList(widgets->view_students->edit_student_class);

    // Fill the variables with results from database request
    GTKStudentGetData(id, &first_name, &last_name, &photo, &email, &bottles, &class, &class_fk);
    itoa(id, idBuffer, 10);

    // Display a scaled version of the student image
    GTKEditStudentImage(photo);

    // Fill entries with request return
    gtk_label_set_text(widgets->view_students->edit_student_id, idBuffer);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_students->edit_student_id), FALSE); // hide the ID label
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

// Update student data
void GTKEditStudentSubmit() {
                     // check if all the fields are filled then sql update request
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

        // Update list and get back to list view
        GTKListStudents();
    }
}

// Check if the required fields are filled when editing a student
int GTKEditStudentSubmitCheckRequiredField() {
    int returnCode = 0;

    // Set bottle count to 0 if negative number
    if (atoi(gtk_entry_get_text(GTK_ENTRY(widgets->view_students->edit_student_bottles))) < 0)
        gtk_entry_set_text(GTK_ENTRY(widgets->view_students->edit_student_bottles), "0");

    if (strlen(gtk_entry_get_text(widgets->view_students->edit_student_first_name)) == 0) {
        fprintf(stderr, "First name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->edit_student_last_name)) == 0) {
        fprintf(stderr, "Last name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->edit_student_email)) == 0) {
        fprintf(stderr, "Email empty!\n");
        returnCode = 1;
    }

    return returnCode;
}

// Fill the variables in parameters with result from single student database request
void GTKStudentGetData(int id, char **first_name, char **last_name, char **photo, char **email, char **bottles,
                       char **class, char **class_fk) {
    char *studentData, *firstAdress;
    size_t columnSize;
    getStudent(&studentData, id); // studentData = "id|first_name|last_name|photo|email|bad_code(count)|nb_bottles|class|class_fk;\n"
    firstAdress = studentData;

    // ID
    studentData += strchr(studentData, '|') - studentData + 1; // id already known, moving to next column

    // FIRST_NAME
    columnSize = strchr(studentData, '|') - studentData;    // difference between the first character and the next '|'
    *first_name = malloc(columnSize + 1);                   // allocate memory for the string
    strncpy(*first_name, studentData, columnSize);          // copy column content into parameter variable
    (*first_name)[columnSize] = '\0';                       // add \0 on the last character
    studentData += columnSize + 1;                          // move the cursor behind the pipe to the next column

    // LAST_NAME
    columnSize = strchr(studentData, '|') - studentData;
    *last_name = malloc(columnSize + 1);
    strncpy(*last_name, studentData, columnSize);
    (*last_name)[columnSize] = '\0';
    studentData += columnSize + 1;

    // PHOTO
    columnSize = strchr(studentData, '|') - studentData;
    *photo = malloc(columnSize + 1);
    strncpy(*photo, studentData, columnSize);
    (*photo)[columnSize] = '\0';
    studentData += columnSize + 1;

    // EMAIL
    columnSize = strchr(studentData, '|') - studentData;
    *email = malloc(columnSize + 1);
    strncpy(*email, studentData, columnSize);
    (*email)[columnSize] = '\0';
    studentData += columnSize + 1;

    // BAD CODE COUNT
    studentData += strchr(studentData, '|') - studentData + 1;  // not interested, moving to next column

    // BOTTLES
    columnSize = strchr(studentData, '|') - studentData;
    *bottles = malloc(columnSize + 1);
    strncpy(*bottles, studentData, columnSize);
    (*bottles)[columnSize] = '\0';
    studentData += columnSize + 1;

    // CLASS
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

// Display a scaled version of the student image
void GTKEditStudentImage(char *path) {
    // Reset the image to be empty
    gtk_image_clear(widgets->view_students->edit_student_image);

    // Structure describing an image
    GdkPixbuf *pixbuf;

    if (strlen(path) < 1) {
        char *defaultProfilPicture = malloc(strlen(storageFolder) + 12);
        sprintf(defaultProfilPicture, "%s/profil.png", storageFolder);
        // Load default image
        if ((pixbuf = gdk_pixbuf_new_from_file(defaultProfilPicture, NULL)) == NULL) {
            fprintf(stderr, "Error while loading default user profile picture\n");
        } else {
            printf("Loaded!\n");
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
        // Load image from file
        if ((pixbuf = gdk_pixbuf_new_from_file(path, NULL)) == NULL) {
            fprintf(stderr, "Error while loading user profil picture\n");
        } else {
            printf("Loaded!\n");
            int width = gdk_pixbuf_get_width(pixbuf);
            int height = gdk_pixbuf_get_height(pixbuf);
            double ratio = (250. / width);
            printf("width: %d, height: %d, ratio: %lf\n", width, height, ratio);

            // Create a new scaled copy of original image, forcing width to 250
            gtk_image_set_from_pixbuf(widgets->view_students->edit_student_image,
                                      gdk_pixbuf_scale_simple(pixbuf, floor(width * ratio), floor(height * ratio),
                                                              GDK_INTERP_BILINEAR));
        }
    }
}

// Update the image (database and file)
int GTKEditStudentSetImage(char *path) {
    // Check if extension is OK
    if (!checkImageExtension(path))
        return 1;

    // Get ID + insert image path in the database + create the directories/file
    int id = atoi(gtk_label_get_text(widgets->view_students->edit_student_id));
    int returnCode = insertTableImage("student", id, path);
    if (returnCode)
        return 1;

    // Back to edition view with updated image
    GTKEditStudent(id);

    return 0;
}

// Display the content of the student creation view
void GTKCreateStudent() {
    // Display the content of the student creation view
    gtk_stack_set_visible_child(widgets->view_students->view_students_stack,
                                widgets->view_students->create_student_fixed);

    // Fill the combo list of the classes
    fillClassComboList(widgets->view_students->create_student_class);

    // Set empty strings to text entries
    gtk_entry_set_text(widgets->view_students->create_student_first_name, "");
    gtk_entry_set_text(widgets->view_students->create_student_last_name, "");
    gtk_entry_set_text(widgets->view_students->create_student_email, "");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_students->create_student_class), "1");

    // Empty the profile photo file chooser
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_students->create_student_image_file_picker));
}

// Create new student from edition view data
void GTKCreateStudentSubmit() {
    // Get photo path from file chooser
    char *photo_path = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_students->create_student_image_file_picker));
    int returnCode;

    // If photo, check extension and update student with photo
    if (photo_path != NULL && strlen(photo_path) > 0 && checkImageExtension(photo_path))
        returnCode = GTKCreateStudentSubmitCheckRequiredField() ||
                    insertStudent(gtk_entry_get_text(widgets->view_students->create_student_first_name),
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
        printf("Student created successfully.\n");
        GTKListStudents();
    }
}

// Check if the required fields are filled when creating a student
int GTKCreateStudentSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_students->create_student_first_name)) == 0) {
        fprintf(stderr, "First name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->create_student_last_name)) == 0) {
        fprintf(stderr, "Last name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_students->create_student_email)) == 0) {
        fprintf(stderr, "Email empty!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Display the classes list view + new database request
void GTKListClasses() {
    // Display the classes list view
    gtk_stack_set_visible_child(widgets->view_classes->view_classes_stack, widgets->view_classes->view_classes_fixed);

    // Flush the previous data from the tree store
    gtk_tree_store_clear(widgets->view_classes->classes_tree_store);

    char *classes, *result, *firstAddress, *buffer;
    int nbClasses = 0;
    listClasses(
            &classes);  // classes = n * "id|name|year|apprenticeship|major|user(first_name + last_name)|user_fk|sanction(name)|sanction_fk;\n"
    firstAddress = classes;
    result = classes;       // result: address of the first character of the classes string, used as a cursor
    GtkTreeIter iter;       // row pointer

    // Count the number of classes
    while ((result = strstr(result, ";\n"))) {
        nbClasses++;
        result++;           // move the cursor behind ";" to avoid infinite loop
    }

    // Fill the tree view
    for (int i = 0; i < nbClasses; ++i) {
        // Create a new row, pointed by "iter"
        gtk_tree_store_append(widgets->view_classes->classes_tree_store, &iter, NULL);

        // ID
        result = strchr(classes, '|');          // result = position of the first | of the row
        size_t columnSize = result - classes;   // difference between the first character and the first '|'
        buffer = malloc(columnSize + 1);  // allocate memory for the string
        strncpy(buffer, classes, columnSize);   // copy column content into variable
        buffer[columnSize] = '\0';              // add \0 on the last character
        // Fill the first column (0) of the new line the with the ID
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 0, atoi(buffer), -1);
        classes += columnSize + 1;              // move the cursor behind the pipe to the next column
        free(buffer);                           // liberating memory for next column

        // NAME
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 1, buffer, -1);
        classes += columnSize + 1;
        free(buffer);

        // YEAR
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 2, atoi(buffer), -1);
        classes += columnSize + 1;
        free(buffer);

        // APPRENTICESHIP
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
        free(buffer);

        // MAJOR
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 4, buffer, -1);
        classes += columnSize + 1;
        free(buffer);

        // USER
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 5, buffer, -1);
        classes += columnSize + 1;
        free(buffer);

        //USER_FK
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 6, atoi(buffer), -1);
        classes += columnSize + 1;
        free(buffer);

        //SANCTION
        result = strchr(classes, '|');
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 7, buffer, -1);
        classes += columnSize + 1;
        free(buffer);

        //SANCTION_FK
        result = strstr(classes, ";\n");
        columnSize = result - classes;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, classes, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_classes->classes_tree_store, &iter, 8, atoi(buffer), -1);
        classes += columnSize + 2; // +2 for ";\n": move to the next row
        free(buffer);
    }
    free(firstAddress);
}

// Display the content of the class edition view
void GTKEditClass(int id) {
    // Display the content of the class edition view
    gtk_stack_set_visible_child(widgets->view_classes->view_classes_stack, widgets->view_classes->edit_class_fixed);
    char *name, *major, *user, *sanction, *year, *sanction_fk, *user_fk, idBuffer[4];
    int apprenticeship;

    // Fill combo lists
    fillSanctionComboList(widgets->view_classes->edit_class_sanction);
    fillUserComboList(widgets->view_classes->edit_class_user);

    // Fill the variables with results from database request
    GTKClassGetData(id, &name, &year, &apprenticeship, &major, &user, &user_fk, &sanction, &sanction_fk);
    itoa(id, idBuffer, 10);

    // Fill entries with request return
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

// Fill the parameters with results from single class database request
void GTKClassGetData(int id, char **name, char **year, int *apprenticeship, char **major, char **user, char **user_fk,
                     char **sanction, char **sanction_fk) {
    char *classData, *intBuffer, *firstAdress;
    size_t columnSize;
    getClass(&classData, id); //classData = "id|name|year|apprenticeship|major|user|user_fk|sanction|sanction_fk;\n"
    firstAdress = classData;

    //ID
    classData += strchr(classData, '|') - classData + 1; // id already known, moving to next column

    //NAME
    columnSize = strchr(classData, '|') - classData;    // difference between the first character and the next '|'
    *name = malloc(columnSize + 1);                     // allocate memory for the string
    strncpy(*name, classData, columnSize);              // copy column content into parameter variable
    (*name)[columnSize] = '\0';                         // add \0 on the last character
    classData += columnSize + 1;                        // move the cursor behind the pipe to the next column

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

// Update the class data
void GTKEditClassSubmit() {
    // check if the required fields have been correctly filled then SQL update: 0 if OK
    int returnCode = GTKEditClassSubmitCheckRequiredField() ||
                     updateClass(atoi(gtk_label_get_text(
                             widgets->view_classes->edit_class_id)), // get class id from invisible label on the view
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
        GTKListClasses(); // Get back to updated list of classes
    }
}

// Check if the required fields have been correctly filled when editing a class
int GTKEditClassSubmitCheckRequiredField() {
    int returnCode = 0;

    // Check if the name is not empty
    if (strlen(gtk_entry_get_text(widgets->view_classes->edit_class_name)) == 0) {
        fprintf(stderr, "Name empty!\n");
        returnCode = 1;
    }

    // Check if the year is composed of four characters
    if (strlen(gtk_entry_get_text(GTK_ENTRY(widgets->view_classes->edit_class_year))) != 4) {
        fprintf(stderr, "Year problem: time travel is forbidden!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Display the content of the class creation view
void GTKCreateClass() {
    // Display the class creation view
    gtk_stack_set_visible_child(widgets->view_classes->view_classes_stack, widgets->view_classes->create_class_fixed);

    // Fill combo lists and display default value
    fillSanctionComboList(widgets->view_classes->create_class_sanction);
    fillUserComboList(widgets->view_classes->create_class_user);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_classes->create_class_sanction), "0");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_classes->create_class_user), "1");

    // Set empty strings for text entries and default values
    gtk_entry_set_text(widgets->view_classes->create_class_name, "");
    gtk_entry_set_text(widgets->view_classes->create_class_major, "");
    gtk_entry_set_text(GTK_ENTRY(widgets->view_classes->create_class_year), "2020");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->view_classes->create_class_apprenticeship), FALSE);

}

// Create new class
void GTKCreateClassSubmit() {
    // check if the required fields have been correctly filled then SQL insertion
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
        GTKListClasses(); // Get back to updated class list view
    }
}

// Check if the required fields have been correctly filled when creating a class
int GTKCreateClassSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_classes->create_class_name)) == 0) {
        fprintf(stderr, "Name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(widgets->view_classes->create_class_year))) != 4) {
        fprintf(stderr, "Wrong year!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Display the content of sanctions list view + new database request
void GTKListSanctions() {
    // Display the sanctions list view
    gtk_stack_set_visible_child(widgets->view_sanctions->view_sanctions_stack,
                                widgets->view_sanctions->view_sanctions_fixed);

    // Flush the previous data from the tree store
    gtk_tree_store_clear(widgets->view_sanctions->sanctions_tree_store);

    char *sanctions, *result, *firstAddress, *buffer;
    int nbSanctions = 0, i;
    listSanctions(&sanctions);  // sanctions = n * "id|name|description|user(first_name + last_name)|user_fk;\n"
    firstAddress = sanctions;
    result = sanctions;         // result: address of the first character of the students string, used as a cursor
    GtkTreeIter iter;           // row pointer

    // Count the number of sanctions
    while ((result = strstr(result, ";\n"))) {
        nbSanctions++;
        result++;               // move the cursor behind ";" to avoid infinite loop
    }

    // Fill the tree view
    for (i = 0; i < nbSanctions; ++i) {
        // Create a new row, pointed by "iter"
        gtk_tree_store_append(widgets->view_sanctions->sanctions_tree_store, &iter, NULL);

        // ID
        result = strchr(sanctions, '|');            // result = position of the first | of the row
        size_t columnSize = result - sanctions;     // difference between the first character and the first '|'
        buffer = malloc(columnSize + 1); // allocate memory for the string
        strncpy(buffer, sanctions, columnSize);     // copy column content into variable
        buffer[columnSize] = '\0';                  // add \0 on the last character
        // Fill the first column (0) of the new line the with the ID
        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 0, atoi(buffer), -1);
        sanctions += columnSize + 1;                // move the cursor behind the pipe to the next column
        free(buffer);                               // liberating memory for next column

        // NAME
        result = strchr(sanctions, '|');
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 1, buffer, -1);
        sanctions += columnSize + 1;
        free(buffer);

        // DESCRIPTION
        result = strchr(sanctions, '|');
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 2, buffer, -1);
        sanctions += columnSize + 1;
        free(buffer);

        //USER
        result = strchr(sanctions, '|');
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 3, buffer, -1);
        sanctions += columnSize + 1;
        free(buffer);

        //USER_FK
        result = strstr(sanctions, ";\n");
        columnSize = result - sanctions;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, sanctions, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_sanctions->sanctions_tree_store, &iter, 4, atoi(buffer), -1);
        sanctions += columnSize + 2;        // +2 for ";\n": move to the next row
        free(buffer);
    }
    free(firstAddress);
}

// Display the content of the sanction edition view
void GTKEditSanction(int id) {
    // Display the sanction edition view
    gtk_stack_set_visible_child(widgets->view_sanctions->view_sanctions_stack,
                                widgets->view_sanctions->edit_sanction_fixed);

    char *name, *description, *user, *user_fk, idBuffer[6];
    itoa(id, idBuffer, 10);

    // Fill the parameters with results from database request
    GTKSanctionGetData(id, &name, &description, &user, &user_fk);

    // Fill user combo list
    fillUserComboList(widgets->view_sanctions->edit_sanction_user);

    // Fill entries with request return
    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->edit_sanction_description);
    gtk_text_buffer_set_text(textBuffer, description, strlen(description));
    gtk_entry_set_text(widgets->view_sanctions->edit_sanction_name, name);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_sanctions->edit_sanction_user), user_fk);
    gtk_label_set_text(widgets->view_sanctions->edit_sanction_id, idBuffer);
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_sanctions->edit_sanction_id), FALSE); // hide ID label

    free(name);
    free(description);
    free(user);
    free(user_fk);
}

// Update sanction
void GTKEditSanctionSubmit() {

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->edit_sanction_description);
    GtkTextIter startIter; // Text buffer iterator
    GtkTextIter endIter;
    gtk_text_buffer_get_start_iter(textBuffer, &startIter);
    gtk_text_buffer_get_end_iter(textBuffer, &endIter);

    // check if the required fields have been correctly filled then SQL update: 0 if OK
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
        GTKListSanctions(); // get back to updated sanctions list
    }
}

// Check if the required fields have been correctly filled when editing a saction
int GTKEditSanctionSubmitCheckRequiredField(char *textIter) {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_sanctions->edit_sanction_name)) == 0) {
        fprintf(stderr, "Name empty!\n");
        returnCode = 1;
    }
    if (strlen(textIter) == 0) {
        fprintf(stderr, "Description empty!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Fill the parameters with result from single sanction database request
void GTKSanctionGetData(int id, char **name, char **description, char **user, char **user_fk) {
    char *sanctionData, *firstAdress;
    size_t columnSize;
    getSanction(&sanctionData, id); // sanctionData = "id|name|description|user(first_name + last_name)|user_fk;\n"
    firstAdress = sanctionData;

    //ID
    sanctionData += strchr(sanctionData, '|') - sanctionData + 1; // id already known, moving to next column

    //NAME
    columnSize = strchr(sanctionData, '|') - sanctionData;  // difference between the first character and the next '|'
    *name = malloc(columnSize + 1);                         // allocate memory for the string
    strncpy(*name, sanctionData, columnSize);               // copy column content into parameter variable
    (*name)[columnSize] = '\0';                             // add \0 on the last character
    sanctionData += columnSize + 1;                         // move the cursor behind the pipe to the next column

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

// Fill the parameters with sanction results from single student database request
void GTKSanctionGetDataStudentId(int student_id, char **name, char **description, char **student, char **student_fk) {
    char *sanctionData, *firstAdress;
    size_t columnSize;
    getSanctionStudentId(&sanctionData, student_id); // sanction data = "id|name|description|student_name|user_fk;\n"
    firstAdress = sanctionData;

    // ID
    sanctionData += strchr(sanctionData, '|') - sanctionData + 1; // moving to next column

    // NAME
    columnSize = strchr(sanctionData, '|') - sanctionData;  // difference between the cursor position and the next '|'
    *name = malloc(columnSize + 1);                         // allocate memory for the string
    strncpy(*name, sanctionData, columnSize);               // copy column content into parameter variable
    (*name)[columnSize] = '\0';                             // add \0 on the last character
    sanctionData += columnSize + 1;                         // move the cursor behind the pipe to the next column

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

// Display the content of the sanction creation view
void GTKCreateSanction() {
    // Display the sanction creation view
    gtk_stack_set_visible_child(widgets->view_sanctions->view_sanctions_stack,
                                widgets->view_sanctions->create_sanction_fixed);

    // Fill combo list
    fillUserComboList(widgets->view_sanctions->create_sanction_user);

    // Set empty strings for text entries and default values
    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->create_sanction_description);
    gtk_text_buffer_set_text(textBuffer, "", 0);
    gtk_entry_set_text(widgets->view_sanctions->create_sanction_name, "");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(widgets->view_sanctions->create_sanction_user), "1");
}

// Create new sanction from edition view
void GTKCreateSanctionSubmit() {
    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(widgets->view_sanctions->create_sanction_description);
    GtkTextIter startIter;
    GtkTextIter endIter;
    gtk_text_buffer_get_start_iter(textBuffer, &startIter);
    gtk_text_buffer_get_end_iter(textBuffer, &endIter);

    // check if the required fields have been correctly filled then SQL update: 0 if OK
    int returnCode = GTKCreateSanctionSubmitCheckRequiredField(
            gtk_text_buffer_get_text(textBuffer, &startIter, &endIter, FALSE)) ||
                     insertSanction(gtk_text_buffer_get_text(textBuffer, &startIter, &endIter, FALSE),
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

// Check if the required fields have been correctly filled when creating a sanction
int GTKCreateSanctionSubmitCheckRequiredField(char *textIter) {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_sanctions->create_sanction_name)) == 0) {
        fprintf(stderr, "Name empty!\n");
        returnCode = 1;
    }
    if (strlen(textIter) == 0) {
        fprintf(stderr, "Description empty!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Display the content of deliverables list view + update the request
void GTKListDeliverables() {
    // Display the deliverables list view
    gtk_stack_set_visible_child(widgets->view_deliverables->view_deliverables_stack,
                                widgets->view_deliverables->view_deliverables_fixed);

    // Flush the previous data from the tree store
    gtk_tree_store_clear(widgets->view_deliverables->deliverables_tree_store);

    char *deliverables, *result, *firstAddress, *buffer;
    int nbdeliverables = 0;
    listDeliverables(&deliverables); // deliverables = n * "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk;\n"
    firstAddress = deliverables;
    result = deliverables;           // result: address of the first character of the students string, used as a cursor
    GtkTreeIter iter;                // row pointer

    // Count the number of deliverables
    while ((result = strstr(result, ";\n"))) {
        nbdeliverables++;
        result++;                    // move the cursor behind ";" to avoid infinite loop
    }

    // Fill the tree view
    for (int i = 0; i < nbdeliverables; ++i) {
        // Create a new row, pointed by "iter"
        gtk_tree_store_append(widgets->view_deliverables->deliverables_tree_store, &iter, NULL);

        //ID
        result = strchr(deliverables, '|');         // result = position of the first | of the row
        size_t columnSize = result - deliverables;  // difference between the first character and the first '|'
        buffer = malloc(columnSize + 1);      // allocate memory for the string
        strncpy(buffer, deliverables, columnSize);  // copy column content into variable
        buffer[columnSize] = '\0';                  // add \0 on the last character
        // Fill the first column (0) of the new line the with the ID
        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 0, atoi(buffer), -1);
        deliverables += columnSize + 1;             // move the cursor behind the pipe to the next column
        free(buffer);                               // liberating memory for next column

        // DUE_DATE
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 1, buffer, -1);
        deliverables += columnSize + 1;
        free(buffer);

        // SUBJECT
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 2, buffer, -1);
        deliverables += columnSize + 1;
        free(buffer);

        // AUDIO_RECORD
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
        free(buffer);

        // VIDEO_RECORD
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
        free(buffer);

        // BAD_CODE
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
        free(buffer);

        // DELIVERABLE_FILE
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
        free(buffer);

        // STATUS
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 7, buffer, -1);
        deliverables += columnSize + 1;
        free(buffer);

        // STUDENT
        result = strchr(deliverables, '|');
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 8, buffer, -1);
        deliverables += columnSize + 1;
        free(buffer);

        // STUDENT_FK
        result = strstr(deliverables, ";\n");
        columnSize = result - deliverables;
        buffer = malloc(columnSize + 1);
        strncpy(buffer, deliverables, columnSize);
        buffer[columnSize] = '\0';
        gtk_tree_store_set(widgets->view_deliverables->deliverables_tree_store, &iter, 9, atoi(buffer), -1);
        deliverables += columnSize + 2; // +2 for ";\n": move to the next row
        free(buffer);
    }
    free(firstAddress);
}

// Display the content of deliverable edition view
void GTKEditDeliverables(int id) {
    // Display the deliverable edition view
    gtk_stack_set_visible_child(widgets->view_deliverables->view_deliverables_stack,
                                widgets->view_deliverables->edit_deliverable_fixed);

    char *due_date, *subject, *audio_record, *video_record, *bad_code, *deliverable_file, *status, *student, *student_fk, *sanction_name, *sanction_description, idBuffer[6];
    itoa(id, idBuffer, 10);

    // Fill the variables with result from single deliverable database request
    GTKDelivreablesGetData(id, &due_date, &subject, &audio_record, &video_record, &bad_code, &deliverable_file, &status,
                           &student, &student_fk, &sanction_name, &sanction_description);

    // Set current due date in calendar
    GTKEditDeliverableSetDueDate(due_date);

    // Fill status combo list
    fillStatusComboList(widgets->view_deliverables->edit_deliverable_status, status);

    // Fill entries with request return
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

    // AUDIO FILE
    // Display the download button if there is a file to download
    if (audio_record != NULL && !strlen(audio_record)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), TRUE);
        // Path in tooltip text will be used to download
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download),
                                    audio_record);
    }

    // VIDEO FILE
    if (video_record != NULL && !strlen(video_record)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), TRUE);
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download),
                                    video_record);
    }

    // DELIVERABLE FILE
    if (deliverable_file != NULL && !strlen(deliverable_file)) {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                               FALSE);
    } else {
        gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                               TRUE);
        gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                                    deliverable_file);
    }

    // BAD CODE FILE
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

// Fill the variables in parameters with result from single deliverable database request
void GTKDelivreablesGetData(int id, char **due_date, char **subject, char **audio_record, char **video_record,
                            char **bad_code, char **deliverable_file, char **status, char **student, char **student_fk,
                            char **sanction_name, char **sanction_description) {
    char *deliverableData, *firstAdress;
    size_t columnSize;
    getDeliverable(&deliverableData, id); //deliverableData = "id|due_date|subject|audio_record|video_record|bad_code|deliverable_file|status|student(first_name + last_name)|student_fk|sanction_name|sanction_description;\n"
    firstAdress = deliverableData;

    //ID
    deliverableData += strchr(deliverableData, '|') - deliverableData + 1; // id already known, moving to next column

    //DUE_DATE
    columnSize = strchr(deliverableData, '|') - deliverableData;            // difference between the first character and the next '|'
    *due_date = malloc(columnSize + 1);                                     // allocate memory for the string
    strncpy(*due_date, deliverableData, columnSize);                        // copy column content into parameter variable
    (*due_date)[columnSize] = '\0';                                         // add \0 on the last character
    deliverableData += columnSize + 1;                                      // move the cursor behind the pipe to the next column

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

// Set due date when editing deliverable (from string to calendar)
void GTKEditDeliverableSetDueDate(char *date) {
    // string date format = YYYY/MM/DD
    guint day, month, year, columnSize;
    char *buffer;

    //YEAR
    columnSize = strchr(date, '/') - date;  // difference between the first character and the first '/'
    buffer = malloc(columnSize + 1);        // allocate memory for the string
    strncpy(buffer, date, columnSize);      // copy column content into variable
    buffer[columnSize] = '\0';              // add \0 on the last character
    year = atoi(buffer);                    // from string to integer
    date += columnSize + 1;                 // move the cursor behind the /
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
    strcpy(buffer, date);
    day = atoi(buffer);
    free(buffer);

    // Set calendar to correct date
    gtk_calendar_select_month(widgets->view_deliverables->edit_deliverable_due_date, month - 1, year); // -1 : months from 0 to 11
    gtk_calendar_select_day(widgets->view_deliverables->edit_deliverable_due_date, day);
}

// Updated deliverable from edition view data
void GTKEditDelivreablesSubmit() {
    guint day, month, year;
    char dateBuffer[9];

    // Get date from calendar
    // year: decimal number (e.g. 2011) or NULL.
    // month: number (between 0 and 11) or NULL.
    // day: number (between 1 and 31) or NULL.
    gtk_calendar_get_date(widgets->view_deliverables->edit_deliverable_due_date, &year, &month, &day);
    sprintf(dateBuffer, "%d/%d/%d", year, month + 1, day);

    // check if the required fields have been correctly filled then SQL update: 0 if OK
    int returnCode = GTKEditDeliverableSubmitCheckRequiredField() ||
                     updateDeliverable(atoi(gtk_label_get_text(widgets->view_deliverables->edit_deliverable_id)),
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

// Check if the required fields have been correctly filled when editing a deliverable
int GTKEditDeliverableSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_deliverables->edit_deliverable_subject)) == 0) {
        fprintf(stderr, "Subject empty!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Update audio file
int GTKDeliverableSetAudio(char *path) {
    // check the extension of the audio file
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
        fprintf(stderr, "Error while adding audio record: %s", path);
        return 1;
    }

    // set the download button visible
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), TRUE);
    // display the new file path in the tooltip (for the download)
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_audio_download), newPath);

    free(newPath);

    return 0;
}

// Update video file
int GTKDeliverableSetVideo(char *path) {
    // check the extension of the video file
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

    // set the download button visible
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), TRUE);
    // display the new file path in the tooltip (for the download)
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_video_download), newPath);

    free(newPath);

    return 0;
}

// Update bad code file
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

    // set the download button visible
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download), TRUE);
    // display the new file path in the tooltip (for the download)
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_bad_code_download), newPath);

    free(newPath);

    return 0;
}

// Update deliverable file
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

    // set the download button visible
    gtk_widget_set_visible(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download), TRUE);
    // display the new file path in the tooltip
    gtk_widget_set_tooltip_text(GTK_WIDGET(widgets->view_deliverables->edit_deliverable_deliverable_file_download),
                                newPath);
    free(newPath);
    return 0;
}

// Display deliverable creation view content
void GTKCreateDelivreables(int student_fk) {
    // Go to the deliverable stack + display deliverable creation view
    gtk_stack_set_visible_child_name(widgets->menu_stack, "view_deliverables");
    gtk_stack_set_visible_child(widgets->view_deliverables->view_deliverables_stack,
                                widgets->view_deliverables->create_deliverable_fixed);

    char *sanction_name, *sanction_description, *student, *student_fk_c;

    // Fill the parameters with results from database request
    GTKSanctionGetDataStudentId(student_fk, &sanction_name, &sanction_description, &student, &student_fk_c);

    // Fill status combo list
    fillStatusComboList(widgets->view_deliverables->create_deliverable_status, "To do");

    // Fill the content of the entries
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

// Create deliverable
void GTKCreateDelivreablesSubmit() {
    guint day, month, year;
    char dateBuffer[9];

    // get the date from calendar
    gtk_calendar_get_date(widgets->view_deliverables->create_deliverable_due_date, &year, &month, &day);
    sprintf(dateBuffer, "%d/%d/%d", year, month + 1, day); // month + 1 to get month number (0 to 11)

    // get file paths
    char *audioPath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_audio));
    char *videoPath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_video));
    char *bcPath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_bad_code));
    char *deliverablePath = gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_deliverables->create_deliverable_deliverable_file));

    // check extensions
    int audioGood = checkAudioExtension(audioPath);
    int videoGood = checkVideoExtension(videoPath);

    // Check if the required fields have been correctly filled then SQL update: 0 if OK
    int returnCode = GTKCreateDeliverableSubmitCheckRequiredField() ||
                     insertDeliverable(dateBuffer,
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

// Check if the subject is filled when creating a deliverable
int GTKCreateDeliverableSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_deliverables->create_deliverable_subject)) == 0) {
        fprintf(stderr, "Subject empty!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Display the content of the user view
void GTKViewUser() {
    // Display the user view
    gtk_stack_set_visible_child(widgets->view_user->view_user_stack, widgets->view_user->view_user_fixed);

    char *email, *first_name, *last_name, *photo, *birthdate, *emailURI;
    int id;

    // Fill the parameters with result from the user database request
    GTKUserGetData(&id, &email, &first_name, &last_name, &photo, &birthdate);

    // Create "mailto" link from user email
    emailURI = malloc(strlen(email) + 8); // +8 for mailto:\0
    strcat(strcpy(emailURI, "mailto:"), email);

    // Display a scaled version of the user image
    GTKUserImage(photo);

    // Fill entries with request return
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

// Display the content of the user edition view
void GTKEditUser() {
    // Display the user edition view
    gtk_stack_set_visible_child(widgets->view_user->view_user_stack, widgets->view_user->edit_user_fixed);

    char *email, *first_name, *last_name, *photo, *birthdate;
    int id;

    // Fill variables with result from the user database request
    GTKUserGetData(&id, &email, &first_name, &last_name, &photo, &birthdate);

    // Fill entries with request return
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

// Update user data
void GTKEditUserSubmit() {
    // check if the required fields have been correctly filled then SQL update: 0 if OK
    int returnCode = GTKEditUserSubmitCheckRequiredField() ||
                     updateUser(1, gtk_entry_get_text(widgets->view_user->edit_user_email),
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

// Check if the required fields are filled when editing a user
int GTKEditUserSubmitCheckRequiredField() {
    int returnCode = 0;
    if (strlen(gtk_entry_get_text(widgets->view_user->edit_user_first_name)) == 0) {
        fprintf(stderr, "First name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_user->edit_user_last_name)) == 0) {
        fprintf(stderr, "Last name empty!\n");
        returnCode = 1;
    }
    if (strlen(gtk_entry_get_text(widgets->view_user->edit_user_email)) == 0) {
        fprintf(stderr, "Email empty!\n");
        returnCode = 1;
    }
    return returnCode;
}

// Fill the parameters with result from the user database request
void GTKUserGetData(int *id, char **email, char **first_name, char **last_name, char **photo, char **birthdate) {
    char *intBuffer, *data, *firstAdress;
    size_t columnSize;
    getUser(&data, 1); // data = "id|email|first_name|last_name|photo|birthdate;\n", ID = 1
    firstAdress = data;

    // ID
    columnSize = strchr(data, '|') - data;  // difference between the first character and the first '|'
    intBuffer = malloc(columnSize + 1);     // allocate memory for the string
    strncpy(intBuffer, data, columnSize);   // copy column content into buffer storing atoi
    intBuffer[columnSize] = '\0';           // add \0 on the last character
    *id = atoi(intBuffer);                  // update parameter variable with content
    data += columnSize + 1;                 // move the cursor behind the pipe to the next column

    columnSize = strchr(data, '|') - data;  // difference between the cursor position and the next '|'
    *email = malloc(columnSize + 1);        // allocate memory for the string
    strncpy(*email, data, columnSize);      // copy column content into parameter variable
    (*email)[columnSize] = '\0';            // add \0 on the last character
    data += columnSize + 1;                 // move the cursor behind the pipe to the next column

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

// Display a scaled version of the user image
void GTKUserImage(char *path) {
    // Reset the image to be empty
    gtk_image_clear(widgets->view_user->view_user_image);

    // Structure describing an image
    GdkPixbuf *pixbuf;

    // Load image from file
    if ((pixbuf = gdk_pixbuf_new_from_file(path, NULL)) == NULL) {
        fprintf(stderr, "Error while loading user profile picture\n");
    } else {
        printf("Loaded!\n");
        int width = gdk_pixbuf_get_width(pixbuf);
        int height = gdk_pixbuf_get_height(pixbuf);
        double ratio = (250. / width);
        printf("width: %d, height: %d, ratio: %lf\n", width, height, ratio);

        // Create a new scaled copy of original image, forcing width to 250
        gtk_image_set_from_pixbuf(widgets->view_user->view_user_image,
                                  gdk_pixbuf_scale_simple(pixbuf, floor(width * ratio), floor(height * ratio),
                                                          GDK_INTERP_BILINEAR));
    }
}

// Insert an image in the user database and creates the directories/file
int GTKUserSetImage(char *path) {
    if (!checkImageExtension(path))
        return 1;

    // Insert image in the database and creates the directories/file
    return insertTableImage("user", 1, path);
}

// Fill the setting view with data from the global variables
void GTKViewSettings() {
    // Dark theme
    gtk_switch_set_active(widgets->view_settings->view_settings_switch_theme_button, darkTheme ? TRUE : FALSE);

    // Console
    gtk_switch_set_active(widgets->view_settings->view_settings_show_terminal_button, showConsole ? TRUE : FALSE);

    // Storage folder
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser));
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser),
                                        storageFolder);

    // Database file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser), dbname);

    // Glade file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser), gladeFile);

    // Default theme file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser),
                                  defaultThemePath);

    // Dark theme file
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser),
                                  darkThemePath);
}

// Update the global variables with the content of the settings edition view
void GTKViewSettingsSubmit() {

    // STORAGE
    storageFolder = realloc(storageFolder,
                            strlen(gtk_file_chooser_get_filename(
                                    GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser)) + 1));
    strcpy(storageFolder,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_storage_folder_chooser)));

    // DATABASE
    dbname = realloc(dbname,
                     strlen(gtk_file_chooser_get_filename(
                             GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser)) + 1));
    strcpy(dbname,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_database_file_chooser)));

    // GLADE
    gladeFile = realloc(gladeFile,
                        strlen(gtk_file_chooser_get_filename(
                                GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser))) + 1);
    strcpy(gladeFile,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_glade_file_chooser)));

    // DEFAULT THEME
    defaultThemePath = realloc(defaultThemePath,
                               strlen(gtk_file_chooser_get_filename(
                                       GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser))) +
                               1);
    strcpy(defaultThemePath, gtk_file_chooser_get_filename(
            GTK_FILE_CHOOSER(widgets->view_settings->settings_default_theme_file_chooser)));

    // DARK THEME
    darkThemePath = realloc(darkThemePath,
                            strlen(gtk_file_chooser_get_filename(
                                    GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser)) + 1));
    strcpy(darkThemePath,
           gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widgets->view_settings->settings_dark_theme_file_chooser)));

    // Write new configuration file
    writeConf();
}

// Load the theme depending on the darkTheme global variable value
void GTKSetTheme() {
    GtkCssProvider *pCssProvider = NULL;    // pointer to a GtkCssProvider structure
    pCssProvider = gtk_css_provider_new();  // create a new GtkCssProvider
    GError *error = NULL;

    // add a style provider to the default screen for the default display
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(pCssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    // decreases the reference count of the CssProvider
    g_object_unref(pCssProvider);

    if (darkTheme) {  // 1 if switch button activated
        // load the dark theme path into the css_provider
        if (!gtk_css_provider_load_from_path(pCssProvider, darkThemePath, &error)) {
            fprintf(stderr, "%s\n", error->message);
            exit(error->code);
        }
    } else {         // 0 if switch button not activated
        // load the default theme path into the css_provider
        if (!gtk_css_provider_load_from_path(pCssProvider, defaultThemePath, &error)) {
            fprintf(stderr, "%s\n", error->message);
            exit(error->code);
        }
    }
}

// Display or hide the console window, depending on the showConsole global variable value
void GTKShowConsole() {
    // Get the window handle used by the console
    HWND hWnd = GetConsoleWindow();
    if (showConsole) {  // 1 if switch button activated
        ShowWindow(hWnd, SW_SHOW);
    } else {            // 0 if switch button not activated
        ShowWindow(hWnd, SW_HIDE);
    }
}

// Fill the widgets structures with the app widgets
void connectWidgets() {

    // Connect general widgets
    widgets->window_dashboard = GTK_WIDGET(gtk_builder_get_object(builder, "window_dashboard"));
    widgets->menu_stack = GTK_STACK(gtk_builder_get_object(builder, "menu_stack"));
    widgets->gtk_fixed = GTK_FIXED(gtk_builder_get_object(builder, "gtk_fixed"));
    widgets->menu_stack_switcher = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "menu_stack_switcher"));
    widgets->search_entry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "search_entry"));

    // Connect view_students
    widgets->view_students = g_slice_new(Students); // Allocate memory for the structure
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

    // Connect view_classes
    widgets->view_classes = g_slice_new(Classes); // Allocate memory for the structure
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

    // Connect view_sanctions
    widgets->view_sanctions = g_slice_new(Sanctions); // Allocate memory for the structure
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

    // Connect view_deliverables
    widgets->view_deliverables = g_slice_new(Deliverables); // Allocate memory for the structure
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

    // Connect view_user
    widgets->view_user = g_slice_new(User); // Allocate memory for the structure
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

    // Connect view_settings
    widgets->view_settings = g_slice_new(Settings); // Allocate memory for the structure
    widgets->view_settings->view_settings_switch_theme_button = GTK_SWITCH(
            gtk_builder_get_object(builder, "view_settings_switch_theme_button"));
    widgets->view_settings->view_settings_show_terminal_button = GTK_SWITCH(
            gtk_builder_get_object(builder, "view_settings_show_terminal_button"));
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

// Hide or display search bar
void setSearchEntry(gboolean visible, GtkTreeView *treeView, const char *placeholder) {
    if (visible) { // if TRUE
        gtk_fixed_move(widgets->gtk_fixed, GTK_WIDGET(widgets->menu_stack_switcher), 240,
                       161); // moving the menu to the right
        gtk_widget_set_visible(GTK_WIDGET(widgets->search_entry), TRUE); // set the search bar visible
        gtk_tree_view_set_search_entry(treeView, GTK_ENTRY(widgets->search_entry)); // set the search
        gtk_entry_set_text(GTK_ENTRY(widgets->search_entry), ""); // flush the text in the search bar
        gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->search_entry), placeholder); // fill it with placeholder text
    } else { // if FALSE
        gtk_widget_set_visible(GTK_WIDGET(widgets->search_entry), FALSE); // hide the search bar
        gtk_fixed_move(widgets->gtk_fixed, GTK_WIDGET(widgets->menu_stack_switcher), 130,
                       161); // place the search bar to the correct place
    }
}

// Open a dialog window to locally download the file
void GTKSaveFile(char *path) {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE; // save mode
    gint res;

    // Create a new GtkFileChooserDialog with two buttons
    dialog = gtk_file_chooser_dialog_new("Save File",
                                         GTK_WINDOW(widgets->window_dashboard),
                                         action,
                                         "Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
    // Define file chooser from dialog window
    chooser = GTK_FILE_CHOOSER (dialog);

    // Confirmation mode activated: confirmation dialog if file name already exist
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);

    // Set path as the filename for the file chooser
    gtk_file_chooser_set_filename(chooser, path);

    // Dialog become main focus until user clicks "save"
    res = gtk_dialog_run(GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(chooser); // get name of the selected file in the file selector
        copyFile(path, filename); // bit by bit copy of file
        g_free(filename);
    }

    // Destroy the GtkFileChooserDialog
    gtk_widget_destroy(dialog);
}

// Open the app and run the program
void dashboardGTK(int *argc, char ***argv) {
    // Allocate memory for the widget structure, holding all the app widgets
    widgets = g_slice_new(App_widgets);

    // Library initialisation
    gtk_init(argc, argv);

    // Build the interface from the glade file
    builder = gtk_builder_new_from_file(gladeFile);

    // Fill the widgets structures with the app widgets
    connectWidgets();

    // Connect the signals to the builder
    g_signal_connect(widgets->window_dashboard, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    // Load the theme depending on the darkTheme global variable value
    GTKSetTheme();

    // Show all the dashboard and its children
    gtk_widget_show_all(widgets->window_dashboard);

    // Run the main loop/program
    gtk_main();

    // Free widgets structures memory
    g_slice_free(Students, widgets->view_students);
    g_slice_free(Classes, widgets->view_classes);
    g_slice_free(Sanctions, widgets->view_sanctions);
    g_slice_free(Deliverables, widgets->view_deliverables);
    g_slice_free(User, widgets->view_user);
    g_slice_free(Settings, widgets->view_settings);
    g_slice_free(App_widgets, widgets);
}
