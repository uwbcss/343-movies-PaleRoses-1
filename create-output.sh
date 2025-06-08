#!/bin/bash

# Run this script as `./create-output.sh > output.txt 2>&1`

# How we want to call our executable,
# possibly with some command line parameters
EXEC_PROGRAM="./a.out "

# Timestamp for starting this script
date

MACHINE=""
# Display machine name if uname command is available
if hash uname 2>/dev/null; then
  uname -a
  MACHINE=`uname -a`
fi

# Display user name if id command is available
if hash id 2>/dev/null; then
  id
fi

# delete a.out, do not give any errors if it does not exist
rm ./a.out 2>/dev/null

echo "====================================================="
echo "1. If the section below is empty, the program compiles "
echo "   without warnings with -Wall -Wextra flags"
echo "====================================================="

g++ -I./header -g -Wall -Wextra -Wno-sign-compare *.cpp src/*.cpp

echo "====================================================="
echo "2. If the section below is empty or has the expected output "
echo "    the program puns and produces correct output"
echo "====================================================="

# Execute program
$EXEC_PROGRAM

echo "====================================================="
echo "3. If the section below is empty, then there are no clang-tidy warnings "
echo "   (ignore warnings from system headers, such as \"13554 warnings generated.\")"
echo "====================================================="

if hash clang-tidy 2>/dev/null; then
  clang-tidy *.cpp src/*.cpp -- -I./header
else
  echo "WARNING: clang-tidy not available."
fi

echo "====================================================="
echo "4. If the section below is empty, clang-format does not find any formatting issues"
echo "   You can fix formatting errors using \"clang-format -i <filename>\" on command line"
echo "====================================================="

if hash clang-format 2>/dev/null; then
  # different LLVMs have slightly different configurations which can break things, so regenerate
  echo "# generated using: clang-format -style=llvm -dump-config > .clang-format" > .clang-format
  clang-format -style=llvm -dump-config >> .clang-format
  for f in ./*.cpp src/*.cpp header/*.h; do
    echo "Running clang-format on $f"
    clang-format $f | diff $f -
  done
else
  echo "WARNING: clang-format not available"
fi

echo "====================================================="
echo "5. If there are any memory leaks, it should be in the output below"
echo "====================================================="

rm ./a.out 2>/dev/null

g++ -I./header -fsanitize=address -fno-omit-frame-pointer -g *.cpp src/*.cpp
# Execute program
$EXEC_PROGRAM > /dev/null 2> /dev/null


echo "====================================================="
echo "6. Using valgrind to check for memory leaks"
echo "   Check for \"definitely lost\" in the output below"
echo "====================================================="

rm ./a.out 2>/dev/null

if hash valgrind 2>/dev/null; then
  g++ -I./header -g *.cpp src/*.cpp
  # redirect program output to /dev/null will running valgrind
  valgrind --log-file="valgrind-output.txt" $EXEC_PROGRAM > /dev/null 2>/dev/null
  cat valgrind-output.txt
  rm valgrind-output.txt 2>/dev/null
else
  echo "WARNING: valgrind not available"
fi

echo "====================================================="
echo "  ✨🌙✨    7. Tests have full code coverage ✨🌙✨"
echo "  ✨🌙✨ The lines below were never executed ✨🌙✨"
echo "====================================================="

# Clean any existing coverage files
rm -f *.gcov *.gcda *.gcno src/*.gcov src/*.gcda src/*.gcno 2>/dev/null
rm ./a.out 2>/dev/null

if hash gcov 2>/dev/null; then
  echo "Compiling with coverage instrumentation..."
  
  # Compile with coverage flags
  if g++ -I./header -fprofile-arcs -ftest-coverage *.cpp src/*.cpp -o a.out 2>/dev/null; then
    
    # Execute program to generate coverage data
    echo "Generating coverage data..."
    $EXEC_PROGRAM > /dev/null 2>&1
    
    echo ""
    echo "COVERAGE SUMMARY:"
    echo "-----------------"
    
    # Process each source file and capture gcov output directly
    for file in main.cpp store_test.cpp; do
      if [ -f "$file" ]; then
        echo -n "Processing $file: "
        gcov "$file" 2>/dev/null | grep -A1 "File '$file'" | grep "Lines executed"
      fi
    done
    
    # Process source directory files
    for file in src/*.cpp; do
      if [ -f "$file" ]; then
        filename=$(basename "$file")
        echo -n "Processing $filename: "
        gcov "$file" 2>/dev/null | grep -A1 "File.*$filename" | grep "Lines executed"
      fi
    done
    
    echo ""
    echo "UNCOVERED CODE DETECTION:"
    echo "-------------------------"
    
    # Generate .gcov files to check for uncovered lines
    gcov *.cpp src/*.cpp > /dev/null 2>&1
    
    # Check if any uncovered lines exist
    uncovered_found=false
    for gcov_file in *.gcov; do
      if [ -f "$gcov_file" ] && grep -q "#####:" "$gcov_file"; then
        uncovered_found=true
        break
      fi
    done
    
    if [ "$uncovered_found" = true ]; then
      echo "Some lines were not executed during testing:"
      echo ""
      for gcov_file in *.gcov; do
        if [ -f "$gcov_file" ]; then
          uncovered=$(grep -n "#####:" "$gcov_file" 2>/dev/null | head -5)
          if [ ! -z "$uncovered" ]; then
            echo "In $gcov_file:"
            echo "$uncovered"
            echo ""
          fi
        fi
      done
    else
      echo "✨ All executable lines have been covered by tests! ✨"
    fi
    
  else
    echo "ERROR: Failed to compile with coverage flags"
  fi
  
  # Clean up all coverage files
  rm -f *.gcov *.gcda *.gcno src/*.gcov src/*.gcda src/*.gcno 2>/dev/null
  
else
  echo "WARNING: gcov not available for coverage analysis"
fi

# Remove the executable
rm -rf ./a.out* 2>/dev/null

date

echo "====================================================="
echo "To create an output.txt file with all the output from this script"
echo "Run the below command"
echo "      ./create-output.sh > output.txt 2>&1 "
echo "====================================================="
