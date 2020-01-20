#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <gtk-3.0/gtk/gtk.h>

#include "headers/sqliteFunctions.h"
#include "headers/gtkFunctions.h"

GtkBuilder *builder;
char *gladeFile = "test.glade";

int main(int argc, char **argv) {

    startGTK(&argc, &argv? gladeFile);

    printf("Exit program");

    return EXIT_SUCCESS;
}