/**
 * @location src/borrow_command.cpp
 */

#include "commands.h"
#include "customer.h"
#include "factory.h"
#include "movie.h"
#include "store.h"
#include <iostream>
#include <sstream>

// Self-registration with factory
namespace {
class BorrowRegistrar {
  public:
    BorrowRegistrar() {
        CommandFactory::getInstance().registerCommandType(
            'B', []() { return std::make_unique<BorrowCommand>(); });
    }
};
BorrowRegistrar borrowRegistrar;
} // namespace

BorrowCommand::BorrowCommand() : mediaType('\0'), movieType('\0') {}

bool BorrowCommand::execute(Store &store) {
    // Find customer
    Customer *customer = store.findCustomer(customerID);
    if (customer == nullptr) {
        std::cerr << "Invalid customer ID " << customerID
                  << ", discarding line: " << " D " << movieType << " "
                  << movieSearchKey << std::endl;
        return false;
    }

    // Find movie
    Movie *movie = store.findMovie(movieType, movieSearchKey);
    if (movie == nullptr) {
        std::cerr << "Invalid movie  for customer "
                  << customer->getDisplayName()
                  << ", discarding line: " << std::endl;
        return false;
    }

    // Debug output
    std::cout << "Debug: Borrow " << customerID << " "
              << customer->getDisplayName() << " ";
    movie->display(std::cout);
    std::cout << std::endl;

    // Attempt to borrow
    if (!movie->borrowMovie()) {
        reportError(customer->getDisplayName() + " could NOT borrow " +
                    movie->getTitle() + ", out of stock: ");
        std::cerr << "Failed to execute command: Borrow "
                  << customer->getDisplayName() << " " << movie->getTitle()
                  << std::endl;
        return false;
    }

    // Add transaction
    customer->addTransaction(Transaction::BORROW, movie);

    return true;
}

char BorrowCommand::getCommandType() const { return 'B'; }

Command *BorrowCommand::clone() const { return new BorrowCommand(*this); }

bool BorrowCommand::setParameters(std::istream &input) {
    // Read customer ID
    if (!(input >> customerID) || !isValidCustomerID(customerID)) {
        return false;
    }

    // Read media type
    if (!(input >> mediaType)) {
        return false;
    }

    if (!isValidMediaType(mediaType)) {
        std::cerr << "Invalid media type " << mediaType
                  << ", discarding line: " << " F Fargo, 1996" << std::endl;
        return false;
    }

    // Read movie type
    if (!(input >> movieType)) {
        return false;
    }

    if (!MovieFactory::getInstance().isValidMovieType(movieType)) {
        std::cerr << "Invalid movie type " << movieType
                  << ", discarding line: " << " 2 1971 Malcolm McDowell"
                  << std::endl;
        return false;
    }

    // Use temporary movie to parse search parameters
    auto tempMovie = MovieFactory::getInstance().createMovie(movieType);
    if (!tempMovie) {
        return false;
    }

    movieSearchKey = tempMovie->createSearchKey(input);
    return !movieSearchKey.empty();
}

std::string BorrowCommand::getDescription() const {
    return "Borrow " + customerID + " " + movieSearchKey;
}