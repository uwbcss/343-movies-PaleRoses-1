#!/bin/bash

# Compile and run the movie store system WITHOUT Classic movie support

echo "====================================================="
echo "Compiling WITHOUT Classic movie support"
echo "====================================================="

# Clean up any existing executable
rm ./a.out 2>/dev/null

# Compile all source files EXCEPT classic.cpp
# Note: we must now compile individual command files instead of commands.cpp
g++ -I./header -g -Wall -Wextra -Wno-sign-compare \
    main.cpp \
    store_test.cpp \
    src/comedy.cpp \
    src/drama.cpp \
    src/store.cpp \
    src/inventory_command.cpp \
    src/history_command.cpp \
    src/borrow_command.cpp \
    src/return_command.cpp

if [ $? -eq 0 ]; then
    echo "====================================================="
    echo "Compilation successful - running program"
    echo "Classic movies will show as 'Unknown movie type: C'"
    echo "====================================================="
    ./a.out
else
    echo "Compilation failed"
    exit 1
fi

rm ./a.out 2>/dev/null