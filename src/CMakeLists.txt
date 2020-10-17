cmake_minimum_required(VERSION 3.7)
project(IFJ)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "$(CMAKE_C_FLAGS) -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread")

set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread")

set(SOURCE_FILES main.c)

add_executable(main ${SOURCE_FILES})