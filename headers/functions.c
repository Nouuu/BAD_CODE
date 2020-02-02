//
// Created by MaleWhere on 30/01/2020.
//

#include "functions.h"

void createPath(char *path) {
    char buffer[20];
    int i;

    for (i = 0; i < strlen(path); ++i) {
        if (*(path + i) == '/') {
            strncpy(buffer, path, i);
            buffer[i] = '\0';
            mkdir(buffer);
        }
    }
    strncpy(buffer, path, i);
    buffer[i] = '\0';
    mkdir(buffer);
}

int copyFile(char *src, char *dest) {

    FILE *file = fopen(src, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open image\n");
        return 1;
    }

    char *targetFolderBuffer = malloc(strlen(dest));
    strcpy(targetFolderBuffer, dest);
    dirname(targetFolderBuffer);

    createPath(targetFolderBuffer);
    free(targetFolderBuffer);

    FILE *target = fopen(dest, "wb");
    if (target == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot open destination file\n");
        return 1;
    }

    int byte;
    while ((byte = fgetc(file)) != EOF) {
        fputc(byte, target);
    }

    fclose(file);
    fclose(target);
    return 0;
}

int removeDirectory(char *src) {
    struct dirent *dir;
    char *filePathBuffer = malloc(strlen(src) + 100);
    DIR *d = opendir(src);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                continue;
            strcat(strcat(strcpy(filePathBuffer, src), "/"), dir->d_name);
            printf("%s\n", filePathBuffer);
            remove(filePathBuffer);
        }
        closedir(d);
        rmdir(src);
    }

    free(filePathBuffer);
    return 0;
}

char *get_filename_ext(char *path) {
    char *bname = basename(path);

    char *dot = strrchr(bname, '.');
    if (!dot || dot == bname) return "";
    return dot + 1;
}

int checkImageExtension(char *path) {
    char *fileExt = get_filename_ext(path);

    if (strcmp(fileExt, "bmp") != 0 && strcmp(fileExt, "gif") != 0 && strcmp(fileExt, "ico") != 0 &&
        strcmp(fileExt, "jpeg") != 0 && strcmp(fileExt, "jpg") != 0 && strcmp(fileExt, "svg") != 0 &&
        strcmp(fileExt, "tiff") != 0)
        return 1;
    return 0;
}
