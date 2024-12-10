#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"
#include "hashing.h"
#include "stage.h"  // Include staging module

// Function to update the index with the new file hash
int store_in_index(const char *file_path, const char *file_hash) {
    // Open the index file in read and write mode
    FILE *index_file = fopen(".repo/objects/index", "r+");
    if (!index_file) {
        perror("Error opening index file");
        return -1;
    }

    char line[256];
    int file_found = 0;

    // Temporary file to store updated index
    FILE *temp_file = fopen(".repo/objects/index.temp", "w");
    if (!temp_file) {
        perror("Error creating temporary index file");
        fclose(index_file);
        return -1;
    }

    // Read the existing index and update the file hash if the file is found
    while (fgets(line, sizeof(line), index_file)) {
        char stored_file[256], stored_hash[65];
        sscanf(line, "%s %s", stored_file, stored_hash);

        if (strcmp(stored_file, file_path) == 0) {
            file_found = 1;
            // Update the hash for this file in the index
            fprintf(temp_file, "%s %s\n", file_path, file_hash);
        } else {
            // Keep the old entry if it's not the one we're updating
            fputs(line, temp_file);
        }
    }

    // If file is not found, append a new entry for it
    if (!file_found) {
        fprintf(temp_file, "%s %s\n", file_path, file_hash);
    }

    // Close both the original and temporary files
    fclose(index_file);
    fclose(temp_file);

    // Replace the old index file with the updated one
    if (rename(".repo/objects/index.temp", ".repo/objects/index") != 0) {
        perror("Error renaming temporary index file");
        return -1;
    }

    return 0;
}

// Add file to both the repository (index) and the staging area
int add_file(const char *file_path) {
    // 1. Generate the hash of the file (this ensures that the hash is fresh and reflects any changes)
    char file_hash[65];
    generate_hash(file_path, file_hash);

    // 2. Store the file and its hash in the staging area
    if (store_in_staging_area(file_path, file_hash) != 0) {
        return -1;
    }

    // 3. Update the index with the new file hash
    if (store_in_index(file_path, file_hash) != 0) {
        return -1;
    }

    return 0;
}

// Main function to handle command-line arguments
int main(int argc, char *argv[]) {
    if (argc < 2) {  // We need at least 2 arguments: command and possibly file paths
        fprintf(stderr, "Usage: %s <command> <file_paths...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Handle the `init` command
    if (strcmp(argv[1], "init") == 0) {
        return initialize_repository();

    // Handle the `add` command
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {  // We need at least one file to add
            fprintf(stderr, "Usage: %s add <file_paths...>\n", argv[0]);
            return EXIT_FAILURE;
        }

        // Loop through all file arguments passed
        for (int i = 2; i < argc; i++) {
            const char *file_path = argv[i];

            // Log the action
            printf("Adding file '%s' to staging area...\n", file_path);

            // Hash the file and store it in both index and staging area
            int result = add_file(file_path);
            if (result != 0) {
                fprintf(stderr, "Error adding file '%s' to staging area.\n", file_path);
                return EXIT_FAILURE;
            }

            printf("File '%s' added to staging area.\n", file_path);
        }

    // Handle the `remove` command
    } else if (strcmp(argv[1], "remove") == 0) {
        if (argc < 3) {  // We need at least one file to remove
            fprintf(stderr, "Usage: %s remove <file_paths...>\n", argv[0]);
            return EXIT_FAILURE;
        }

        // Loop through all file arguments passed
        for (int i = 2; i < argc; i++) {
            const char *file_path = argv[i];

            // Log the action
            printf("Removing file '%s' from staging area...\n", file_path);

            // Remove the file from the staging area
            int result = remove_from_staging_area(file_path);
            if (result != 0) {
                fprintf(stderr, "Error removing file '%s' from staging area.\n", file_path);
                return EXIT_FAILURE;
            }

            printf("File '%s' removed from staging area.\n", file_path);
        }

    // Handle the `stagedlist` command to list staged files
    } else if (strcmp(argv[1], "stagedlist") == 0) {
        list_staged_files();  // List the staged files

    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
