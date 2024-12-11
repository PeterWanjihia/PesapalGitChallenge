#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commit_utils.h"

// Function to get the commit author's name and email
AuthorInfo get_commit_author() {
    // We'll reuse the logic from get_author_info (in case it's already implemented elsewhere).
    // For the sake of this example, we simulate getting the author's name and email.
    
    AuthorInfo author;

    // Simulate collecting the author's information (in a real system, we'd prompt the user or pull from a config)
    printf("Enter Author Name: ");
    fgets(author.name, sizeof(author.name), stdin);
    author.name[strcspn(author.name, "\n")] = '\0';  // Remove newline character if present

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
