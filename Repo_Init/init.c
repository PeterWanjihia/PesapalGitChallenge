#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "init.h"

// Function to create directories recursively
int create_directory(const char *dir_path) {
    char temp[256];
    char *ptr = NULL;
    size_t len;

    strncpy(temp, dir_path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';
    len = strlen(temp);

    if (temp[len - 1] == '/') {
        temp[len - 1] = '\0';
    }

    for (ptr = temp + 1; *ptr; ptr++) {
        if (*ptr == '/') {
            *ptr = '\0';
            if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
                perror("Error creating directory");
                return -1;
            }
            *ptr = '/';
        }
    }

    if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
        perror("Error creating directory");
        return -1;
    }

    return 0;
}

// Function to create a file with a given content
int create_file(const char *file_path, const char *content) {
    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("Error creating file");
        return -1;
    }
    if (content != NULL) {
        fprintf(file, "%s", content);
    }
    fclose(file);
    return 0;
}

// Function to generate a unique repository ID
void generate_repo_id(char *repo_id) {
    time_t t = time(NULL);
    snprintf(repo_id, 41, "%lx", (long)t);
}

// Function to create an initial commit placeholder
void create_initial_commit() {
    const char *commit_file = ".repo/refs/heads/main";
    if (create_file(commit_file, "") != 0) {
        fprintf(stderr, "Error: Failed to create initial commit file.\n");
        exit(EXIT_FAILURE);
    }
    printf("Initial commit placeholder created at '%s'.\n", commit_file);
}

// Function to create the config file
void create_config_file() {
    const char *config_path = ".repo/config";
    const char *config_content = "[core]\nrepositoryformatversion = 0\n";
    if (create_file(config_path, config_content) != 0) {
        fprintf(stderr, "Error: Failed to create config file.\n");
        exit(EXIT_FAILURE);
    }
    printf("Config file created at '%s'.\n", config_path);
}

// Function to initialize the repository
int initialize_repository() {
    const char *repo_dir = ".repo";
    const char *objects_dir = ".repo/objects";
    const char *refs_dir = ".repo/refs";
    const char *heads_dir = ".repo/refs/heads";

    struct stat st = {0};
    if (stat(repo_dir, &st) == 0) {
        fprintf(stderr, "Error: A repository already exists in this directory.\n");
        return EXIT_FAILURE;
    }

    if (create_directory(repo_dir) != 0 ||
        create_directory(objects_dir) != 0 ||
        create_directory(refs_dir) != 0 ||
        create_directory(heads_dir) != 0) {
        return EXIT_FAILURE;
    }

    char repo_id[41];
    generate_repo_id(repo_id);
    printf("Generated repository ID: %s\n", repo_id);

    create_initial_commit();
    create_config_file();

    printf("Repository successfully initialized.\n");
    return EXIT_SUCCESS;
}
