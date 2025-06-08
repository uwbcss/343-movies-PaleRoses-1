/**
 * @location src/history_command.cpp
 */

#include "commands.h"
#include "customer.h"
#include "factory.h"
#include "store.h"
#include <iostream>

// Self-registration with factory
namespace {
class HistoryRegistrar {
public:
  HistoryRegistrar() {
    CommandFactory::getInstance().registerCommandType(
        'H', []() { return std::make_unique<HistoryCommand>(); });
  }
};
HistoryRegistrar historyRegistrar;
} // namespace

HistoryCommand::HistoryCommand() {}

bool HistoryCommand::execute(Store &store) {
  // Debug output as shown in sample
  std::cout << "Debug: History for " << customerID;

  Customer *customer = store.findCustomer(customerID);
  if (customer == nullptr) {
    reportError("Customer " + customerID + " not found");
    return false;
  }

  std::cout << " " << customer->getDisplayName() << "\n";
  std::cout << "==========================\n";

  customer->displayHistory(std::cout);

  return true;
}

char HistoryCommand::getCommandType() const { return 'H'; }

Command *HistoryCommand::clone() const { return new HistoryCommand(*this); }

bool HistoryCommand::setParameters(std::istream &input) {
  if (!(input >> customerID)) {
    return false;
  }

  if (!isValidCustomerID(customerID)) {
    return false;
  }

  std::string remainder;
  std::getline(input, remainder);

  return true;
}

std::string HistoryCommand::getDescription() const {
  return "History " + customerID;
}