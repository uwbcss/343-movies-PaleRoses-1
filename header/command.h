/**
 * @file command.h
 * @location header/command.h
 * @brief Abstract base class for all command types
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>

// Forward declaration
class Store;

class Command {
  public:
    virtual ~Command() = default;

    // Execute the command on the store
    virtual bool execute(Store &store) = 0;

    // Return command type code: 'B', 'R', 'I', 'H'
    virtual char getCommandType() const = 0;

    // Virtual constructor pattern
    virtual Command *clone() const = 0;

    // Parse parameters from input stream
    virtual bool setParameters(std::istream &input) = 0;

    // Get description for error messages
    virtual std::string getDescription() const = 0;

  protected:
    Command() = default;

    // Validate 4-digit customer ID
    bool isValidCustomerID(const std::string &id) const {
        if (id.length() != 4)
            return false;
        for (char c : id) {
            if (!std::isdigit(c))
                return false;
        }
        return true;
    }

    // Check if media type is 'D'
    bool isValidMediaType(char mediaType) const { return mediaType == 'D'; }

    // Print error with consistent formatting
    void reportError(const std::string &message) const {
        std::cerr << "==========================\n";
        std::cerr << message << "\n";
        std::cerr << "==========================\n";
    }
};

#endif // COMMAND_H