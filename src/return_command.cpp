/**
 * @location src/return_command.cpp
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
class ReturnRegistrar {
  public:
    ReturnRegistrar() {
        CommandFactory::getInstance().registerCommandType(
            'R', []() { return std::make_unique<ReturnCommand>(); });
    }
};
ReturnRegistrar returnRegistrar;
} // namespace

ReturnCommand::ReturnCommand() : mediaType('\0'), movieType('\0') {}

bool ReturnCommand::execute(Store &store) {
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
    std::cout << "Debug: Return " << customerID << " "
              << customer->getDisplayName() << " ";
    movie->display(std::cout);
    std::cout << std::endl;

    // Check if customer has movie borrowed
    if (!customer->hasMovieBorrowed(movie)) {
        reportError(customer->getDisplayName() + " does not have " +
                    movie->getTitle() + " checked out");
        std::cerr << "Failed to execute command: Return "
                  << customer->getDisplayName() << " " << movie->getTitle()
                  << std::endl;
        return false;
    }

    // Return the movie
    movie->returnMovie();

    // Add transaction
    customer->addTransaction(Transaction::RETURN, movie);

    return true;
}

char ReturnCommand::getCommandType() const { return 'R'; }

Command *ReturnCommand::clone() const { return new ReturnCommand(*this); }

bool ReturnCommand::setParameters(std::istream &input) {
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

std::string ReturnCommand::getDescription() const {
    return "Return " + customerID + " " + movieSearchKey;
}