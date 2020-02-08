#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <wchar.h>

#include "headers/gtkFunctions.h"
#include "headers/functions.h"

GtkBuilder *builder;
char *dbname = NULL;
char *gladeFile = NULL;
char *storageFolder = NULL;
char *darkThemePath = NULL;
char *defaultThemePath = NULL;
char *configFile = "../config.ini";
int darkTheme = -1;

int main(int argc, char **argv) {

//    readConf();
//    dashboardGTK(&argc, &argv);

    wchar_t *string = convertUnicodeStringToUTF8String("héhé");
    printf("UNICODE:\t héhé\n");
    wprintf(L"UTF8 :\t\t %ls\n", string);

    free(dbname);
    free(gladeFile);
    free(storageFolder);
    free(darkThemePath);
    free(defaultThemePath);
    printf("Exit program");
    return EXIT_SUCCESS;
}