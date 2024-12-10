#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage.h"

// Main function to handle command-line arguments
int main(int argc, char *argv[]) {
    if (argc < 2) {  // We need at least 2 arguments: command and possibly file paths
        fprintf(stderr, "Usage: %s <command> <file_paths...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Handle the 'add' command
    if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {  // We need at least one file to add
            fprintf(stderr, "Usage: %s add <file_paths...>\n", argv[0]);
            return EXIT_FAILURE;
        }
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
    }
    // Handle the 'remove' command
    else if (strcmp(argv[1], "remove") == 0) {
        if (argc < 3) {  // We need at least one file to remove
            fprintf(stderr, "Usage: %s remove <file_paths...>\n", argv[0]);
            return EXIT_FAILURE;
        }
        // Loop through all file arguments passed
        for (int i = 2; i < argc; i++) {
            char file_path[512];
            strcpy(file_path, argv[i]);

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
    }
    // Handle the 'stagedlist' command
    else if (strcmp(argv[1], "stagedlist") == 0) {
        // List all staged files
        list_staged_files();
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
