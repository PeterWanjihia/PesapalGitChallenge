#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing.h"

int main() {
    const char *test_file = "test_file.txt";

    printf("=== Testing `hashing.c` Functions ===\n\n");

    // Step 1: Check if the file exists
    printf("[1] Checking file existence...\n");
    if (!check_file_exists(test_file)) {
        fprintf(stderr, "Error: Test file '%s' does not exist. Create it first.\n", test_file);
        return 1;
    }
    printf("File '%s' exists.\n\n", test_file);

    // Step 2: Initialize hash context
    printf("[2] Initializing hash context...\n");
    void *hash_context = initialize_hash_context();
    if (!hash_context) {
        fprintf(stderr, "Error: Failed to initialize hash context.\n");
        return 1;
    }
    printf("Hash context initialized successfully.\n\n");

    // Step 3: Read file in chunks and process
    printf("[3] Reading file in chunks and hashing...\n");
    void process_chunk(const unsigned char *data, size_t len) {
        update_hash_context(hash_context, data, len);
        printf("Processed chunk of size: %zu bytes\n", len);
    }

    if (read_file_in_chunks(test_file, process_chunk) != 0) {
        fprintf(stderr, "Error: Failed to read file '%s'.\n", test_file);
        free(hash_context);
        return 1;
    }
    printf("File '%s' read successfully in chunks.\n\n", test_file);

    // Step 4: Finalize the hash and convert to hex
    printf("[4] Finalizing hash...\n");
    unsigned char binary_hash[32];  // SHA-256 binary output
    finalize_hash_context(hash_context, binary_hash);
    printf("Binary hash finalized successfully.\n");

    char hex_hash[65];  // Hex string for the hash
    format_hash_to_hex(binary_hash, hex_hash);
    printf("Hexadecimal hash: %s\n\n", hex_hash);

    // Step 5: Write metadata
    printf("[5] Writing metadata to `.repo/objects/index`...\n");
    if (write_metadata(test_file, hex_hash) != 0) {
        fprintf(stderr, "Error: Failed to write metadata for '%s'.\n", test_file);
        return 1;
    }
    printf("Metadata written successfully for file '%s'.\n\n", test_file);

    printf("=== All tests passed! ===\n");
    return 0;
}
