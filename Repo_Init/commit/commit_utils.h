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

#endif // COMMIT_UTILS_H
