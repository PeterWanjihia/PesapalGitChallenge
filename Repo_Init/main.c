#include <stdio.h>
#include <string.h>
#include "init.h"
#include "hashing.h"
#include "staging.h"  // Include staging module

// Add file to both the repository (index) and the staging area
int add_file(const char *file_path) {
    // 1. Hash the file using the existing hashing module and store metadata in the index
    if (handle_add_command(file_path) != 0) {
        return -1;
    }

    // 2. Get the file hash from the index (the same as the one used for the add operation)
    FILE *index_file = fopen(".repo/objects/index", "r");
    if (!index_file) {
        perror("Error opening index file");
        return -1;
    }

    char line[256];
    char hash[65];  // Local variable to store the file hash
    while (fgets(line, sizeof(line), index_file)) {
        char file_in_index[256];
        sscanf(line, "%s %s", file_in_index, hash);
        if (strcmp(file_in_index, file_path) == 0) {
            // File found in the index, store it in the staging area
            fclose(index_file);
            return store_in_staging_area(file_path, hash);  // Pass both the file path and hash
        }
    }

    fclose(index_file);
    fprintf(stderr, "File not found in index: %s\n", file_path);
    return -1;  // File was not found in the index
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [<args>]\n", argv[0]);
        return 1;
    }

    // Handle the `init` command
    if (strcmp(argv[1], "init") == 0) {
        return initialize_repository();

    // Handle the `add` command
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s add <file>\n", argv[0]);
            return 1;
        }

        // Call the `add_file()` to hash and store the file in both index and staging
        const char *file_path = argv[2];
        return add_file(file_path);

    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return 1;
    }
}
