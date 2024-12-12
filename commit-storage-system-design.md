# Commit Storage System Design

## Project Overview

### Goal
Design a version control-inspired commit storage system to:
- Extract staged files
- Store blobs based on file hashes
- Commit blobs and metadata
- Create an organized file storage format

## Key Challenges and Solutions

### 1. Code Modularization
**Problem:** Duplicate definitions and unclear responsibilities

**Solution:** 
- Separate file extraction logic (extract_argument.c/h)
- Keep commit storage logic in commit_storage.c/h
- Include extract_argument.h in commit_storage.h

### 2. Library and Constant Issues
**Problems:**
- Undefined constants (SHA256_DIGEST_LENGTH)
- Missing system libraries

**Solution:**
- Include OpenSSL library
- Add system headers (sys/stat.h, sys/types.h)

### 3. Path and Directory Management
**Problem:** Constructing and creating storage paths

**Solution:** 
- Create a `create_directory()` helper function
- Use `stat()` to check directory existence
- Use `mkdir()` to create directories as needed

## Design Decisions

### File Storage Strategy
- Use first two characters of SHA256 hash as directory name
- Use remaining hash characters as filename
- Prevents file collisions
- Enables efficient file organization

### Extraction Process
1. Read staged_files.txt
2. Populate StagedFile structure
3. Track number of staged files
4. Store each file blob in hash-based directory

## Implementation Workflow

### Blob Storage Function
- Convert hash to hexadecimal
- Create directory based on hash prefix
- Store file content in appropriate directory

## Key Reflections

### Modular Approach Benefits
- Improved code maintainability
- Enhanced reusability
- Clear separation of concerns
- Better scalability

### Directory Hashing Advantages
- Reduced collision risk
- Logical file organization
- Efficient storage mechanism
