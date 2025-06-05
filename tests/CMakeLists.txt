cmake_minimum_required(VERSION 3.15)

# Enable testing support
enable_testing()

# Include directories for your project headers and Homebrew includes
include_directories(${CMAKE_SOURCE_DIR}/include /opt/homebrew/include)

# Link directories for Homebrew libs (cmocka, openssl)
link_directories(/opt/homebrew/lib)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Test: Crypto
add_executable(test_crypto
        test_crypto.c
        ../src/crypto.c
)
target_link_libraries(test_crypto
        cmocka
)
add_test(NAME CryptoTest COMMAND test_crypto)

# Test: Password
add_executable(test_password
        test_password.c
        ../src/password.c
        ../src/utils.c
)
target_link_libraries(test_password
        cmocka
)
add_test(NAME PasswordTest COMMAND test_password)

# Test: Storage (needs OpenSSL linked)
add_executable(test_storage
        test_storage.c
        ../src/storage.c
        ../src/utils.c
)
target_link_libraries(test_storage
        cmocka
        OpenSSL::SSL
        OpenSSL::Crypto
)
add_test(NAME StorageTest COMMAND test_storage)
