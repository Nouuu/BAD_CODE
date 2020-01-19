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

int main(int argc, char **argv) {

//    startGTK(&argc, &argv);
//    insertTableImage(dbname, "user", 21, "C:\\Users\\Unknow\\Downloads\\jojo.jfif");
    char *ch;
    for (int i = 0; i < 50; ++i) {

        listStudent(dbname, &ch);
        printf("%s\n", ch);
        free(ch);
        listClass(dbname, &ch);
        printf("%s\n", ch);
        free(ch);

    }
    printf("Exit program");

    return EXIT_SUCCESS;
}