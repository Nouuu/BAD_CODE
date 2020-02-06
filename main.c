#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

#include "headers/gtkFunctions.h"
#include "headers/functions.h"

GtkBuilder *builder;
char *dbname = NULL;
char *gladeFile = NULL;
char *storageFolder = NULL;
char *configFile = "../config.ini";
int darkTheme = -1;

int main(int argc, char **argv) {

    readConf();
    dashboardGTK(&argc, &argv);

    free(dbname);
    free(gladeFile);
    free(storageFolder);
    printf("Exit program");
    return EXIT_SUCCESS;
}