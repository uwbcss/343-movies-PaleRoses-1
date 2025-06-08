/**
 * @file commands.cpp
 * @location src/commands.cpp
 * @brief Implementation of all concrete command classes
 */

 #include "commands.h"
 #include "factory.h"
 #include "store.h"
 #include "movie.h"
 #include "customer.h"
 #include <iostream>
 #include <sstream>
 #include <memory>
 
 // =============================================================================
 // Self-Registration Section
 // Each command registers itself with the factory at program startup
 // =============================================================================
 
 namespace {
     // Inventory command registration
     class InventoryRegistrar {
     public:
         InventoryRegistrar() {
             CommandFactory::getInstance().registerCommandType('I',
                 []() { return std::make_unique<InventoryCommand>(); });
         }
     };
     InventoryRegistrar inventoryRegistrar;
 
     // History command registration
     class HistoryRegistrar {
     public:
         HistoryRegistrar() {
             CommandFactory::getInstance().registerCommandType('H',
                 []() { return std::make_unique<HistoryCommand>(); });
         }
     };
     HistoryRegistrar historyRegistrar;
 
     // Borrow command registration
     class BorrowRegistrar {
     public:
         BorrowRegistrar() {
             CommandFactory::getInstance().registerCommandType('B',
                 []() { return std::make_unique<BorrowCommand>(); });
         }
     };
     BorrowRegistrar borrowRegistrar;
 
     // Return command registration
     class ReturnRegistrar {
     public:
         ReturnRegistrar() {
             CommandFactory::getInstance().registerCommandType('R',
                 []() { return std::make_unique<ReturnCommand>(); });
         }
     };
     ReturnRegistrar returnRegistrar;
 }
 
 // =============================================================================
 // InventoryCommand Implementation
 // =============================================================================
 
 bool InventoryCommand::execute(Store& store) {
     std::cout << "==========================\n";
     store.displayInventory(std::cout);
     return true;
 }
 
 char InventoryCommand::getCommandType() const {
     return 'I';
 }
 
 Command* InventoryCommand::clone() const {
     return new InventoryCommand(*this);
 }
 
 bool InventoryCommand::setParameters(std::istream& input) {
     // Inventory command has no parameters
     std::string remainder;
     std::getline(input, remainder);
     return true;
 }
 
 std::string InventoryCommand::getDescription() const {
     return "Inventory";
 }
 
 // =============================================================================
 // HistoryCommand Implementation
 // =============================================================================
 
 HistoryCommand::HistoryCommand() : customerID("") {
 }
 
 bool HistoryCommand::execute(Store& store) {
     // Debug output as shown in sample
     std::cout << "Debug: History for " << customerID;
     
     Customer* customer = store.findCustomer(customerID);
     if (customer == nullptr) {
         reportError("Customer " + customerID + " not found");
         return false;
     }
     
     std::cout << " " << customer->getDisplayName() << "\n";
     std::cout << "==========================\n";
     
     customer->displayHistory(std::cout);
     
     return true;
 }
 
 char HistoryCommand::getCommandType() const {
     return 'H';
 }
 
 Command* HistoryCommand::clone() const {
     return new HistoryCommand(*this);
 }
 
 bool HistoryCommand::setParameters(std::istream& input) {
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
 
 // =============================================================================
 // BorrowCommand Implementation
 // =============================================================================
 
 BorrowCommand::BorrowCommand() 
     : customerID(""), mediaType('\0'), movieType('\0'), movieSearchKey(""), movieTitle("") {
 }
 
 bool BorrowCommand::execute(Store& store) {
     // Find customer
     Customer* customer = store.findCustomer(customerID);
     if (customer == nullptr) {
         std::cerr << "Invalid customer ID " << customerID 
                   << ", discarding line: " << " D " << movieType 
                   << " " << movieSearchKey << std::endl;
         return false;
     }
     
     // Find movie
     Movie* movie = store.findMovie(movieType, movieSearchKey);
     if (movie == nullptr) {
         std::cerr << "Invalid movie  for customer " << customer->getDisplayName()
                   << ", discarding line: " << std::endl;
         return false;
     }
     
     // Debug output
     std::cout << "Debug: Borrow " << customerID << " " << customer->getDisplayName() 
               << " ";
     movie->display(std::cout);
     std::cout << std::endl;
     
     // Attempt to borrow
     if (!movie->borrowMovie()) {
         reportError(customer->getDisplayName() + " could NOT borrow " 
                     + movie->getTitle() + ", out of stock: ");
         std::cerr << "Failed to execute command: Borrow " 
                   << customer->getDisplayName() << " " << movie->getTitle() << std::endl;
         return false;
     }
     
     // Add transaction
     customer->addTransaction(Transaction::BORROW, movie);
     
     return true;
 }
 
 char BorrowCommand::getCommandType() const {
     return 'B';
 }
 
 Command* BorrowCommand::clone() const {
     return new BorrowCommand(*this);
 }
 
 bool BorrowCommand::setParameters(std::istream& input) {
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
                   << ", discarding line: " << " 2 1971 Malcolm McDowell" << std::endl;
         return false;
     }
     
     // Use temporary movie to parse search parameters
     auto tempMovie = MovieFactory::getInstance().createMovie(movieType);
     if (!tempMovie) {
         return false;
     }
     
     movieSearchKey = tempMovie->createSearchKey(input);
     if (movieSearchKey.empty()) {
         return false;
     }
     
     return true;
 }
 
 std::string BorrowCommand::getDescription() const {
     return "Borrow " + customerID + " " + movieSearchKey;
 }
 
 // =============================================================================
 // ReturnCommand Implementation
 // =============================================================================
 
 ReturnCommand::ReturnCommand() 
     : customerID(""), mediaType('\0'), movieType('\0'), movieSearchKey(""), movieTitle("") {
 }
 
 bool ReturnCommand::execute(Store& store) {
     // Find customer
     Customer* customer = store.findCustomer(customerID);
     if (customer == nullptr) {
         std::cerr << "Invalid customer ID " << customerID 
                   << ", discarding line: " << " D " << movieType 
                   << " " << movieSearchKey << std::endl;
         return false;
     }
     
     // Find movie
     Movie* movie = store.findMovie(movieType, movieSearchKey);
     if (movie == nullptr) {
         std::cerr << "Invalid movie  for customer " << customer->getDisplayName()
                   << ", discarding line: " << std::endl;
         return false;
     }
     
     // Debug output
     std::cout << "Debug: Return " << customerID << " " << customer->getDisplayName() 
               << " ";
     movie->display(std::cout);
     std::cout << std::endl;
     
     // Check if customer has movie borrowed
     if (!customer->hasMovieBorrowed(movie)) {
         reportError(customer->getDisplayName() + " does not have " 
                     + movie->getTitle() + " checked out");
         std::cerr << "Failed to execute command: Return " 
                   << customer->getDisplayName() << " " << movie->getTitle() << std::endl;
         return false;
     }
     
     // Return the movie
     movie->returnMovie();
     
     // Add transaction
     customer->addTransaction(Transaction::RETURN, movie);
     
     return true;
 }
 
 char ReturnCommand::getCommandType() const {
     return 'R';
 }
 
 Command* ReturnCommand::clone() const {
     return new ReturnCommand(*this);
 }
 
 bool ReturnCommand::setParameters(std::istream& input) {
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
                   << ", discarding line: " << " 2 1971 Malcolm McDowell" << std::endl;
         return false;
     }
     
     // Use temporary movie to parse search parameters
     auto tempMovie = MovieFactory::getInstance().createMovie(movieType);
     if (!tempMovie) {
         return false;
     }
     
     movieSearchKey = tempMovie->createSearchKey(input);
     if (movieSearchKey.empty()) {
         return false;
     }
     
     return true;
 }
 
 std::string ReturnCommand::getDescription() const {
     return "Return " + customerID + " " + movieSearchKey;
 }