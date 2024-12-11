#include <stdio.h>
#include "commit_utils.h"

int main() {
    // Get commit author information
    AuthorInfo author = get_commit_author();
    printf("Author Name: %s\n", author.name);
    printf("Author Email: %s\n", author.email);

    // Get commit timestamp
    const char* timestamp = get_commit_timestamp();
    printf("Commit Timestamp: %s\n", timestamp);

    return 0;
}
