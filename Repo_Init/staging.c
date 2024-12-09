#include "staging.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// // Ensure that the staging directory and metadata file exist
// int ensure_staging_directory_exists(void) {
//     struct stat st = {0};

//     // Check if the .repo/staging directory exists
//     if (stat(".repo/staging", &st) == -1) {
//         if (mkdir(".repo/staging", 0755) == -1) {
//             perror("Error creating staging directory");
//             return -1;
//         }
//     }

//     // Check if the staged_files.txt exists; if not, create it
//     if (stat(STAGING_FILE_PATH, &st) == -1) {
//         FILE *file = fopen(STAGING_FILE_PATH, "w");
//         if (!file) {
//             perror("Error creating staging metadata file");
//             return -1;
//         }
//         fclose(file);
//     }

//     return 0;
// }


// Add a file to the staging area
int store_in_staging_area(const char *file_path, const char *file_hash) {
    if (file_path == NULL || file_hash == NULL) {
        fprintf(stderr, "Invalid file path or hash.\n");
        return -1;
    }

    // Check if the file is already staged
    char existing_hash[65];  // Store the hash of the existing file
    if (is_file_staged(file_path, existing_hash) == 1) {
        // If the file is already staged, check if the hash matches
        if (strcmp(existing_hash, file_hash) == 0) {
            printf("File '%s' is already staged with the same hash.\n", file_path);
            return 0;  // File is already staged, no need to add it again
        }
    }

    // Append the new file and hash to the staging metadata file
    FILE *file = fopen(STAGING_FILE_PATH, "a");
    if (!file) {
        perror("Error opening staging metadata file");
        return -1;
    }

    fprintf(file, "%s %s\n", file_path, file_hash);
    fclose(file);

    printf("File '%s' staged with hash: %s\n", file_path, file_hash);
    return 0;
}

// Remove a file from the staging area
int remove_from_staging_area(const char *file_path) {
    if (file_path == NULL) {
        fprintf(stderr, "Invalid file path.\n");
        return -1;
    }

    FILE *file = fopen(STAGING_FILE_PATH, "r");
    if (!file) {
        perror("Error opening staging metadata file");
        return -1;
    }

    // Read all lines and copy them into a temporary file, excluding the line to remove
    FILE *temp_file = fopen(".repo/staging/temp.txt", "w");
    if (!temp_file) {
        perror("Error creating temporary staging file");
        fclose(file);
        return -1;
    }

    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, file_path, strlen(file_path)) == 0) {
            found = 1;  // We found the file to remove
        } else {
            fputs(line, temp_file);  // Copy the line to the temp file
        }
    }

    fclose(file);
    fclose(temp_file);

    // If the file wasn't found, return an error
    if (!found) {
        fprintf(stderr, "File '%s' not found in staging area.\n", file_path);
        return -1;
    }

    // Replace the original staging file with the temporary one
    if (rename(".repo/staging/temp.txt", STAGING_FILE_PATH) != 0) {
        perror("Error renaming temporary staging file");
        return -1;
    }

    printf("File '%s' removed from staging area.\n", file_path);
    return 0;
}

// Check if a file is already staged and retrieve its hash if present
int is_file_staged(const char *file_path, char *existing_hash) {
    if (file_path == NULL || existing_hash == NULL) {
        return -1;
    }

    FILE *file = fopen(STAGING_FILE_PATH, "r");
    if (!file) {
        perror("Error opening staging metadata file");
        return -1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char stored_path[512];
        char stored_hash[65];
        if (sscanf(line, "%s %s", stored_path, stored_hash) == 2) {
            if (strcmp(stored_path, file_path) == 0) {
                strcpy(existing_hash, stored_hash);  // File is staged, return its hash
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;  // File not found in staging
}

// List all staged files (for debugging purposes)
void list_staged_files(void) {
    FILE *file = fopen(STAGING_FILE_PATH, "r");
    if (!file) {
        perror("Error opening staging metadata file");
        return;
    }

    char line[1024];
    printf("Staged files:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}
