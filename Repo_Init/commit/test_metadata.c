#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commit.h"
#include "commit_utils.h"

// Function to check if the argument for the commit message is passed
int get_commit_message(int argc, char *argv[], char *commit_message) {
    // Check if the user has provided the '-m' flag for the commit message
    if (argc < 3) {
        printf("Error: Commit message missing. Use the -m flag to specify the commit message.\n");
        return 1;  // Return 1 to indicate error
    }

    // Ensure that the '-m' flag is present
    if (strcmp(argv[1], "commit") != 0 || strcmp(argv[2], "-m") != 0) {
        printf("Error: Invalid command. Usage: ./program commit -m \"commit message\"\n");
        return 1;
    }

    // Collect the commit message (everything after the -m flag)
    commit_message[0] = '\0';  // Initialize the commit_message to an empty string
    for (int i = 3; i < argc; i++) {
        strcat(commit_message, argv[i]);
        if (i != argc - 1) {
            strcat(commit_message, " ");  // Add space between words
        }
    }

    return 0;  // Return 0 to indicate success
}

int main(int argc, char *argv[]) {
    // Buffers to hold commit data
    char commit_message[256];    // Buffer for the commit message
    AuthorInfo author;           // Structure to hold author info
    const char *timestamp;       // Timestamp for the commit
    char staged_file_hashes[1024];  // Staged file hashes

    // Get the commit message from command line arguments
    if (get_commit_message(argc, argv, commit_message) != 0) {
        return 1;  // Exit with an error code if commit message isn't correct
    }

    // Get author name and email
    author = get_commit_author();

    // Get current timestamp
    timestamp = get_commit_timestamp();

    // Get staged file hashes (simulated for now)
    strcpy(staged_file_hashes, get_staged_file_hashes());

    // Create the commit object
    Commit commit;
    create_commit_object(&commit, commit_message, author, timestamp, staged_file_hashes);

    // Log the commit object details to the console
    printf("\nCommit ID: %s\n", commit.commit_id);
    printf("Commit Message: %s\n", commit.commit_message);
    printf("Author Name: %s\n", commit.author.name);
    printf("Author Email: %s\n", commit.author.email);
    printf("Commit Timestamp: %s\n", commit.timestamp);
    printf("Staged File Hashes: %s\n", commit.staged_file_hashes);
    printf("Parent Commit: %s\n", commit.parent_commit);

    return 0;
}
