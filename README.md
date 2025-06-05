# 🔐 Password Manager in C

A secure command-line password manager written in C, using **OpenSSL** for encryption and **CMocka** for unit testing.

> Developed as a software engineering project focused on secure storage, encryption, modular C design, and testing.

---

## 📁 Project Structure

```text
password_manager/
├── include/                 # Header files
│   ├── crypto.h             # Crypto-related functions
│   ├── password.h           # Entry structure and logic
│   ├── storage.h            # File handling and encryption
│   └── utils.h              # Utility functions
├── src/                     # Source files
│   ├── crypto.c
│   ├── password.c
│   ├── storage.c
│   └── utils.c
├── tests/                   # Unit tests with CMocka
│   ├── CMakeLists.txt
│   ├── test_crypto.c
│   ├── test_password.c
│   └── test_storage.c
├── docs/                    # Project documentation
│   ├── requirements.md
│   ├── design.md
│   └── future.md
├── main.c                   # Entry point for CLI app
├── CMakeLists.txt
└── README.md                # Project overview and instructions
```

---

## ⚙️ Requirements

- macOS or Linux
- [Homebrew](https://brew.sh) (for installing libraries)
- [CMake ≥ 3.15](https://cmake.org/)
- [OpenSSL](https://www.openssl.org/)
- [CMocka](https://cmocka.org/) for unit testing
- [CLion](https://www.jetbrains.com/clion/) or terminal-based CMake build

---

## 📥 Setup Instructions (macOS)

```bash
# Install required dependencies
brew install openssl cmake cmocka

# Clone this repo
git clone https://github.com/PabloRDev/password_manager.git
cd password_manager

# Configure the project
cmake -B build

# Build the app
cmake --build build

# Run tests
cd build
ctest --verbose

# Run the app
./password_manager
```