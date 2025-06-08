/**
 * @location src/inventory_command.cpp
 */

#include "commands.h"
#include "factory.h"
#include "store.h"
#include <iostream>

// Self-registration with factory
namespace {
class InventoryRegistrar {
public:
  InventoryRegistrar() {
    CommandFactory::getInstance().registerCommandType(
        'I', []() { return std::make_unique<InventoryCommand>(); });
  }
};
InventoryRegistrar inventoryRegistrar;
} // namespace

bool InventoryCommand::execute(Store &store) {
  std::cout << "==========================\n";
  store.displayInventory(std::cout);
  return true;
}

char InventoryCommand::getCommandType() const { return 'I'; }

Command *InventoryCommand::clone() const { return new InventoryCommand(*this); }

bool InventoryCommand::setParameters(std::istream &input) {
  // Inventory command has no parameters
  std::string remainder;
  std::getline(input, remainder);
  return true;
}

std::string InventoryCommand::getDescription() const { return "Inventory"; }