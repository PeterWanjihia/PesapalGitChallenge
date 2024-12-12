#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract_argument.h"

// Function to extract staged files from the staged_files.txt file
int extract_staged_arguments(const char *file_name, StagedFile staged_files[], int *num_files) {
    FILE *file = fopen(file_name, "r");  // Open the staged files list
    if (!file) {
        perror("Error opening staged_files.txt");
        return -1;  // Return error if file can't be opened
    }

    int count = 0;
    // Read each line of the file
    while (fscanf(file, "%255s %64s", staged_files[count].filename, staged_files[count].filehash) == 2) {
        count++;
        if (count >= MAX_FILES) {
            break;  // Stop if we have reached the max number of files
        }
    }

    fclose(file);
    *num_files = count;  // Set the number of staged files
    return 0;  // Return success
}


