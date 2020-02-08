#include "functions.h"

// Function creating a path string
void createPath(char *path) {
    char buffer[60];
    int i;

    // mkdir: creates the first directory that doesn't exist
    // Go boucle sur chaque "/" pour créer l'arborescence
    for (i = 0; i < strlen(path); ++i) { // boucle sur tous les caractères
        if (*(path + i) == '/') { // vérification si c'est un slash
            strncpy(buffer, path, i);
            buffer[i] = '\0'; //strn : pas de \0
            mkdir(buffer);
        }
    }

    // le dernier dossier n'a pas de /, donc une fois qu'on est à la fin, il faut faire un dernier mkdir pour le dernier élément
    strncpy(buffer, path, i);
    buffer[i] = '\0';
    mkdir(buffer);
}

// copier un fichier
int copyFile(const char *src, const char *dest) {

    // destination : chemin de dossier + le nom du fichier
    char *targetFolderBuffer = malloc(strlen(dest) + 1); //+1 pour \0
    strcpy(targetFolderBuffer, dest);
    dirname(targetFolderBuffer); //extrait le nom du chemin, sans le fichier

    createPath(targetFolderBuffer); //création de l'arborescence basée du sur le path
    free(targetFolderBuffer); // free parce qu'on a malloc

    //TODO use wfopen and convert char* to wchar* to fix UTF-8 encoding issue
    FILE *file = fopen(src, "rb"); // ouvre de la source en lecture, bit
    if (file == NULL) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    // création/ouverture du fichier de destination
    FILE *target = fopen(dest, "wb");
    if (target == NULL) {
        fclose(file);
        fprintf(stderr, "Cannot open destination file\n");
        return 1;
    }

    //copie bit à bit du fichier source dans le fichier de destination
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
    if (mbstowcs(newString, string, strlen(string)+1) != strlen(string))
        fprintf(stderr, "Error while converting string\n");
    return newString;
}

// Suppression d'un étudiant : suppression des fichiers dans un dossier puis suppression du dossier
// Ne fonctionne pas si dans le dossier il y a un autre dossier : donc on a évité
int removeDirectory(char *src) {
    struct dirent *dir; //structure propre à la librairie dirent
    char *filePathBuffer = malloc(strlen(src) + 100); //malloc la taille du dossier + un fichier dont le nom est max de 100 caractères
    DIR *d = opendir(src); // pointeur de dossier
    if (d) {
        while ((dir = readdir(d)) != NULL) { // while car lecture du premier fichier, et si null c'est que plus rien
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) //vérification que ce ne sont pas les fichiers . et ..
                continue;
            strcat(strcat(strcpy(filePathBuffer, src), "/"), dir->d_name); //chemin du dossier + / + nom du fichier
            printf("%s\n", filePathBuffer);
            remove(filePathBuffer); // fonction qui supprime le fichier dans la mémoire
        }
        closedir(d);
        rmdir(src); //une fois que le dossier est vide, on supprime le dossier (syscall)
    }

    free(filePathBuffer);
    return 0;
}

// Get the extension of a file
char *get_filename_ext(char *path) {
    char *bname = basename(path); // basename : récupère le nom du fichier dans le path

    char *dot = strrchr(bname, '.'); // cherche la dernière occurence de '.' dans le nom du fichier
    if (!dot || dot == bname) return ""; // si pas de point, pas d'extension donc on retourne une chaîne de caractère vide
    return dot + 1; // +1 pour avoir "exe" et pas ".exe"
}

// Get the extension of an image and checks if the format is ok
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

// Lit le fichier de conf et l'assigne aux variables globales
void readConf() {
    printf("Reading conf file...\n%s\n", configFile); //configFIle déclaré dans le main.c
    FILE *file = fopen(configFile, "rb");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(EXIT_FAILURE);
    }

    // Calcul de la taille du fichier
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file) / sizeof(char);
    fseek(file, 0, SEEK_SET);

    // copie du contenu du fichier dans la chaîne de caractères
    char *fileString = malloc((size + 1) * sizeof(char));
    if (fread(fileString, sizeof(char), size, file) != size) {
        fprintf(stderr, "Error while reading conf file ...\n");
        exit(EXIT_FAILURE);
    }

    //DATABASE
    char *P = strstr(fileString, "[DATABASE]");
    P = strchr(P, '\n') + 1; // On cherche le saut de ligne + 1 pour être à la ligne
    char buffer[255];
    sscanf(P, "path : %s\n", buffer); //scan ce qu'il y a entre le "path" du fichier txt et le prochain \n
    dbname = malloc(strlen(buffer) + 1); // Il le colle dans le buffer
    strcpy(dbname, buffer);

    //STORAGE
    P = strstr(fileString, "[STORAGE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "path : %s\n", buffer);
    storageFolder = malloc(strlen(buffer)+1);
    strcpy(storageFolder, buffer);

    //GLADE
    P = strstr(fileString, "[GLADE]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "path : %s\n", buffer);
    gladeFile = malloc(strlen(buffer)+1);
    strcpy(gladeFile, buffer);

    //THEME
    P = strstr(fileString, "[THEME]");
    P = strchr(P, '\n') + 1;
    sscanf(P, "default_path : %s\n", buffer);
    defaultThemePath = malloc(strlen(buffer)+1);
    strcpy(defaultThemePath, buffer);

    P = strchr(P, '\n') + 1;
    sscanf(P, "dark_path : %s\n", buffer);
    darkThemePath = malloc(strlen(buffer)+1);
    strcpy(darkThemePath, buffer);
    P = strchr(P, '\n') + 1;
    sscanf(P, "dark : %d\n", &darkTheme);

    fclose(file);
    free(fileString);
    printf("Done!\n");
}

// Lit les variables globales et les écrit dans le fichier
// Ecrit l'intégralité du fichier, plus simple vu que le fichier est court
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
    fprintf(file, "dark : %d\n", darkTheme);

    fclose(file);
}
