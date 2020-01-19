#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <gtk-3.0/gtk/gtk.h>

#include "headers/functions.h"
#include "headers/sqliteFunctions.h"
#include "headers/gtkFunctions.h"

GtkBuilder *builder;
char *dbname = "database.db";

int main(int argc, char **argv) {

//    startGTK(&argc, &argv);

    char *class;
    listClass(dbname, &class);
    printf("Class : %s\n", class);

    printf("Exit program");

    return EXIT_SUCCESS;
}