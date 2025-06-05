# 📄 Requirements Specification

## 1. Introduction

This document defines the functional and non-functional requirements for this command-line password manager.
It targets students and developers who want a simple, secure, offline password management tool.

---

## 2. Functional Requirements

### 2.1 Master Password Authentication

- On launch, the application prompts for a master password.
- Password is hashed and compared to a stored, hashed version.
- Access is denied if the password is incorrect.

### 2.2 Add Password Entry

- User can add a new password entry:
    - Service name
    - Username
    - Password
    - Notes (optional)

### 2.3 List All Entries

- Lists all stored services (not showing the actual passwords).
- Requires master password to access.

### 2.4 View Entry Details

- User selects a service and views full decrypted details.

### 2.5 Edit Entry

- Modify one or more fields in a password entry.

### 2.6 Delete Entry

- Select and delete a saved password entry.

### 2.7 Search

- Search by service name or username.

### 2.8 Data Persistence

- Data is saved to an encrypted file on disk and persists between sessions.

---

## 3. Non-Functional Requirements

### 3.1 Security

- Data is encrypted using AES-256 (via OpenSSL).
- Master password is hashed using SHA-256 + salt.
- Sensitive data is cleared from memory after use.

### 3.2 Usability

- Terminal interface.
- Clear navigation and prompts.

### 3.3 Portability

- Compatible with Unix-like systems (macOS, Linux).

### 3.4 Performance

- Designed for up to ~1000 entries with minimal delay.

### 3.5 Robustness

- Handles invalid input and corrupted storage files gracefully.

---

## 4. Constraints & Assumptions

- Single-user local application.
- No cloud storage or network access.
- One encrypted file stores all entries.


