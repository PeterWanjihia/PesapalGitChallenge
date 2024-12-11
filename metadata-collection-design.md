# Metadata Collection: The Backbone of the Commit System

## Introduction

The commit system is fundamentally based on managing and tracking changes in a version-controlled project. The metadata associated with each commit plays a critical role in ensuring the accuracy, traceability, and integrity of the project's history.

### Metadata Components
Metadata includes essential details such as:
- Commit message
- Author information
- Timestamps
- File hashes
- Parent commit references
- Commit IDs

## Why Is Metadata Collection Critical?

As we designed the commit system, we realized that metadata is the cornerstone of any version control system. It provides:

1. **Identity & Traceability**
   - Who made the change
   - When it was made
   - What exactly was changed

2. **History Management**
   - Enables tracking the evolution of the codebase over time

3. **Content Integrity**
   - Ensures every file included in the commit is verified and uniquely identified

> Note: In Git, metadata is handled extremely efficiently, and our system aims to mimic these processes, even if on a smaller scale.

## Designing the Metadata Collection Logic

### 1. What Metadata Do We Need?

We identified the most important pieces of metadata for a commit:

- **Commit Message**
  - User-provided description of the commit's purpose

- **Author Information**
  - Name and email of the person making the commit

- **Commit Timestamp**
  - Exact time of commit occurrence

- **Staged File Hashes**
  - Unique identifier (hash) for each file in the staging area

- **Parent Commit Reference(s)**
  - Reference (ID) to the previous commit
  - Helps establish project history

- **Commit ID**
  - Unique hash identifying the commit based on all other metadata

### 2. Metadata Collection Strategy

#### Commit Message
- Provided by user through command line argument (-m)
- Read directly from command input

#### Author Information
- Prompt user for name and email during commit process
- Simple and clear solution

#### Commit Timestamp
- Generated programmatically using system time functions
- Reflects exact commit creation time

#### Staged File Hashes
- Computed by hashing file contents
- Retrieval from staging area at commit time

#### Parent Commit Reference
- Automatically retrieved from current branch's head
- Null for first commit
- Linked to parent commit for subsequent commits

#### Commit ID
- Hash derived from all metadata
- Ensures unique and deterministic identification

## Code Organization

### File Structure

1. **`commit_metadata.c`**
   - Implements metadata collection logic
   - Handles user interactions
   - Generates timestamps
   - Manages staged file hashes

2. **`commit_utils.h`**
   - Declares function prototypes
   - Serves as interface for metadata functions
   - Enables modular design

3. **`test_metadata.c`**
   - Main testing file for metadata collection
   - Ensures function correctness
   - Validates metadata gathering process

## Detailed File Breakdown

### commit_metadata.c

#### Purpose
Gather and store metadata

#### Key Functions
- `get_commit_author()`
  - Collects author's name and email
- `get_commit_timestamp()`
  - Generates formatted timestamp
- `get_staged_files_hashes()`
  - Computes hashes for staged files

### commit_utils.h

#### Purpose
Declare function prototypes

#### Example Prototypes
```c
AuthorInfo get_commit_author();
const char* get_commit_timestamp();
void get_staged_files_hashes(char* staged_file_hashes[]);
```

### test_metadata.c

#### Purpose
Test metadata collection logic

#### Key Activities
- Test all metadata collection functions
- Log collected metadata
- Verify data integrity

## Design Rationale

### Advantages of Our Approach

1. **Modularity**
   - Organized, maintainable code
   - Clear separation of concerns

2. **Testability**
   - Functions can be tested in isolation
   - Easier debugging and error fixing

3. **Clarity and Reusability**
   - Reusable functions
   - Reduced code redundancy

4. **User Interaction**
   - Intuitive user prompts
   - Mimics real-world version control systems

## Testing Strategy

### Pre-Integration Testing
1. Run `test_metadata.c`
2. Verify metadata collection functions
3. Log collected data
4. Confirm correct parsing of:
   - Commit messages
   - Author information
   - File hashes
