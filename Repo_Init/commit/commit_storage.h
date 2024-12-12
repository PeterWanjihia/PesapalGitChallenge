#ifndef COMMIT_STORAGE_H
#define COMMIT_STORAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  // Include sys/stat.h for mkdir and stat
#include <sys/types.h> // Include sys/types.h for mkdir

#include <openssl/sha.h> // Include OpenSSL for SHA256

#define MAX_FILES 100  // Max number of staged files we can process
#define STAGING_FILE_PATH ".repo/staging/staged_files.txt"  // Path to the staged files list
#define STORAGE_DIR ".repo/objects/storage_files"

// Define a structure to hold the filename and hash
typedef struct {
    char filename[256];  // Max filename length (can be adjusted)
    char filehash[65];   // SHA256 hash in hex is 64 chars + null terminator
} StagedFile;

/**
 * Function to extract staged files from the specified file.
 *
 * @param file_name The name of the staged files list (e.g., "staged.txt").
 * @param staged_files Array of StagedFile structures where file data will be stored.
 * @param num_files A pointer to the variable that will hold the number of staged files.
 * @return 0 on success, -1 on failure (if the file cannot be opened or read).
 */
int extract_staged_arguments(const char *file_name, StagedFile staged_files[], int *num_files);

#endif  // COMMIT_STORAGE_H
