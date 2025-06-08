#!/bin/bash

# Compile and run the movie store system WITHOUT Borrow command support

echo "====================================================="
echo "Compiling WITHOUT Borrow command support"
echo "====================================================="

# Clean up any existing executable
rm ./a.out 2>/dev/null

# Compile all source files EXCEPT borrow_command.cpp
g++ -I./header -g -Wall -Wextra -Wno-sign-compare \
    main.cpp \
    store_test.cpp \
    src/classic.cpp \
    src/comedy.cpp \
    src/drama.cpp \
    src/store.cpp \
    src/inventory_command.cpp \
    src/history_command.cpp \
    src/return_command.cpp

if [ $? -eq 0 ]; then
    echo "====================================================="
    echo "Compilation successful - running program"
    echo "Borrow commands will show as 'Unknown command type: B'"
    echo "====================================================="
    ./a.out
else
    echo "Compilation failed"
    exit 1
fi

rm ./a.out 2>/dev/null