#ifndef STAGING_H
#define STAGING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STAGING_FILE_PATH ".repo/staging/staged_files.txt"  // Path to the staging metadata file

// Function to ensure the staging directory and file exist
int ensure_staging_directory_exists(void);

// Function to add a file to the staging area
int store_in_staging_area(const char *file_path, const char *file_hash);

// Function to remove a file from the staging area
int remove_from_staging_area(const char *file_path);

// Function to check if a file is already staged
int is_file_staged(const char *file_path, char *existing_hash);

// Function to list all staged files (for debugging purposes)
void list_staged_files(void);

#endif  // STAGING_H
