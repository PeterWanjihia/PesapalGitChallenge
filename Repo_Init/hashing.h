#ifndef HASHING_H
#define HASHING_H

#include <stddef.h>
#define HASH_LENGTH 64  // Define HASH_LENGTH

// Check if the file exists and is accessible
int check_file_exists(const char *file_path);

// Read a file in chunks and process each chunk
int read_file_in_chunks(const char *file_path, void (*process_chunk)(const unsigned char *, size_t));

// Initialize the hash context
void *initialize_hash_context();

// Update the hash context with a chunk of data
void update_hash_context(void *hash_context, const unsigned char *data, size_t len);

// Finalize the hash and retrieve the binary output
void finalize_hash_context(void *hash_context, unsigned char *output_hash);

// Convert the binary hash to a hexadecimal string
void format_hash_to_hex(const unsigned char *binary_hash, char *hex_hash);

// Write file metadata (file path and hash) to the metadata index
int write_metadata(const char *file_path, const char *hash);

// Handle the `add` command for hashing and storing file metadata
int handle_add_command(const char *file_path);

#endif


