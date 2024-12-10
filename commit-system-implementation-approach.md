# Order of Approach for Commit System Implementation

## Overview
To implement the commit system step by step, we need to carefully plan the sequence of tasks and ensure that each component is properly integrated. Here's a logical sequence for implementing the commit system in the `commit/` folder.

## Step 1: Core Commit Object Creation

### Purpose
The commit object is the foundation of the commit system. Without this, we can't store or track any commit data.

### Commit Object Components
- Staged files (file hashes)
- Commit message
- Timestamp
- Parent commit (if any)

### Tasks in Step 1
1. Implement the function `create_commit_object()`.
2. Ensure that the staged files (from the staging area) are included.
3. Handle metadata like commit message, author, and timestamp.

### Why Start Here?
The commit object is the heart of the commit system. If we don't have a valid commit object, the entire system won't make sense. This should be our first priority.

### Imports/Variables
- `STAGED_FILE_PATH`: Staged files are stored in a specific directory (`.repo/staging/`), and we need to read these staged files in `create_commit_object()` to fetch their hashes.
- `COMMIT_MESSAGE`: We need a commit message to store in the commit metadata.
- `AUTHOR_INFO` (optional): For simplicity, this can be hardcoded initially or read from an environment variable.
- `TIMESTAMP`: This can be generated using `time(NULL)` in C.

## Step 2: Commit ID Generation

### Purpose
Create a unique commit ID by hashing the commit content.

### Commit ID Components
- Staged file hashes
- Commit message
- Parent commit (if it exists)
- Timestamp
- Author info

### Tasks in Step 2
1. Implement `generate_commit_id()`.
2. Ensure the commit ID is deterministic (using the same commit content always results in the same commit ID).

### Imports/Variables
- `commit_hashing.h` (or the existing `hashing.h`): This can help us hash the commit content (commit message, files, etc.).
- `staging.h`: To access staged files.

## Step 3: Storing the Commit Object

### Purpose
Store the commit object in the repository's object storage (typically inside `.repo/objects/`).

### Tasks in Step 3
1. Implement `store_commit_object()` to write the commit object to disk, using the commit ID as the filename.

### Imports/Variables
- `COMMIT_OBJECT_PATH`: We can store commit objects in `.repo/objects/`, with the commit ID as the file name.
- `create_commit_object()`: We will need to call this to fetch the commit object to be stored.

## Step 4: Commit History Management

### Purpose
Ensure the commit history is updated and each commit points to its parent commit.

### Tasks in Step 4
1. Implement `update_commit_history()`, which updates the commit log (history).
2. Update the branch reference to point to the new commit.

### Imports/Variables
- `COMMIT_HISTORY_PATH`: This stores the history of commits (e.g., `.repo/refs/heads/main`).
- `update_branch_reference()`: This updates the current branch to point to the new commit.

## Step 5: Branch Management

### Purpose
Update branch references so that the correct branch points to the latest commit.

### Tasks in Step 5
1. Implement `update_branch_reference()` to modify the branch pointer.

## Step 6: Validation and Error Handling

### Purpose
Validate staged files, changes, and commit message before committing.

### Tasks in Step 6
1. Implement `validate_staged_files()`.
2. Implement `check_for_changes()`.
3. Implement `validate_commit_message()`.

These should be called before making a commit to ensure everything is correct.

## Step 7: Integrating Everything into a Workflow

### Purpose
Combine individual functions into the main commit workflow.

### Tasks in Step 7
1. Implement `commit_changes()`, which ties together commit creation, history management, and branch updates.

## Testing the Commit System Incrementally

### Testing Approach
We will test each step as we go:

1. **Test Commit Object Creation:** Test the `create_commit_object()` function. This function should be able to generate the commit metadata, store the staged files, and include all required metadata.

2. **Test Commit ID Generation:** After creating the commit object, test `generate_commit_id()` to ensure it produces the correct and deterministic commit ID.

3. **Test Commit Object Storage:** Ensure that the commit object is correctly saved in the `.repo/objects/` directory.

4. **Test Commit History Update:** After storing the commit object, test `update_commit_history()` to ensure the commit history is updated with the new commit.

5. **Test Branch Update:** Ensure that the branch pointer is updated correctly after a commit.

## File Imports and Variables in the `commit/` Folder

### Key Variables

1. **`STAGED_FILE_PATH`** (e.g., `.repo/staging/`)
   - Needed in functions like `create_commit_object()` to retrieve staged files and their hashes.

2. **`COMMIT_MESSAGE`**
   - Used to create the commit message for the commit object.

3. **`COMMIT_HISTORY_PATH`**
   - Path to store commit history (e.g., `.repo/refs/heads/main`).

4. **`COMMIT_OBJECT_PATH`**
   - Directory to store commit objects (e.g., `.repo/objects/`).

5. **`AUTHOR_INFO`** (optional)
   - Could be retrieved via environment variables like `GIT_AUTHOR_NAME` and `GIT_AUTHOR_EMAIL`.

### Importing Other Modules

- `staging.h`: For staged files and handling staged data.
- `hashing.h`: For commit ID generation and hashing staged files.
- `time.h`: For generating timestamps.
- `commit.h`: For function prototypes specific to commit operations.
