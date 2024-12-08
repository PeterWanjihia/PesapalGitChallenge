#include "hashing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <openssl/sha.h>  // OpenSSL library for SHA-256 hashing

#define CHUNK_SIZE 1024
#define HASH_LENGTH 64
#define BINARY_HASH_LENGTH 32
#define METADATA_FILE ".repo/objects/index"

// Check if the file exists and is accessible
int check_file_exists(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file) {
        fclose(file);
        return 1;  // File exists
    }
    return 0;  // File does not exist
}

// Read a file in chunks and process each chunk
int read_file_in_chunks(const char *file_path, void (*process_chunk)(const unsigned char *, size_t)) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    unsigned char buffer[CHUNK_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        process_chunk(buffer, bytes_read);  // Process the chunk using the provided callback
    }

    if (ferror(file)) {
        perror("Error reading file");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;  // Success
}

// Initialize the hash context
void *initialize_hash_context() {
    SHA256_CTX *sha256 = malloc(sizeof(SHA256_CTX));
    if (!sha256) {
        fprintf(stderr, "Error allocating memory for SHA256 context\n");
        return NULL;
    }
    SHA256_Init(sha256);
    return sha256;
}

// Update the hash context with a chunk of data
void update_hash_context(void *hash_context, const unsigned char *data, size_t len) {
    if (!hash_context) {
        fprintf(stderr, "Hash context is NULL\n");
        return;
    }
    SHA256_Update((SHA256_CTX *)hash_context, data, len);
}

// Finalize the hash and retrieve the binary output
void finalize_hash_context(void *hash_context, unsigned char *output_hash) {
    if (!hash_context) {
        fprintf(stderr, "Hash context is NULL\n");
        return;
    }
    SHA256_Final(output_hash, (SHA256_CTX *)hash_context);
    free(hash_context);  // Free the allocated hash context
}

// Convert the binary hash to a hexadecimal string
void format_hash_to_hex(const unsigned char *binary_hash, char *hex_hash) {
    for (int i = 0; i < BINARY_HASH_LENGTH; i++) {
        sprintf(hex_hash + (i * 2), "%02x", binary_hash[i]);
    }
    hex_hash[HASH_LENGTH] = '\0';  // Null-terminate the hex string
}

// Write file metadata (file path and hash) to the metadata index
int write_metadata(const char *file_path, const char *hash) {
    FILE *metadata_file = fopen(METADATA_FILE, "a");  // Open in append mode
    if (!metadata_file) {
        perror("Error opening metadata file");
        return -1;
    }

    // Write file path and hash
    fprintf(metadata_file, "%s %s\n", file_path, hash);
    fclose(metadata_file);
    return 0;  // Success
}

// Handle the `add` command for hashing and storing file metadata
int handle_add_command(const char *file_path) {
    if (!check_file_exists(file_path)) {
        fprintf(stderr, "Error: File '%s' does not exist\n", file_path);
        return -1;
    }

    // Initialize the hash context
    void *hash_context = initialize_hash_context();
    if (!hash_context) {
        return -1;
    }

    // Callback to update the hash context with file chunks
    void process_chunk(const unsigned char *data, size_t len) {
        update_hash_context(hash_context, data, len);
    }

    // Read the file and hash its content
    if (read_file_in_chunks(file_path, process_chunk) != 0) {
        free(hash_context);  // Free the context if reading fails
        return -1;
    }

    // Finalize the hash
    unsigned char binary_hash[BINARY_HASH_LENGTH];
    finalize_hash_context(hash_context, binary_hash);

    // Convert the binary hash to a hex string
    char hex_hash[HASH_LENGTH + 1];
    format_hash_to_hex(binary_hash, hex_hash);

    // Write the metadata (file path and hash) to the repository index
    if (write_metadata(file_path, hex_hash) != 0) {
        return -1;
    }

    printf("File '%s' hashed and added to index.\nHash: %s\n", file_path, hex_hash);
    return 0;
}
