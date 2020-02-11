#include "functions.h"

// Creating directories with mkdir
void createPath(char *path) {
    char buffer[60];
    int i;

    // mkdir: creates the first directory that doesn't exist
    // Every passage in the loop: adding a new part of the path to create the folder
    for (i = 0; i < strlen(path); ++i) {
        if (*(path + i) == '/') {
            strncpy(buffer, path, i);
            buffer[i] = '\0';
            mkdir(buffer);
        }
    }

    // Last folder of the path: often no finishing "/"
    // Once we've reached the end of the string, need a last mkdir
    strncpy(buffer, path, i);
    buffer[i] = '\0';
    mkdir(buffer);
}

// Bit by bit copy of a file
int copyFile(const char *src, const char *dest) {

    // Extraction the destination folder path and creation of the directories
    char *targetFolderBuffer = malloc(strlen(dest) + 1); // +1 for '\0'
    strcpy(targetFolderBuffer, dest);
    dirname(targetFolderBuffer);
    createPath(targetFolderBuffer);
    free(targetFolderBuffer);

    // Opening the source file
    // TODO: use wfopen and convert char* to wchar* to fix UTF-8 encoding issue
    FILE *file = fopen(src, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open source file\n");
        return 1;
    }

    // Creation/opening of the destination file
    FILE *target = fopen(dest, "wb");
    if (target == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot open destination file\n");
        return 1;
    }

    // Bit by bit copy of source file to destination file
    int byte;
    while ((byte = fgetc(file)) != EOF) {
        fputc(byte, target);
    }

    fclose(file);
    fclose(target);
    return 0;
}

wchar_t *convertUnicodeStringToUTF8String(char *string) {
    //mbstowcs_s
    wchar_t *newString = malloc(strlen(string) + 1);
    if (mbstowcs(newString, string, strlen(string) + 1) != strlen(string))
        fprintf(stderr, "Error while converting string\n");
    return newString;
}

// Removing a student folder
int removeDirectory(char *src) {
    struct dirent *dir;
    char *filePathBuffer = malloc(strlen(src) + 100); // folder path + file name (100 characters max)
    DIR *d = opendir(src);

    if (d) {
        // Removing every file in the folder
        while ((dir = readdir(d)) != NULL) { // Reading the first file, if NULL: no more file to read
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) // check if not "." or ".."
                continue;
            strcat(strcat(strcpy(filePathBuffer, src), "/"), dir->d_name); // folder path + / + file name
            printf("%s\n", filePathBuffer);
            remove(filePathBuffer); // Removing the file from the memory
        }
        closedir(d);

        // Removing the folder once it's empty
        rmdir(src);
    }

    free(filePathBuffer);
    return 0;
}

// Get the extension of a file
char *get_filename_ext(char *path) {
    char *bname = basename(path); // getting last part of the path (filename.extension)
    char *dot = strrchr(bname, '.'); // searching the last occurrence of '.'
    if (!dot || dot == bname) return ""; // if no '.', then no extension then return empty string
    return dot + 1; // +1 to get 'ext' instead of '.ext'
}

// Get the extension of an image file and checks if the format is ok
// Returns 1 if format is OK, 0 if not
int checkImageExtension(char *path) {
    char *fileExt = get_filename_ext(path);
    if (strcmp(fileExt, "bmp") != 0 && strcmp(fileExt, "gif") != 0 && strcmp(fileExt, "ico") != 0 &&
        strcmp(fileExt, "jpeg") != 0 && strcmp(fileExt, "jpg") != 0 && strcmp(fileExt, "svg") != 0 &&
        strcmp(fileExt, "tiff") != 0 && strcmp(fileExt, "png") != 0 && strcmp(fileExt, "jfif") != 0)
        return 0;

    return 1;
}

// Get the extension of a video file and checks if the format is ok
// Returns 1 if format is OK, 0 if not
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

// Get the extension of an audio file and checks if the format is ok
// Returns 1 if format is OK, 0 if not
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

// Reads the configuration file and fill the global variables
void readConf() {
    printf("Reading conf file...\n%s\n", configFile); // declared in main.c

    // Opening the configuration file
    FILE *file = fopen(configFile, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    // Calculating the size of the file
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file) / sizeof(char);
    fseek(file, 0, SEEK_SET);

    // Copying the content of the file in a string
    char *fileString = malloc((size + 1) * sizeof(char));
    if (fread(fileString, sizeof(char), size, file) != size) {
        fprintf(stderr, "Error while reading conf file.\n");
        exit(EXIT_FAILURE);
    }

    // DATABASE
    char *P = strstr(fileString, "[DATABASE]"); // searching the position of the keyword
    P = strchr(P, '\n') + 1; // pointer on next line (looking for '\n', end of line, +1)
    char buffer[255];
    sscanf(P, "path : %s\n", buffer); // buffer = the string between "path : " and the next "\n"
    dbname = malloc(strlen(buffer) + 1);
    strcpy(dbname, buffer); // dbname = buffer

    // STORAGE
    P = strstr(fileString, "[STORAGE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "path : %s\n", buffer); // erasing the previous content and replacing it with the new one
    storageFolder = malloc(strlen(buffer) + 1);
    strcpy(storageFolder, buffer);

    // GLADE
    P = strstr(fileString, "[GLADE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "path : %s\n", buffer);
    gladeFile = malloc(strlen(buffer) + 1);
    strcpy(gladeFile, buffer);

    // THEME
    // Default
    P = strstr(fileString, "[THEME]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "default_path : %s\n", buffer);
    defaultThemePath = malloc(strlen(buffer) + 1);
    strcpy(defaultThemePath, buffer);

    // Dark theme
    P = strchr(P, '\n') + 1;
    sscanf(P, "dark_path : %s\n", buffer);
    darkThemePath = malloc(strlen(buffer) + 1);
    strcpy(darkThemePath, buffer);
    P = strchr(P, '\n') + 1;
    sscanf(P, "dark : %d\n", &darkTheme);

    //CONSOLE
    P = strstr(fileString, "[CONSOLE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "dark : %d\n", &showConsole);


    fclose(file);
    free(fileString);
    printf("Done!\n");
}

// Reads the global variables and rewrites the file accordingly
void writeConf() {
    printf("Writing conf file...\n%s\n", configFile);

    // Creating and opening a new file, erasing the previous one
    FILE *file = fopen(configFile, "wb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file.\n");
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