cmake_minimum_required(VERSION 3.17)
project(dc_application C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_library(dc_application SHARED application.c)
target_compile_options(dc_application PRIVATE -Wpedantic -Wall -Wextra)

install(TARGETS dc_application DESTINATION lib)
install(FILES application.h config.h DESTINATION include/dc_application)
