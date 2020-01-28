//
// Created by MaleWhere on 15/01/2020.
//

#ifndef BAD_CODE_GTKFUNCTIONS_H
#define BAD_CODE_GTKFUNCTIONS_H

// link builder to the one in main.c
extern GtkBuilder *builder;
extern char *dbname;

typedef struct {
    GtkWidget *view_students_fixed;
    GtkWidget *view_students_view;
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
} View_students;

typedef struct {
    GtkWidget *view_classes_fixed;
    GtkWidget *view_classes_view;
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
} View_classes;

typedef struct {
    GtkWidget *view_sanctions_fixed;
    GtkWidget *view_sanctions_view;
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
} View_sanctions;

typedef struct {
    GtkWidget *window_dashboard;
    GtkStack *menu_stack;
    View_students *view_students;
    View_classes *view_classes;
    View_sanctions *view_sanctions;
} App_widgets;

App_widgets *widgets;

//Event listeners
G_MODULE_EXPORT void on_select_changed(GtkWidget *widget);

G_MODULE_EXPORT void on_menu_stack_visible_child_notify(GtkStack *stack);

G_MODULE_EXPORT void on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher);


void on_destroy() {
    gtk_main_quit();
}

void on_select_changed(GtkWidget *c) {
    gchar *cValue;
    guint iValue;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(c), &model, &iter) == FALSE)
        return;

    gtk_tree_model_get(model, &iter, 0, &iValue, -1);
    printf("id: %d, ", iValue);

    gtk_tree_model_get(model, &iter, 1, &cValue, -1);
    printf("first_name: %s, ", cValue);

    gtk_tree_model_get(model, &iter, 2, &cValue, -1);
    printf("last_name: %s, ", cValue);

    gtk_tree_model_get(model, &iter, 4, &cValue, -1);
    printf("email: %s, ", cValue);

    gtk_tree_model_get(model, &iter, 5, &iValue, -1);
    printf("bad_code: %d, ", iValue);

    gtk_tree_model_get(model, &iter, 6, &iValue, -1);
    printf("nb_bottles: %d, ", iValue);

    gtk_tree_model_get(model, &iter, 7, &cValue, -1);
    printf("class: %s, ", cValue);

    gtk_tree_model_get(model, &iter, 8, &iValue, -1);
    printf("class_fk: %d\n", iValue);


}

void GTKListStudents() {
    char *students, *result, *firstAddress;
    int nbStudents = 0;
    listStudents(dbname, &students);
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

        //TODO PHOTO
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);
//
//        strncpy(buffer, students, columnSize);
//        buffer[columnSize] = '\0';
//
//        gtk_tree_store_set(treeStore, &iter, 3, buffer, -1);
        students += columnSize + 1;

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

void GTKListClasses() {
    char *classes, *result, *firstAddress;
    int nbClasses = 0;
    listClasses(dbname, &classes);
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
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "Oui");
        } else {
            buffer = realloc(buffer, 3 * sizeof(char));
            strcpy(buffer, "Non");
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

void GTKListSanctions() {
    char *sanctions, *result, *firstAddress;
    int nbSanctions = 0;
    listSanctions(dbname, &sanctions);
    firstAddress = sanctions;
    result = sanctions;

    while ((result = strstr(result, ";\n"))) {
        nbSanctions++;
        result++;
    }

    GtkTreeIter iter;
    gtk_tree_store_clear(widgets->view_sanctions->sanctions_tree_store);

    for (int i = 0; i < nbSanctions; ++i) {
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

void on_menu_stack_visible_child_notify(GtkStack *stack) {
    if (gtk_stack_get_visible_child_name(stack) != NULL) {
        const gchar *menu = gtk_stack_get_visible_child_name(widgets->menu_stack);
        if (!strcmp(menu, "view_classes")) {
            printf("Classes !\n");
            GTKListClasses();
        } else if (!strcmp(menu, "view_students")) {
            printf("Students !\n");
            GTKListStudents();

        } else if (!strcmp(menu, "view_sanctions")) {
            printf("Sanctions !\n");
            GTKListSanctions();
        } else if (!strcmp(menu, "view_deliverables")) {
            printf("Deliverables !\n");

        } else if (!strcmp(menu, "view_settings")) {
            printf("Settings !\n");

        }
    }
}

void on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher) {
    if (gtk_stack_switcher_get_stack(stackSwitcher) != NULL)
        on_menu_stack_visible_child_notify(gtk_stack_switcher_get_stack(stackSwitcher));
}

void connectWidgets() {

    widgets->window_dashboard = GTK_WIDGET(gtk_builder_get_object(builder, "window_dashboard"));
    widgets->menu_stack = GTK_STACK(gtk_builder_get_object(builder, "menu_stack"));

    //Connect view_students
    widgets->view_students = g_slice_new(View_students);
    widgets->view_students->view_students_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_students"));
    widgets->view_students->view_students_view = GTK_WIDGET(gtk_builder_get_object(builder, "students_view"));
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
    widgets->view_classes = g_slice_new(View_classes);
    widgets->view_classes->view_classes_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_classes"));
    widgets->view_classes->view_classes_view = GTK_WIDGET(gtk_builder_get_object(builder, "classes_view"));
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
    widgets->view_sanctions = g_slice_new(View_sanctions);
    widgets->view_sanctions->view_sanctions_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "view_sanctions"));
    widgets->view_sanctions->view_sanctions_view = GTK_WIDGET(gtk_builder_get_object(builder, "sanctions_view"));
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
}


void dashboardGTK(int *argc, char ***argv) {
    // Déclaration des variables
    widgets = g_slice_new(App_widgets);

    gtk_init(argc, argv);

    builder = gtk_builder_new_from_file("..\\glade\\dashboard.glade"); // Chemin absolu à modifier

    connectWidgets();

    g_signal_connect(widgets->window_dashboard, "destroy", G_CALLBACK(on_destroy), NULL);

    gtk_builder_connect_signals(builder, NULL);
//    g_object_unref(builder); // Decreases the reference count of builder : if count = 0, memory is freed

    gtk_widget_show_all(widgets->window_dashboard);

    gtk_main();

    g_slice_free(View_students, widgets->view_students);
    g_slice_free(View_classes, widgets->view_classes);
    g_slice_free(View_sanctions, widgets->view_sanctions);
    g_slice_free(App_widgets, widgets);
}

#endif //BAD_CODE_GTKFUNCTIONS_H
