cmake_minimum_required(VERSION 3.15)
project(password_manager C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories for headers
include_directories(include)

# Source files
set(SOURCE_FILES
        main.c
        src/crypto.c
        src/password.c
        src/storage.c
        src/utils.c
)

add_executable(password_manager ${SOURCE_FILES})

# Link OpenSSL
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(password_manager OpenSSL::SSL OpenSSL::Crypto)
else ()
    message(FATAL_ERROR "OpenSSL not found!")
endif ()

# Enable testing
enable_testing()
add_subdirectory(tests)
add_custom_target(run_all
        COMMAND ${CMAKE_CTEST_COMMAND} --verbose
        COMMAND ${CMAKE_COMMAND} -E echo "Launching password_manager..."
        COMMAND password_manager
        DEPENDS password_manager
        USES_TERMINAL
)

