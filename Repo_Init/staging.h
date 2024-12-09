#ifndef STAGING_H
#define STAGING_H

// Function to check if the staging area exists
int check_staging_area_exists(void);

// Function to store the file metadata (path and hash) in the staging area
int store_in_staging_area(const char *file_path, const char *file_hash);

// Function to list staged files in the staging area (for future use)
int list_staged_files(void);

#endif // STAGING_H
