/**
 * @file main.cpp
 *
 * This program initializes the movie store with inventory and customer data,
 * then processes a series of commands from a file.
 */

#include "header/store.h"
#include <exception>
#include <iostream>
#include <string>

int main() {
    try {
        // Create the store instance
        Store movieStore;

        // File paths for data files
        const std::string movieFile = "data4movies.txt";
        const std::string customerFile = "data4customers.txt";
        const std::string commandFile = "data4commands.txt";

        // Initialize the store with movie and customer data
        if (!movieStore.initialize(movieFile, customerFile)) {
            std::cerr << "Failed to initialize store with data files"
                      << std::endl;
            return 1;
        }

        // Process commands from the command file
        if (!movieStore.processCommands(commandFile)) {
            std::cerr << "Failed to process command file" << std::endl;
            return 1;
        }

        std::cout << "Done!" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}