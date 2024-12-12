#ifndef COMMIT_H
#define COMMIT_H

#include "commit_utils.h"

// Define the commit structure
typedef struct {
    char commit_message[256];  // Max length for the commit message
    AuthorInfo author;         // Author info
    const char *timestamp;     // Timestamp for the commit
    char staged_file_hashes[1024]; // Staged file hashes
    char parent_commit[64];    // Parent commit hash
    char *commit_id;           // Commit ID (this will store the result of generate_commit_id)
} Commit;


// Function to create the commit object with the first five parts
void create_commit_object(Commit *commit, char *commit_message, AuthorInfo author, const char *timestamp, const char *staged_hashes);


#endif // COMMIT_H
