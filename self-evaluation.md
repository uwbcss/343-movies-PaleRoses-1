# Self-Evaluation

## Name(s):

Rosalia Fialkova

Out of 25 points. Use output.txt created using
`./create-output.sh > output.txt 2>&1` for guidance.

Complete all questions with "Q:"

Q: Does the program compile and run to completion: Yes/No

Yes.

- If the program does not compile or gives a segmentation error when run,
  the maximum possible grade is 50%. No need to continue with the rest of self-evaluation

Q: All public functions have been implemented: ~100

There was no pre-defined list of "public functions." Therefore the number has ballooned into dozens.

- -2 for each functionality not implemented

For each command, state Full, Partial or None to indicate
if it has been fully, partially or not implemented at all.
Explain any partial implementations.

Inventory: FULL
History: FULL
Borrow: FULL
Return: FULL

Q: -1 for each compilation warning, min -3: 0

No compilation warnings!

- Check under _1. Compiles without warnings_
- If the warning message is addressed in README.md, including how the programmer tried to address it, no deductions

Q: -1 for each clang-tidy warning, min -3: 0

Ran the command to fix tidy warnings.

- Check under _3. clang-tidy warnings_
- If the warning message is addressed in README.md, including how the programmer tried to address it, no deductions

Q: -1 for each clang-format warning, min -3: 0

Ran command to fix format warnings.

- Check under _4. clang-format does not find any formatting issues_

Q: -2 for any detected memory leak: 0

- Check under _5. No memory leaks using g++_
- Check under _6. No memory leaks using valgrind_

Q: Do the tests sufficiently test the code: Yes.

An attempt was made to adjust create-out.sh to provide another segment, that revoles around test coverage.

- -1 for each large block of code not executed
- -2 for each function that is never called when testing
- Check under _7. Tests have full code coverage_ paying attention to _The lines below were never executed_

Q: Are all functions in .h and .cpp file documents (min -3): Yes, all, see below.

- -1 for each function not documented

## Location of error message or functionality

State the file and function where the information can be found
USED markdown format to make this legible!!
**Invalid Command Code**  
`src/store.cpp`, `processCommandLine()` → outputs "Unknown command type: X, discarding line:"

**Invalid Movie Type**  
`src/store.cpp`, `processMovieLine()` → outputs "Unknown movie type: Z, discarding line:"

**Invalid Customer ID**  
`src/commands.cpp`, `BorrowCommand::setParameters()` and `ReturnCommand::setParameters()` → outputs "Invalid customer ID XXXX, discarding line:"

**Invalid Movie**  
`src/commands.cpp`, `BorrowCommand::execute()` and `ReturnCommand::execute()` → outputs "Invalid movie for customer Name, discarding line:"

**Factory Classes**  
`header/factory.h` → contains `MovieFactory` and `CommandFactory` singleton classes

**Hashtable**  
`header/hashtable.h` → custom hash table used in `Store` class to store customers for O(1) lookup by 4-digit ID

**Container Used for Comedy Movies**  
`BSTree<Movie*>` stored in `Store::genreTrees['F']`

**Function for Sorting Comedy Movies**  
`Comedy::operator<()` in `src/comedy.cpp`

**Function Where Comedy Movies Are Sorted**  
`Store::addMovie()` in `src/store.cpp` when calling `tree->insert()`

**Functions Called When Retrieving a Comedy Movie Based on Title and Year**  
`Store::findMovie()` → `BSTree::findByPredicate()` → `Comedy::getSearchKey()`

**Functions Called for Retrieving and Printing Customer History**  
`HistoryCommand::execute()` → `Customer::displayHistory()`

**Container Used for Customer History**  
`std::vector<Transaction>` in `Customer` class (`header/customer.h`)

**Functions Called When Borrowing a Movie**  
`BorrowCommand::execute()` → `Movie::borrowMovie()` → `Customer::addTransaction()`

**Explain Borrowing a Movie That Does Not Exist**  
`BorrowCommand::execute()` checks if `findMovie()` returns nullptr, then outputs error message

**Explain Borrowing a Movie That Has 0 Stock**  
`Movie::borrowMovie()` returns false when stock <= 0, `BorrowCommand::execute()` outputs "could NOT borrow Title, out of stock:"

**Explain Returning a Movie That Customer Has Not Checked Out**  
`ReturnCommand::execute()` calls `Customer::hasMovieBorrowed()` which returns false if not borrowed, outputs error message

**Any Static_cast or Dynamic_cast Used**  
None!

## Bonus +5

Are there multiple files, of the form runit-without-XXX, where the same set of files will compile and run excluding some of the commands or genres?

**Yes.** Five runit-without-XXX scripts demonstrate this idea.

- **runit-without-classic.sh** - Excludes Classic movie support
- **runit-without-comedy.sh** - Excludes Comedy movie support
- **runit-without-borrow.sh** - Excludes Borrow command
- **runit-without-return.sh** - Excludes Return command
- **runit-without-history.sh** - Excludes History command

Each script compiles and runs successfully with the excluded component generating appropriate "Unknown type" errors while all other functionality remains operational.

Q: Total points: ADD_ALL_POINTS (max 25 (+ 5 Bonus))

30/25
