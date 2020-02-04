//
// Created by Unknow on 18/01/2020.
//

#ifndef BAD_CODE_FUNCTIONS_H
#define BAD_CODE_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <dirent.h>

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

#endif //BAD_CODE_FUNCTIONS_H
