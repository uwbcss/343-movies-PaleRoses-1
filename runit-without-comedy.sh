#!/bin/bash

# Compile and run the movie store system WITHOUT Comedy movie support

echo "====================================================="
echo "Compiling WITHOUT Comedy movie support"
echo "====================================================="

# Clean up any existing executable
rm ./a.out 2>/dev/null

# Compile all source files EXCEPT comedy.cpp
g++ -I./header -g -Wall -Wextra -Wno-sign-compare \
    main.cpp \
    store_test.cpp \
    src/classic.cpp \
    src/drama.cpp \
    src/store.cpp \
    src/inventory_command.cpp \
    src/history_command.cpp \
    src/borrow_command.cpp \
    src/return_command.cpp

if [ $? -eq 0 ]; then
    echo "====================================================="
    echo "Compilation successful - running program"
    echo "Comedy movies will show as 'Unknown movie type: F'"
    echo "====================================================="
    ./a.out
else
    echo "Compilation failed"
    exit 1
fi

rm ./a.out 2>/dev/null