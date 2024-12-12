#include "commit_storage.h"
#include <errno.h>  // For errno and EEXIST

// Function to create directories recursively
int create_directory(const char *dir_path) {
    char path[512];
    snprintf(path, sizeof(path), "%s", dir_path);

    // Check if directory already exists
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        // Create the directory
        if (mkdir(path, 0700) == -1) {
            perror("Error creating directory");
            return -1;
        }
    }
    return 0;  // Directory created or already exists
}

// Convert a binary hash to a hexadecimal string
void hash_to_hex(const unsigned char *hash, char *hex_str, size_t hash_len) {
    for (size_t i = 0; i < hash_len; ++i) {
        snprintf(hex_str + (i * 2), 3, "%02x", hash[i]);
    }
}

// Function to store the blob in the object storage
void store_blob(const char *file_path, const unsigned char *hash) {
    // Convert the hash to a hexadecimal string
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];  // Double the size of the binary hash + null-terminator
    hash_to_hex(hash, hash_hex, SHA256_DIGEST_LENGTH);

    // First two characters of the hash become the directory name
    char dir_path[3];  // Only two characters for the directory
    snprintf(dir_path, sizeof(dir_path), "%02x", hash[0]);  // e.g., "35" from hash

    // Full path for the directory where the object will be stored
    char full_dir_path[256];  // Increased buffer size
    snprintf(full_dir_path, sizeof(full_dir_path), "%s/%s", STORAGE_DIR, dir_path);

    // Create the .repo/objects/storage_files directory if it doesn't exist
    if (create_directory(STORAGE_DIR) != 0) {
        return;  // Exit if the directory couldn't be created
    }

    // Create the specific directory for the object based on the hash
    if (create_directory(full_dir_path) != 0) {
        return;  // Exit if the directory couldn't be created
    }

    // Full path for the file where the object will be stored (using the rest of the hash as filename)
    char object_file_path[512];  // Increased buffer size
    snprintf(object_file_path, sizeof(object_file_path), "%s/%s", full_dir_path, hash_hex + 2);  // Skip first 2 chars for the filename

    // Open the input file to read its content
    FILE *input_file = fopen(file_path, "rb");
    if (!input_file) {
        perror("Error opening input file");
        return;
    }

    // Open the object file to write the content
    FILE *object_file = fopen(object_file_path, "wb");
    if (!object_file) {
        perror("Error creating object file");
        fclose(input_file);
        return;
    }

    // Read from the input file and write to the object file
    unsigned char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
        fwrite(buffer, 1, bytes_read, object_file);
    }

    fclose(input_file);
    fclose(object_file);

    printf("Blob stored at: %s\n", object_file_path);
}

int main() {
    StagedFile staged_files[MAX_FILES];  // Array to hold staged files
    int num_files = 0;

    // Extract staged files from the default STAGING_FILE_PATH
    if (extract_staged_arguments(STAGING_FILE_PATH, staged_files, &num_files) != 0) {
        printf("Error extracting staged files.\n");
        return 1;
    }

    // Process each staged file and store the blobs
    for (int i = 0; i < num_files; i++) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        // Convert hash from hex string to byte array
        for (int j = 0; j < SHA256_DIGEST_LENGTH; j++) {
            sscanf(staged_files[i].filehash + 2 * j, "%2hhx", &hash[j]);
        }

        store_blob(staged_files[i].filename, hash);
    }

    return 0;
}
