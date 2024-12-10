#ifndef STAGE_H
#define STAGE_H

// Constants
#define STAGING_FILE_PATH ".repo/staging/staged_files.txt"

// Function declarations
void normalize_path(char *path);
void trim_hash(char *hash);
void generate_hash(const char *filename, char *hash_out);
void log_message(const char *message);
int store_in_staging_area(const char *file_path, const char *file_hash);
int remove_from_staging_area(const char *file_path);

#endif // STAGE_H
