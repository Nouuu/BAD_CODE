#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <windows.h>

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
int showConsole = -1;

int main(int argc, char **argv) {

    // Read the configuration file and fill the global variables
    readConf();

    // Display or hide the console window, depending on the showConsole config variable
    HWND hWnd = GetConsoleWindow();
    if (showConsole) {
        ShowWindow(hWnd, SW_SHOW);
        ShowWindow(hWnd, SW_MINIMIZE);
    } else {
        ShowWindow(hWnd, SW_HIDE);
    }

    // Open the app and run the main loop / program
    dashboardGTK(&argc, &argv);

    free(dbname);
    free(gladeFile);
    free(storageFolder);
    free(darkThemePath);
    free(defaultThemePath);

    printf("Exit program");
    return EXIT_SUCCESS;
}