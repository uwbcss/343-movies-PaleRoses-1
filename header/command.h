/**
 * @location header/command.h
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <algorithm>
#include <cctype>
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
  static bool isValidCustomerID(const std::string &id) {
    if (id.length() != 4) {
      return false;
    }
    return std::all_of(id.begin(), id.end(),
                       [](char c) { return std::isdigit(c); });
  }

  // Check if media type is 'D'
  static bool isValidMediaType(char mediaType) { return mediaType == 'D'; }

  // Print error with consistent formatting
  static void reportError(const std::string &message) {
    std::cerr << "==========================\n";
    std::cerr << message << "\n";
    std::cerr << "==========================\n";
  }
};

#endif // COMMAND_H