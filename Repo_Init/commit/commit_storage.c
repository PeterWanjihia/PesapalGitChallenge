#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "commit_utils.h"

#define STORAGE_DIR ".repo/objects/storage_files"

// Function to create directories recursively
int create_directory(const char *dir_path) {
    char path[512];
    snprintf(path, sizeof(path), "%s", dir_path);

    // Check if directory exists, if not, create it
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        // Attempt to create the directory recursively
        if (mkdir(path, 0700) == -1) {
            if (errno == ENOENT) {
                // If the error is "No such file or directory", create parent dirs
                char *slash = strrchr(path, '/');
                if (slash != NULL) {
                    *slash = '\0';  // Temporarily terminate path at the last '/'
                    if (create_directory(path) == 0) {
                        return mkdir(path, 0700);  // Reattempt creation of the current directory
                    }
                }
            } else if (errno != EEXIST) {
                perror("Error creating directory");
                return -1;  // Error occurred, could not create directory
            }
        }
    }
    return 0;  // Directory created or already exists
}

void store_blob(const char *hash) {
    // Ensure we are dealing with a valid SHA-256 hash (length should be 64 characters)
    if (strlen(hash) != 64) {
        printf("Invalid hash length. Expected 64 characters for SHA-256, got %zu characters.\n", strlen(hash));
        return;
    }

    // Create the base storage directory
    if (create_directory(STORAGE_DIR) != 0) {
        printf("Error creating storage directory\n");
        return;
    }

    // Create the first level directory based on the first two characters of the hash
    char dir_path[512];
    snprintf(dir_path, sizeof(dir_path), "%s/%c%c", STORAGE_DIR, hash[0], hash[1]);

    if (create_directory(dir_path) != 0) {
        printf("Error creating subdirectory for hash\n");
        return;
    }

    // Calculate the length of the full object file path
    size_t path_length = strlen(dir_path) + strlen(hash + 2) + 1;  // +1 for '/'
    if (path_length >= sizeof(dir_path)) {
        printf("Warning: Object file path too long! It might get truncated.\n");
        return;
    }

    // Safely construct the object file path (skip the first 2 chars of the hash)
    char object_file_path[1024];  // Increased buffer size to 1024
    snprintf(object_file_path, sizeof(object_file_path), "%s/%s", dir_path, hash + 2);

    // Create the object file, but we are not writing content yet
    FILE *object_file = fopen(object_file_path, "w");
    if (!object_file) {
        perror("Error creating object file");
        return;
    }

    fclose(object_file);

    printf("Blob created at: %s\n", object_file_path);
}






int main() {
    // Get the list of file hashes
    char *hashes = get_staged_file_hashes();
    if (!hashes) {
        printf("Error: No hashes available\n");
        return 1;
    }

    // Iterate over each hash, call store_blob for each
    char *hash = strtok(hashes, " ");
    while (hash) {
        store_blob(hash);
        hash = strtok(NULL, " ");
    }

    return 0;
}
