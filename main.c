#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sqlite3.h>
#include <gtk-3.0/gtk/gtk.h>

#include "headers/functions.h"
#include "headers/sqliteFunctions.h"
#include "headers/gtkFunctions.h"

GtkBuilder *builder;
char *dbname = "database.db";
char *gladeFile = "test.glade";

int main(int argc, char **argv) {

    startGTK2(&argc, &argv, gladeFile);




    printf("Exit program");
    return EXIT_SUCCESS;
}