#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Function prototypes
int create_directory(const char *dir_path);
int create_file(const char *file_path, const char *content);
void generate_repo_id(char *repo_id);
void create_initial_commit();
void create_config_file();

int main() {
    const char *repo_dir = ".repo";  // Root directory of the repository
    const char *objects_dir = ".repo/objects"; // Objects directory (for file tracking)
    const char *refs_dir = ".repo/refs"; // References directory (for branches)
    const char *heads_dir = ".repo/refs/heads"; // Heads subdirectory (for branches)
    const char *config_file = ".repo/config"; // Configuration file

    // Step 1: Check if the repository already exists
    struct stat st = {0};
    if (stat(repo_dir, &st) == 0) {
        fprintf(stderr, "Error: A repository already exists in this directory.\n");
        return EXIT_FAILURE; // Exit if repository already exists
    }

    // Step 2: Create the main .repo directory
    if (create_directory(repo_dir) != 0) {
        return EXIT_FAILURE; // Fail if the directory creation fails
    }

    // Step 3: Create the objects, refs, and refs/heads subdirectories
    if (create_directory(objects_dir) != 0 ||
        create_directory(refs_dir) != 0 ||
        create_directory(heads_dir) != 0) {
        return EXIT_FAILURE; // Fail if the subdirectories cannot be created
    }

    // Step 4: Generate a unique repository ID
    char repo_id[41];  // SHA-1 hex is 40 chars + null terminator
    generate_repo_id(repo_id);
    printf("Generated repository ID: %s\n", repo_id);

    // Step 5: Create the initial commit placeholder (empty file)
    create_initial_commit();

    // Step 6: Create the config file (empty for now)
    create_config_file();

    // Success message
    printf("Repository successfully initialized.\n");
    return EXIT_SUCCESS;
}

// Function to create directories recursively (like `mkdir -p` in shell)
int create_directory(const char *dir_path) {
    char temp[256];
    char *ptr = NULL;
    size_t len;

    // Copy the directory path to temp, and make sure it is null-terminated
    strncpy(temp, dir_path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';
    len = strlen(temp);

    // If the path ends with a '/', remove it to ensure the path is correct
    if (temp[len - 1] == '/') {
        temp[len - 1] = '\0';
    }

    // Create all directories in the given path
    for (ptr = temp + 1; *ptr; ptr++) {
        if (*ptr == '/') {
            *ptr = '\0'; // Temporarily null-terminate for `mkdir`
            if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
                perror("Error creating directory");
                return -1;
            }
            *ptr = '/'; // Restore the path
        }
    }

    // Finally, create the last directory
    if (mkdir(temp, 0755) != 0 && errno != EEXIST) {
        perror("Error creating directory");
        return -1;
    }

    return 0; // Return 0 on success
}

// Function to create a file with a given content
int create_file(const char *file_path, const char *content) {
    FILE *file = fopen(file_path, "w");
    if (!file) {
        perror("Error creating file");
        return -1; // Return -1 if file creation fails
    }
    if (content != NULL) {
        fprintf(file, "%s", content);
    }
    fclose(file);
    return 0; // Return 0 on success
}

// Function to generate a unique repository ID (for simplicity, using time)
void generate_repo_id(char *repo_id) {
    time_t t = time(NULL);
    snprintf(repo_id, 41, "%lx", (long)t); // Using current timestamp as a unique ID
}

// Function to create an initial commit placeholder file
void create_initial_commit() {
    const char *commit_file = ".repo/refs/heads/main"; // The initial commit file
    if (create_file(commit_file, "") != 0) {
        fprintf(stderr, "Error: Failed to create initial commit file.\n");
        exit(EXIT_FAILURE); // Exit if we fail to create the initial commit
    }
    printf("Initial commit placeholder created at '%s'.\n", commit_file);
}

// Function to create the config file with basic default settings
void create_config_file() {
    const char *config_path = ".repo/config";
    const char *config_content = "[core]\nrepositoryformatversion = 0\n";
    if (create_file(config_path, config_content) != 0) {
        fprintf(stderr, "Error: Failed to create config file.\n");
        exit(EXIT_FAILURE); // Exit if we fail to create the config file
    }
    printf("Config file created at '%s'.\n", config_path);
}
