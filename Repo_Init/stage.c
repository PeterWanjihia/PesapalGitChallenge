#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <openssl/sha.h>
#include "stage.h"

// Function to normalize file path by removing trailing slashes
void normalize_path(char *path) {
    size_t len = strlen(path);
    while (len > 0 && (path[len - 1] == '/' || path[len - 1] == '\\')) {
        path[len - 1] = '\0';
        len--;
    }
}

// Function to trim trailing whitespaces and newlines from a string
void trim_hash(char *hash) {
    size_t len = strlen(hash);
    while (len > 0 && (hash[len - 1] == '\n' || hash[len - 1] == ' ' || hash[len - 1] == '\r')) {
        hash[len - 1] = '\0';
        len--;
    }
}

// Function to generate the SHA-256 hash of a file
void generate_hash(const char *filename, char *hash_out) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file for hashing");
        exit(EXIT_FAILURE);
    }

    unsigned char buffer[1024];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) != 0) {
        SHA256_Update(&sha256_ctx, buffer, bytes_read);
    }

    SHA256_Final(hash, &sha256_ctx);
    fclose(file);

    // Convert hash to hexadecimal string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash_out[i * 2], "%02x", hash[i]);
    }
}

// Function to log information to the terminal
void log_message(const char *message) {
    printf("%s\n", message);
}

// Function to store or update a file in the staging area
int store_in_staging_area(const char *file_path, const char *file_hash) {
    if (file_path == NULL || file_hash == NULL) {
        fprintf(stderr, "Invalid file path or hash.\n");
        return -1;
    }

    // Normalize the file path to avoid issues with different file formats
    char normalized_path[512];
    strcpy(normalized_path, file_path);
    normalize_path(normalized_path);

    FILE *staging_file = fopen(STAGING_FILE_PATH, "r+");
    if (!staging_file) {
        perror("Error opening staging metadata file");
        return -1;
    }

    // Create a temporary file to write the updated staging information
    FILE *temp_file = fopen(".repo/staging/temp.txt", "w");
    if (!temp_file) {
        perror("Error creating temporary staging file");
        fclose(staging_file);
        return -1;
    }

    char line[1024];
    int file_found = 0;
    while (fgets(line, sizeof(line), staging_file)) {
        char stored_file[512], stored_hash[65];
        // Read each entry into file and hash
        if (sscanf(line, "%s %s", stored_file, stored_hash) == 2) {
            normalize_path(stored_file);  // Normalize the stored file path
            trim_hash(stored_hash);       // Trim the existing hash

            // Compare the normalized file path
            if (strcmp(stored_file, normalized_path) == 0) {
                file_found = 1;
                log_message("File entry found in staging area.");
                // Compare the existing hash with the new hash
                if (strcmp(stored_hash, file_hash) != 0) {
                    log_message("Hashes are different. Updating the hash.");

                    // Clear the current line and write the new hash
                    fprintf(temp_file, "%s %s\n", normalized_path, file_hash);
                    log_message("Hash updated successfully.");
                } else {
                    // Log that the hash is the same, no update needed
                    log_message("Hash is the same. No changes made.");
                    fputs(line, temp_file); // Keep the existing entry
                }
            } else {
                // If it's a different file, copy it to the temp file
                fputs(line, temp_file);
            }
        } else {
            // Handle malformed lines
            fprintf(stderr, "Skipping malformed line: %s", line);
            continue;
        }
    }

    // If the file wasn't found, append the new file and hash to the staging file
    if (!file_found) {
        fprintf(temp_file, "%s %s\n", normalized_path, file_hash);
        log_message("File entry not found in staging area. Adding new entry.");
    }

    // Close both the staging file and the temporary file
    fclose(staging_file);
    fclose(temp_file);

    // Replace the original staging file with the updated one
    if (rename(".repo/staging/temp.txt", STAGING_FILE_PATH) != 0) {
        perror("Error renaming temporary staging file");
        return -1;
    }

    return 0;
}

// Function to remove a file from the staging area
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

    // Create a temporary file to write the updated staging information
    FILE *temp_file = fopen(".repo/staging/temp.txt", "w");
    if (!temp_file) {
        perror("Error creating temporary staging file");
        fclose(file);
        return -1;
    }

    char line[1024];
    int file_found = 0;
    while (fgets(line, sizeof(line), file)) {
        char stored_file[512];
        if (sscanf(line, "%s", stored_file) == 1) {
            normalize_path(stored_file);  // Normalize the stored file path

            // If the file path matches, skip the line (remove it)
            if (strcmp(stored_file, file_path) == 0) {
                file_found = 1;
                log_message("Removing file from staging area...");
                continue;
            }
        }

        // Copy remaining lines to the temporary file
        fputs(line, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    // If the file wasn't found, return an error
    if (!file_found) {
        fprintf(stderr, "File '%s' not found in staging area.\n", file_path);
        return -1;
    }

    // Replace the original staging file with the updated one
    if (rename(".repo/staging/temp.txt", STAGING_FILE_PATH) != 0) {
        perror("Error renaming temporary staging file");
        return -1;
    }

    log_message("File successfully removed from staging area.");
    return 0;
}

// Function to list all staged files
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