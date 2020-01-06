#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <gtk-3.0/gtk/gtk.h>

//#include "headers/sqlite/sqlite3.h"
#include "headers/sqliteFunctions.h"

void createGTKWindows(int *argc, char ***argv);
void OnDestroy();

int main(int argc, char **argv) {
//    checkVersion();
//    sqlite3 *db;
//
//    connectDB("tests.db", &db);
//    getAllStudents(db);
//    sqlite3_close(db);
    createGTKWindows(&argc, &argv);

    return EXIT_SUCCESS;
}

void createGTKWindows(int *argc, char ***argv) {

    GtkWidget *pWindow;
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;

    gtk_init(argc, argv);

    builder = gtk_builder_new();

    filename = g_build_filename("test.glade", NULL);

    gtk_builder_add_from_file(builder, filename, &error);
    g_free(filename);

    if (error)
    {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        exit(code);
    }

    pWindow = GTK_WIDGET(gtk_builder_get_object (builder, "Bad Code"));

//    g_signal_connect (G_OBJECT (pWindow), "destroy", G_CALLBACK(OnDestroy), NULL);
    g_signal_connect (G_OBJECT (pWindow), "destroy", (GCallback)gtk_main_quit, NULL);


    gtk_widget_show_all(pWindow);

    gtk_main();
}

void OnDestroy()
{
    /* Arret de la boucle evenementielle */
    gtk_main_quit();
}
