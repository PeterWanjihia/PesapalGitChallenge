#ifndef COMMIT_UTILS_H
#define COMMIT_UTILS_H

// Structure to hold author information
typedef struct {
    char name[100];
    char email[100];
} AuthorInfo;

// Declare the functions to retrieve commit metadata

// Function to get the commit author's name and email
AuthorInfo get_commit_author();

// Function to get the current timestamp
const char* get_commit_timestamp();

// Function to retrieve staged file hashes (simulated here)
char* get_staged_file_hashes();

// Function to get the parent commit (returns "none" for now)
const char* get_parent_commit();

// Function to generate a commit ID based on the commit data
char* generate_commit_id(const char* commit_message, const AuthorInfo author, const char* timestamp, const char* staged_file_hashes, const char* parent_commit);


#endif // COMMIT_UTILS_H
