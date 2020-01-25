//
// Created by MaleWhere on 15/01/2020.
//

#ifndef BAD_CODE_GTKFUNCTIONS_H
#define BAD_CODE_GTKFUNCTIONS_H

// link builder to the one in main.c
extern GtkBuilder *builder;
extern char *dbname;
//Event listener
G_MODULE_EXPORT void on_Bad_Code_destroy(GtkWidget *widget, gpointer user_data);

G_MODULE_EXPORT void on_button_name_clicked(GtkWidget *widget, gpointer user_data);

void on_destroy() {
    gtk_main_quit();
}

void startGTK(int *argc, char ***argv, char *gladeFile) {
    GtkWidget *window;

//Init GTK
    gtk_init(argc, argv);

    //Create gtk window from glade file
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, gladeFile, NULL) == 0) {
        fprintf(stderr, "Error: can't open glade file\n");
        exit(EXIT_FAILURE);
    }

    //Get main window by name
    window = GTK_WIDGET(gtk_builder_get_object(builder, "Bad_Code"));

    //Connect signals
    gtk_builder_connect_signals(builder, NULL);

    //Show main window
    gtk_widget_show(window);

    //GTK loop
    gtk_main();
    g_object_unref(G_OBJECT(builder));
}

void on_Bad_Code_destroy(GtkWidget *widget, gpointer user_data) {
    printf("Close window\n");
    gtk_main_quit();
}

void on_button_name_clicked(GtkWidget *widget, gpointer user_data) {
    int max_name_size = 20;
    const char *error_message = "Pas de nom";
    const char *name_message = "Votre nom est : ";
    const char *name;
    char *msg = NULL;

    GtkLabel *label_name = GTK_LABEL(gtk_builder_get_object(builder, "label_name"));
    GtkEntry *entry_name = GTK_ENTRY(gtk_builder_get_object(builder, "entry_name"));

    name = gtk_entry_get_text(entry_name);

    if (!strcmp(name, "")) {
        msg = malloc(strlen(error_message) * sizeof(char));
        strcpy(msg, error_message);
    } else {
        msg = malloc((strlen(name_message) + max_name_size) * sizeof(char));
        strcpy(msg, "Votre nom est : ");
        strncpy(msg, name, max_name_size);
        msg[strlen(name) >= max_name_size
            ? strlen(name_message) + max_name_size - 1
            : strlen(name_message) + strlen(name)] = '\0';
    }

    gtk_label_set_text(label_name, msg);
    free(msg);
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
    GtkTreeSelection *select;
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
    select = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "select"));

    gtk_tree_view_column_add_attribute(cx1, cr1, "text", 0);
    gtk_tree_view_column_add_attribute(cx2, cr2, "text", 1);
    gtk_tree_view_column_add_attribute(cx3, cr3, "text", 2);
    gtk_tree_view_column_add_attribute(cx4, cr4, "pixbuf", 3);
    gtk_tree_view_column_add_attribute(cx5, cr5, "text", 4);
    gtk_tree_view_column_add_attribute(cx6, cr6, "text", 5);
    gtk_tree_view_column_add_attribute(cx7, cr7, "text", 6);
    gtk_tree_view_column_add_attribute(cx8, cr8, "text", 7);
    gtk_tree_view_column_add_attribute(cx9, cr9, "text", 8);


    select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv1));

    gtk_widget_show_all(window);
    gtk_main();

}

#endif //BAD_CODE_GTKFUNCTIONS_H
