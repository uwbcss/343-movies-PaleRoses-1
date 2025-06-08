/**
 * @file commands.h
 * @location header/commands.h
 * @brief All concrete command classes for the movie store system
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "command.h"
#include <string>

/**
 * @brief Command to display entire movie inventory
 */
class InventoryCommand : public Command {
public:
  InventoryCommand() = default;
  virtual ~InventoryCommand() = default;

  bool execute(Store &store) override;
  char getCommandType() const override;
  Command *clone() const override;
  bool setParameters(std::istream &input) override;
  std::string getDescription() const override;
};

/**
 * @brief Command to display customer transaction history
 */
class HistoryCommand : public Command {
public:
  HistoryCommand();
  virtual ~HistoryCommand() = default;

  bool execute(Store &store) override;
  char getCommandType() const override;
  Command *clone() const override;
  bool setParameters(std::istream &input) override;
  std::string getDescription() const override;

private:
  std::string customerID;
};

/**
 * @brief Command to borrow a movie from inventory
 */
class BorrowCommand : public Command {
public:
  BorrowCommand();
  virtual ~BorrowCommand() = default;

  bool execute(Store &store) override;
  char getCommandType() const override;
  Command *clone() const override;
  bool setParameters(std::istream &input) override;
  std::string getDescription() const override;

private:
  std::string customerID;
  char mediaType;
  char movieType;
  std::string movieSearchKey;
  std::string movieTitle; // For error messages
};

/**
 * @brief Command to return a borrowed movie
 */
class ReturnCommand : public Command {
public:
  ReturnCommand();
  virtual ~ReturnCommand() = default;

  bool execute(Store &store) override;
  char getCommandType() const override;
  Command *clone() const override;
  bool setParameters(std::istream &input) override;
  std::string getDescription() const override;

private:
  std::string customerID;
  char mediaType;
  char movieType;
  std::string movieSearchKey;
  std::string movieTitle; // For error messages
};

#endif // COMMANDS_H