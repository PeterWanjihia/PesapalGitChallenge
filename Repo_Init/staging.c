// staging.c
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>  // For mkdir
#include <errno.h>

#define STAGING_DIR ".repo/staging"

// Create the staging directory if it doesn't exist
int ensure_staging_directory_exists() {
    struct stat st = {0};
    if (stat(STAGING_DIR, &st) == -1) {
        if (mkdir(STAGING_DIR, 0755) == -1) {
            perror("Error creating staging directory");
            return -1;
        }
    }
    return 0;
}

// Store the file metadata in the staging area
int store_in_staging_area(const char *file_path, const char *file_hash) {
    // Ensure the staging directory exists
    if (ensure_staging_directory_exists() != 0) {
        return -1;
    }

    // Open the staging file
    FILE *staging_file = fopen(STAGING_DIR "/staged_files.txt", "a");
    if (!staging_file) {
        perror("Error opening staging file");
        return -1;
    }

    // Write the file path and its hash into the staging file
    fprintf(staging_file, "%s %s\n", file_path, file_hash);
    fclose(staging_file);
    
    return 0;
}
