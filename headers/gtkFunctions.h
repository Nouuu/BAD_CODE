//
// Created by MaleWhere on 15/01/2020.
//

#ifndef BAD_CODE_GTKFUNCTIONS_H
#define BAD_CODE_GTKFUNCTIONS_H

// link builder to the one in main.c
extern GtkBuilder *builder;

//Event listener
G_MODULE_EXPORT void on_Bad_Code_destroy(GtkWidget *widget, gpointer user_data);

G_MODULE_EXPORT void on_button_name_clicked(GtkWidget *widget, gpointer user_data);

void startGTK(int *argc, char ***argv) {
    GtkWidget *window;

//Init GTK
    gtk_init(argc, argv);

    //Create gtk window from glade file
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "test.glade", NULL) == 0) {
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


#endif //BAD_CODE_GTKFUNCTIONS_H
