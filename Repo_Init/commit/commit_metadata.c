#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commit_utils.h"
#include "../stage.h"
#include <openssl/sha.h>



// Function to get the commit author's name and email
AuthorInfo get_commit_author() {
    AuthorInfo author;

    // Get author's name
    printf("Enter Author Name: ");
    fgets(author.name, sizeof(author.name), stdin);
    author.name[strcspn(author.name, "\n")] = '\0';  // Remove newline character if present

    // Get author's email
    printf("Enter Author Email: ");
    fgets(author.email, sizeof(author.email), stdin);
    author.email[strcspn(author.email, "\n")] = '\0';  // Remove newline character if present

    return author;
}

// Function to get the current commit timestamp in the format "YYYY-MM-DD HH:MM:SS"
const char* get_commit_timestamp() {
    static char timestamp[20];  // Store the timestamp as a static string

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Format the timestamp as "YYYY-MM-DD HH:MM:SS"
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    
    return timestamp;
}



// Function to get all the staged file hashes from the staged_files.txt
char* get_staged_file_hashes() {
    FILE *file = fopen(STAGING_FILE_PATH, "r");  // Open the staged files list
    if (file == NULL) {
        printf("Error: Unable to open staged files list at %s\n", STAGING_FILE_PATH);
        return NULL;
    }

    static char all_hashes[1024];  // Buffer to hold concatenated hashes
    memset(all_hashes, 0, sizeof(all_hashes));  // Clear the buffer to ensure it's empty

    char line[256];  // Buffer for each line from the file
    char hash[65];   // Buffer to store the hash (assuming SHA-256 hash length)
    while (fgets(line, sizeof(line), file)) {
        // Parse the line to get the file name and hash
        if (sscanf(line, "%*s %64s", hash) == 1) {
            // Append the hash to the result buffer
            if (strlen(all_hashes) > 0) {
                strcat(all_hashes, " ");  // Separate hashes with a space
            }
            strcat(all_hashes, hash);  // Add the hash
        }
    }

    fclose(file);  // Close the file
    return all_hashes;  // Return the concatenated hashes
}

// Function to simulate retrieving the parent commit ID
// This would typically involve reading the most recent commit hash from a commit history.
const char* get_parent_commit() {
    // For the first commit, there will be no parent. In subsequent commits, you'd fetch the parent commit hash.
    printf("Parent Commit: none\n"); // Replace with actual logic when implementing full system.
}

// Function to generate commit ID based on commit metadata
char* generate_commit_id(const char* commit_message, const AuthorInfo author, const char* timestamp, const char* staged_file_hashes, const char* parent_commit) {
    // Concatenate the commit metadata into a single string
    char commit_data[2048];  // Buffer large enough to hold all the concatenated data
    snprintf(commit_data, sizeof(commit_data), "%s%s%s%s%s", commit_message, author.name, author.email, timestamp, staged_file_hashes);
    
    // Hash the concatenated string to generate a commit ID using SHA-256
    static char commit_id[SHA256_DIGEST_LENGTH * 2 + 1];  // Buffer for the commit ID (SHA-256 produces 64 hex characters)

    // Perform SHA-256 hash on the commit data
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_context;
    SHA256_Init(&sha256_context);
    SHA256_Update(&sha256_context, commit_data, strlen(commit_data));
    SHA256_Final(hash, &sha256_context);

    // Convert the hash to a hexadecimal string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(commit_id + (i * 2), "%02x", hash[i]);
    }

    return commit_id;  // Return the commit ID as a hexadecimal string
}
