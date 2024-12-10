#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command> <file_paths...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Handle commands
    if (strcmp(argv[1], "add") == 0) {
        // Loop through all file arguments passed
        for (int i = 2; i < argc; i++) {
            char file_path[512];
            char file_hash[65];

            // Copy the file path from the command-line argument
            strcpy(file_path, argv[i]);

            // Generate the file hash
            generate_hash(file_path, file_hash);

            // Log the action
            printf("Adding file '%s' to staging area...\n", file_path);

            // Store the file in the staging area
            int result = store_in_staging_area(file_path, file_hash);
            if (result != 0) {
                fprintf(stderr, "Error adding file '%s' to staging area.\n", file_path);
                return EXIT_FAILURE;
            }

            printf("File '%s' added to staging area.\n", file_path);
        }
    } else if (strcmp(argv[1], "remove") == 0) {
        for (int i = 2; i < argc; i++) {
            char file_path[512];

            // Copy the file path from the command-line argument
            strcpy(file_path, argv[i]);

            // Remove the file from the staging area
            int result = remove_from_staging_area(file_path);
            if (result != 0) {
                fprintf(stderr, "Error removing file '%s' from staging area.\n", file_path);
                return EXIT_FAILURE;
            }

            printf("File '%s' removed from staging area.\n", file_path);
        }
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
