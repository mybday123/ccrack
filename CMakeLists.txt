cmake_minimum_required(VERSION 3.10)
project(ccrack)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
add_executable(cracc "main.c" "cracc.c")
target_link_libraries(ccrack OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
