//
// Created by MaleWhere on 15/01/2020.
//

#ifndef BAD_CODE_GTKFUNCTIONS_H
#define BAD_CODE_GTKFUNCTIONS_H

// link builder to the one in main.c
extern GtkBuilder *builder;
extern char *dbname;

typedef struct {
    GtkWidget *rb_classes;
    GtkWidget *rb_students;
    GtkWidget *rb_sanctions;
    GtkWidget *rb_deliverables;
    GtkWidget *rb_settings;
    GtkWidget *lbl_button_state;
} app_widgets;

//Event listeners
G_MODULE_EXPORT void on_select_changed(GtkWidget *widget);

G_MODULE_EXPORT void on_menu_stack_visible_child_notify(GtkStack *stack, app_widgets *app_wdgts);

G_MODULE_EXPORT void
on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher, app_widgets *app_wdgts);

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

void GTKListStudents(GtkTreeStore *treeStore) {
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

    for (int i = 0; i < nbStudents; ++i) {
        gtk_tree_store_append(treeStore, &iter, NULL);

        //ID
        result = strchr(students, '|');
        size_t columnSize = result - students;
        char *buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 0, atoi(buffer), -1);
        students += columnSize + 1;

        //FIRST_NAME
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 1, buffer, -1);
        students += columnSize + 1;

        //LAST_NAME
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 2, buffer, -1);
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

        gtk_tree_store_set(treeStore, &iter, 4, buffer, -1);
        students += columnSize + 1;

        //BAD_CODE
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 5, atoi(buffer), -1);
        students += columnSize + 1;

        //NB_BOTTLES
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 6, atoi(buffer), -1);
        students += columnSize + 1;

        //CLASS
        free(buffer);
        result = strchr(students, '|');
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 7, buffer, -1);
        students += columnSize + 1;

        //CLASS_FK
        free(buffer);
        result = strstr(students, ";\n");
        columnSize = result - students;
        buffer = malloc(columnSize + 1);

        strncpy(buffer, students, columnSize);
        buffer[columnSize] = '\0';

        gtk_tree_store_set(treeStore, &iter, 8, atoi(buffer), -1);
        students += columnSize + 2;

    }

    free(firstAddress);
}

void startGTK2(int *argc, char ***argv, char *gladeFile) {

    GtkWidget *window;
    GtkWidget *fixed1;
    GtkWidget *view1;
    GtkTreeStore *treeStore;
    GtkTreeView *tv1;
    GtkTreeViewColumn *cx1;
    GtkTreeViewColumn *cx2;
    GtkTreeViewColumn *cx3;
    GtkTreeViewColumn *cx4;
    GtkTreeViewColumn *cx5;
    GtkTreeViewColumn *cx6;
    GtkTreeViewColumn *cx7;
    GtkTreeViewColumn *cx8;
    GtkTreeViewColumn *cx9;
    GtkTreeSelection *selection;
    GtkCellRenderer *cr1;
    GtkCellRenderer *cr2;
    GtkCellRenderer *cr3;
    GtkCellRenderer *cr4;
    GtkCellRenderer *cr5;
    GtkCellRenderer *cr6;
    GtkCellRenderer *cr7;
    GtkCellRenderer *cr8;
    GtkCellRenderer *cr9;

    gtk_init(argc, argv);

    builder = gtk_builder_new_from_file("listStudents.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    gtk_builder_connect_signals(builder, NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    view1 = GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
    treeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treeStore"));
    tv1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tv1"));
    cx1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx1"));
    cx2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx2"));
    cx3 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx3"));
    cx4 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx4"));
    cx5 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx5"));
    cx6 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx6"));
    cx7 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx7"));
    cx8 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx8"));
    cx9 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cx9"));
    cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr1"));
    cr2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr2"));
    cr3 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr3"));
    cr4 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr4"));
    cr5 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr5"));
    cr6 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr6"));
    cr7 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr7"));
    cr8 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr8"));
    cr9 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr9"));
    selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selection"));

    gtk_tree_view_column_add_attribute(cx1, cr1, "text", 0);
    gtk_tree_view_column_add_attribute(cx2, cr2, "text", 1);
    gtk_tree_view_column_add_attribute(cx3, cr3, "text", 2);
    gtk_tree_view_column_add_attribute(cx4, cr4, "pixbuf", 3);
    gtk_tree_view_column_add_attribute(cx5, cr5, "text", 4);
    gtk_tree_view_column_add_attribute(cx6, cr6, "text", 5);
    gtk_tree_view_column_add_attribute(cx7, cr7, "text", 6);
    gtk_tree_view_column_add_attribute(cx8, cr8, "text", 7);
    gtk_tree_view_column_add_attribute(cx9, cr9, "text", 8);

    GTKListStudents(treeStore);


    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv1));

    gtk_widget_show_all(window);
    gtk_main();

}

void on_menu_stack_visible_child_notify(GtkStack *stack, app_widgets *app_wdgts) {
    if (gtk_stack_get_visible_child_name(stack) != NULL)
        printf("%s\n", gtk_stack_get_visible_child_name(stack));
}

void on_menu_stack_switcher_visible_child_notify(GtkStackSwitcher *stackSwitcher, app_widgets *app_wdgts) {
    if (gtk_stack_switcher_get_stack(stackSwitcher) != NULL)
        printf("%s\n", gtk_stack_get_visible_child_name(gtk_stack_switcher_get_stack(stackSwitcher)));
}

void dashboardGTK(int *argc, char ***argv) {
    // Déclaration des variables
    GtkWidget *window_dashboard;
    app_widgets *widgets = g_slice_new(app_widgets);

    gtk_init(argc, argv);

    builder = gtk_builder_new_from_file("D:\\Projets\\BAD_CODE\\glade\\dashboard.glade"); // Chemin absolu à modifier

    window_dashboard = GTK_WIDGET(gtk_builder_get_object(builder, "window_dashboard"));
    g_signal_connect(window_dashboard, "destroy", G_CALLBACK(on_destroy), NULL);

    // get pointers to radio button widgets
    widgets->rb_classes = GTK_WIDGET(gtk_builder_get_object(builder, "rb_classes"));
    widgets->rb_students = GTK_WIDGET(gtk_builder_get_object(builder, "rb_students"));
    widgets->rb_sanctions = GTK_WIDGET(gtk_builder_get_object(builder, "rb_sanctions"));
    widgets->rb_deliverables = GTK_WIDGET(gtk_builder_get_object(builder, "rb_deliverables"));
    widgets->rb_settings = GTK_WIDGET(gtk_builder_get_object(builder, "rb_settings"));
    widgets->lbl_button_state = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_button_state"));

    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder); // Decreases the reference count of builder : if count = 0, memory is freed

    gtk_widget_show_all(window_dashboard);

    gtk_main();

    g_slice_free(app_widgets, widgets);
}


#endif //BAD_CODE_GTKFUNCTIONS_H
