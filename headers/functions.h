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

int copyFile(char *src, char *dest);

int removeDirectory(char *src);

#endif //BAD_CODE_FUNCTIONS_H
