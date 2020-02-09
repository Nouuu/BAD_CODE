#ifndef BAD_CODE_FUNCTIONS_H
#define BAD_CODE_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <dirent.h>

// Global variables from main.c / config file
extern char *dbname;
extern char *configFile;
extern char *storageFolder;
extern char *gladeFile;
extern char *darkThemePath;
extern char *defaultThemePath;
extern int darkTheme;

void createPath(char *path);

int copyFile(const char *src, const char *dest);

int removeDirectory(char *src);

char *get_filename_ext(char *path);

/**
 * @name checkImageExtension
 * @param path
 * @return 1 if it is image, 0 if not
 */
int checkImageExtension(char *path);

int checkVideoExtension(char *path);

int checkAudioExtension(char *path);

void readConf();

void writeConf();

#endif //BAD_CODE_FUNCTIONS_H
