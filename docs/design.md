# 🧠 Design Specification

## 1. Overview

This document describes the architecture and design decisions for a secure password manager. It focuses
on modular design, encapsulation, and secure data handling.

---

## 2. High-Level Architecture

### 🔧 Modules

| Module       | Purpose                                      |
|--------------|----------------------------------------------|
| `main.c`     | User interface and main menu loop            |
|
| `password.*` | Structures and logic for password entries    |
| `crypto.*`   | Encryption and hashing with OpenSSL          |
| `storage.*`  | File handling, saving/loading encrypted data |
| `utils.*`    | Miscellaneous helper functions               |

---

## 3. Component Diagram

```text
[main.c]
├──> [password.h] (Entry creation and validation)
├──> [storage.h] (Save/load encrypted file)
├──> [crypto.h] (Encrypt/decrypt, hash password)
└──> [utils.h] (Prompt, string sanitization, etc.)
```

---

## 4. Data Structures

| Purpose                         | Data Structure Type             |
|---------------------------------|---------------------------------|
| Store multiple password entries | **Static array**                |
| Manage input buffers            | **Fixed-size character arrays** |
| Search and filter entries       | **Linear traversal of array**   |
| Store master password hash/salt | **Struct (hash + salt)**        |
|

---

## 5. File Format

A binary file (`vault.dat`) stores all password entries and authentication metadata.

- The file contains:
    - A salt (16 bytes) used for key derivation from the master password.
    - The SHA-256 hash of the master password (32 bytes).
    - A serialized array of password entries.
- All contents (except the salt) are encrypted using AES-256-CBC via OpenSSL.
- On application startup, the file is decrypted and the hash is used to verify the user-provided master password.

---

## 6. Cryptography Design

- **Encryption algorithm:** AES-256 in CBC mode using OpenSSL.
- **Key derivation:** SHA-256 applied to (salt + master password).
- **Master password verification:**
    - On first run, a salt is generated and stored alongside the hash of the master password.
    - On subsequent runs, the same salt is used to re-derive the key and check the stored hash.
- **Vault integrity:** The full password vault is encrypted as one block to simplify integrity and decryption.

---

## 7. User Interaction Flow

```text
   [ Launch App ]
↓
[ Enter Master Password ]
↓
[ Authenticated? ]
├── No → Exit
└── Yes
↓
[ Show Menu ]
├── Add Entry
├── View Entries
├── Search Entries
├── Edit/Delete
└── Exit
   ```

---

## 8. Unit Testing Plan

Each module is tested individually using CMocka:

- `test_crypto.c`: AES encryption/decryption, hash verification
- `test_password.c`: Validation, entry comparison
- `test_storage.c`: File I/O and serialization

Integration and edge case testing will verify:

- Corrupted or missing vault files
- Invalid master password inputs
- File permission or path errors

---

## 9. Build System

CMake project with the following layout:

- include/
- src/
- tests/
- docs/
- `main.c`
- CMakeLists.txt
- README.md

External dependencies:

- OpenSSL (crypto)
- CMocka (testing)
