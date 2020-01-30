#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

#include "headers/gtkFunctions.h"

GtkBuilder *builder;
char *dbname = "..\\bdd\\database.db";
char *gladeFile = "test.glade";

int main(int argc, char **argv) {
    dashboardGTK(&argc, &argv);

    printf("Exit program");
    return EXIT_SUCCESS;
}