cmake_minimum_required(VERSION 3.10)
project(ccrack)

find_package(OpenSSL REQUIRED)
add_executable(ccrack "main.c" "ccrack.c")
target_link_libraries(ccrack OpenSSL::SSL OpenSSL::Crypto)
