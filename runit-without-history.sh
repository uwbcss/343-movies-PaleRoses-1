#!/bin/bash

# Compile and run the movie store system WITHOUT History command support

echo "====================================================="
echo "Compiling WITHOUT History command support"
echo "====================================================="

# Clean up any existing executable
rm ./a.out 2>/dev/null

# Compile all source files EXCEPT history_command.cpp
g++ -I./header -g -Wall -Wextra -Wno-sign-compare \
    main.cpp \
    store_test.cpp \
    src/classic.cpp \
    src/comedy.cpp \
    src/drama.cpp \
    src/store.cpp \
    src/inventory_command.cpp \
    src/borrow_command.cpp \
    src/return_command.cpp

if [ $? -eq 0 ]; then
    echo "====================================================="
    echo "Compilation successful - running program"
    echo "History commands will show as 'Unknown command type: H'"
    echo "====================================================="
    ./a.out
else
    echo "Compilation failed"
    exit 1
fi

rm ./a.out 2>/dev/null