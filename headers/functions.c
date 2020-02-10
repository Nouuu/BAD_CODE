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

int copyFile(const char *src, const char *dest) {
    //TODO use wfopen and convert char* to wchar* to fix UTF-8 encoding issue
    FILE *file = fopen(src, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    char *targetFolderBuffer = malloc(strlen(dest) + 1);
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
        strcmp(fileExt, "tiff") != 0 && strcmp(fileExt, "png") != 0 && strcmp(fileExt, "jfif") != 0)
        return 0;

    return 1;
}

int checkVideoExtension(char *path) {
    char *fileExt = get_filename_ext(path);

    if (strcmp(fileExt, "webm") != 0 && strcmp(fileExt, "mkv") != 0 && strcmp(fileExt, "flv") != 0 &&
        strcmp(fileExt, "vob") != 0 && strcmp(fileExt, "ogg") != 0 && strcmp(fileExt, "gif") != 0 &&
        strcmp(fileExt, "avi") != 0 && strcmp(fileExt, "TS") != 0 && strcmp(fileExt, "mov") != 0 &&
        strcmp(fileExt, "wmv") != 0 && strcmp(fileExt, "mp4") != 0 && strcmp(fileExt, "m4v") != 0 &&
        strcmp(fileExt, "3gp") != 0)
        return 0;

    return 1;
}

int checkAudioExtension(char *path) {
    char *fileExt = get_filename_ext(path);

    if (strcmp(fileExt, "3gp") != 0 && strcmp(fileExt, "aac") != 0 && strcmp(fileExt, "flac") != 0 &&
        strcmp(fileExt, "m4a") != 0 && strcmp(fileExt, "mp3") != 0 && strcmp(fileExt, "ogg") != 0 &&
        strcmp(fileExt, "opus") != 0 && strcmp(fileExt, "wav") != 0 && strcmp(fileExt, "wma") != 0 &&
        strcmp(fileExt, "webm") != 0 && strcmp(fileExt, "aiff") != 0 && strcmp(fileExt, "alac") != 0 &&
        strcmp(fileExt, "m4p") != 0)
        return 0;

    return 1;
}

void readConf() {
    printf("Reading conf file...\n%s\n", configFile);
    FILE *file = fopen(configFile, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file) / sizeof(char);
    fseek(file, 0, SEEK_SET);

    char *fileString = malloc((size + 1) * sizeof(char));

    if (fread(fileString, sizeof(char), size, file) != size) {
        fprintf(stderr, "Error while reading conf file ...\n");
        exit(EXIT_FAILURE);
    }

    //DATABASE
    char *P = strstr(fileString, "[DATABASE]");
    P = strchr(P, '\n') + 1;

    char buffer[255];
    sscanf(P, "path : %s\n", buffer);
    dbname = malloc(strlen(buffer) + 1);
    strcpy(dbname, buffer);

    //STORAGE
    P = strstr(fileString, "[STORAGE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "path : %s\n", buffer);
    storageFolder = malloc(strlen(buffer) + 1);
    strcpy(storageFolder, buffer);

    //GLADE
    P = strstr(fileString, "[GLADE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "path : %s\n", buffer);
    gladeFile = malloc(strlen(buffer) + 1);
    strcpy(gladeFile, buffer);

    //THEME
    P = strstr(fileString, "[THEME]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "default_path : %s\n", buffer);
    defaultThemePath = malloc(strlen(buffer) + 1);
    strcpy(defaultThemePath, buffer);

    P = strchr(P, '\n') + 1;
    sscanf(P, "dark_path : %s\n", buffer);
    darkThemePath = malloc(strlen(buffer) + 1);
    strcpy(darkThemePath, buffer);
    P = strchr(P, '\n') + 1;
    sscanf(P, "dark : %d\n", &darkTheme);

    //CONSOLE
    P = strstr(fileString, "[CONSOLE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "show : %d\n", &showConsole);


    fclose(file);
    free(fileString);
    printf("Done!\n");
}

void writeConf() {
    printf("Writing conf file...\n%s\n", configFile);
    FILE *file = fopen(configFile, "wb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(EXIT_FAILURE);
    }

    //DATABASE
    fprintf(file, "[DATABASE]\n");
    fprintf(file, "path : %s\n\n", dbname);

    //STORAGE
    fprintf(file, "[STORAGE]\n");
    fprintf(file, "path : %s\n\n", storageFolder);

    //GLADE
    fprintf(file, "[GLADE]\n");
    fprintf(file, "path : %s\n\n", gladeFile);

    //THEME
    fprintf(file, "[THEME]\n");
    fprintf(file, "default_path : %s\n", defaultThemePath);
    fprintf(file, "dark_path : %s\n", darkThemePath);
    fprintf(file, "dark : %d\n\n", darkTheme);

    //CONSOLE
    fprintf(file, "[CONSOLE]\n");
    fprintf(file, "show : %d\n", showConsole);


    fclose(file);
}
