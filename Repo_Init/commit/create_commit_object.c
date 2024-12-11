#include <stdio.h>
#include <string.h>
#include "commit.h"
#include "commit_utils.h"



// Function to create the commit object with the first five parts
void create_commit_object(Commit *commit, char *commit_message, AuthorInfo author, const char *timestamp, const char *staged_hashes) {
    // Store commit message
    strncpy(commit->commit_message, commit_message, sizeof(commit->commit_message) - 1);
    commit->commit_message[sizeof(commit->commit_message) - 1] = '\0';

    // Store author info
    commit->author = author;

    // Store timestamp
    commit->timestamp = timestamp;

    // Store staged file hashes
    strncpy(commit->staged_file_hashes, staged_hashes, sizeof(commit->staged_file_hashes) - 1);
    commit->staged_file_hashes[sizeof(commit->staged_file_hashes) - 1] = '\0';

    // Store parent commit info (initially "none")
    strncpy(commit->parent_commit, "none", sizeof(commit->parent_commit) - 1);
    commit->parent_commit[sizeof(commit->parent_commit) - 1] = '\0';

    // Generate commit ID based on the data
    commit->commit_id = generate_commit_id(commit->commit_message, commit->author, commit->timestamp, commit->staged_file_hashes, commit->parent_commit);
}
