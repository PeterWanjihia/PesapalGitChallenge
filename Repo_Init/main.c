#include <stdio.h>
#include <string.h>
#include "init.h"
#include "hashing.h"  // Include Ticket 2's hashing module

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

        // Call the hashing module to add the file
        const char *file_path = argv[2];
        return handle_add_command(file_path);

    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return 1;
    }
}
