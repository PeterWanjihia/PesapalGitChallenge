# Why the extract_argument Module Was Crucial

## 1. The Challenge with the store_blob Signature

The `store_blob` function signature is:
```c
void store_blob(const char *file_path, const unsigned char *hash);
```

This function expects two arguments:
- `file_path`: The path to the file being stored
- `hash`: The SHA256 hash of the file

## 2. Problems with Manual Argument Passing

### Challenges
- **Redundancy and Risk of Error**: Manually passing file paths and hashes can lead to mismatches
- **Increased Cognitive Load**: Tracking arguments for multiple files is complex
- **Scalability**: Becomes increasingly difficult to manage as file count grows

## 3. Why extract_argument.c Was the Logical Solution

### a. Automating Argument Passing
- Automatically extract filenames and hashes from staged_files.txt
- Store in a structured StagedFile struct
- Iterate and pass to store_blob without manual specification

### b. Streamlining commit_storage.c
- Decouple data extraction from data storage
- `commit_storage.c` focuses on storing blobs
- `extract_argument.c` handles file extraction

### c. Efficient File Iteration
- Use `extract_staged_arguments()` to get all staged files
- Iterate through StagedFile array
- Automatically pass filename and hash to store_blob

### d. Avoiding Hardcoded File Paths
- Prevent typos and hash mismatches
- Ensure correct filename-hash pairing
- Easily scalable as file count increases

## 4. Why Not Keep Everything in commit_storage.c?

### Potential Issues
- Code duplication
- Increased complexity
- Lack of reusability

## 5. Key Reasons for extract_argument.c

- Automate filename and hash passing
- Keep commit_storage.c focused
- Ensure scalability and maintainability
- Avoid code duplication
- Create a modular, reusable extraction logic

## Conclusion

The extract_argument module solved a critical challenge: simplifying the process of passing arguments to `store_blob` while reducing human error and improving code scalability.
