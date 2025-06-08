/**
 * @location src/store.cpp
 */

#include "store.h"
#include "bstree.h"
#include "classic.h"
#include "comedy.h"
#include "command.h"
#include "customer.h"
#include "drama.h"
#include "factory.h"
#include "hashtable.h"
#include "movie.h"
#include <fstream>
#include <iostream>
#include <sstream>

Store::Store() {
  // Initialize the hash table for customers
  customers = std::make_unique<HashTable<std::string, Customer *>>();

  // Initialize BSTs for each movie genre
  // Using map allows easy extension for new genres
  genreTrees['F'] = std::make_unique<BSTree<Movie *>>(); // Comedy
  genreTrees['D'] = std::make_unique<BSTree<Movie *>>(); // Drama
  genreTrees['C'] = std::make_unique<BSTree<Movie *>>(); // Classics
}

Store::~Store() {
  // Smart pointers handle cleanup automatically
}

bool Store::initialize(const std::string &movieFile,
                       const std::string &customerFile) {
  // Load customers first (they're referenced by commands)
  int customersLoaded = loadCustomers(customerFile);
  if (customersLoaded == 0) {
    std::cerr << "Warning: No customers loaded from " << customerFile
              << std::endl;
  }

  // Load movies
  int moviesLoaded = loadMovies(movieFile);
  if (moviesLoaded == 0) {
    std::cerr << "Warning: No movies loaded from " << movieFile << std::endl;
    return false;
  }

  return true;
}

bool Store::processCommands(const std::string &commandFile) {
  std::ifstream file(commandFile);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open command file " << commandFile
              << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    processCommandLine(line);
  }

  file.close();
  return true;
}

Customer *Store::findCustomer(const std::string &customerID) {
  Customer **customerPtr = customers->find(customerID);
  return (customerPtr != nullptr) ? *customerPtr : nullptr;
}

Movie *Store::findMovie(char movieType, const std::string &searchKey) {
  BSTree<Movie *> *tree = getGenreTree(movieType);
  if (tree == nullptr) {
    return nullptr;
  }

  // Use predicate search since search key format doesn't follow BST ordering
  Movie **result = tree->findByPredicate([&searchKey](Movie *const &movie) {
    return movie->getSearchKey() == searchKey;
  });

  return (result != nullptr) ? *result : nullptr;
}

void Store::displayInventory(std::ostream &out) const {
  // Display in order: Comedy, Drama, Classics
  const char genreOrder[] = {'F', 'D', 'C'};

  for (char genre : genreOrder) {
    auto it = genreTrees.find(genre);
    if (it != genreTrees.end() && it->second) {
      it->second->inOrderTraversal([&out](Movie *const &movie) {
        movie->display(out);
        out << "\n";
      });
    }
  }
}

bool Store::displayCustomerHistory(const std::string &customerID,
                                   std::ostream &out) {
  Customer *customer = findCustomer(customerID);
  if (customer == nullptr) {
    return false;
  }

  customer->displayHistory(out);
  return true;
}

bool Store::addMovie(std::unique_ptr<Movie> movie) {
  if (!movie) {
    return false;
  }

  char movieType = movie->getMovieType();
  BSTree<Movie *> *tree = getGenreTree(movieType);
  if (tree == nullptr) {
    std::cerr << "No tree available for movie type " << movieType << std::endl;
    return false;
  }

  // Get raw pointer for tree storage
  Movie *moviePtr = movie.get();

  // Define comparison function for this movie type
  auto compareFunc = [](Movie *const &a, Movie *const &b) {
    return a->getSortingKey() < b->getSortingKey();
  };

  // Insert into appropriate tree
  tree->insert(moviePtr, compareFunc);

  // Transfer ownership to inventory vector
  movieInventory.push_back(std::move(movie));

  return true;
}

bool Store::addCustomer(std::unique_ptr<Customer> customer) {
  if (!customer) {
    return false;
  }

  const std::string &id = customer->getID();
  Customer *customerPtr = customer.get();

  // Insert into hash table
  if (!customers->insert(id, customerPtr)) {
    std::cerr << "Customer with ID " << id << " already exists" << std::endl;
    return false;
  }

  // Transfer ownership to customer list
  customerList.push_back(std::move(customer));

  return true;
}

int Store::loadMovies(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open movie file " << filename << std::endl;
    return 0;
  }

  std::string line;
  int count = 0;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    if (processMovieLine(line)) {
      count++;
    }
  }

  file.close();
  return count;
}

int Store::loadCustomers(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open customer file " << filename
              << std::endl;
    return 0;
  }

  std::string line;
  int count = 0;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    if (processCustomerLine(line)) {
      count++;
    }
  }

  file.close();
  return count;
}

BSTree<Movie *> *Store::getGenreTree(char movieType) {
  auto it = genreTrees.find(movieType);
  if (it != genreTrees.end()) {
    return it->second.get();
  }
  return nullptr;
}

const BSTree<Movie *> *Store::getGenreTree(char movieType) const {
  auto it = genreTrees.find(movieType);
  if (it != genreTrees.end()) {
    return it->second.get();
  }
  return nullptr;
}

bool Store::processMovieLine(const std::string &line) {
  std::istringstream iss(line);
  char movieType;

  // Read movie type
  if (!(iss >> movieType)) {
    return false;
  }

  // Skip comma after movie type
  char comma;
  iss >> comma;

  // Create movie using factory
  auto movie = MovieFactory::getInstance().createMovie(movieType);
  if (!movie) {
    std::cerr << "Unknown movie type: " << movieType
              << ", discarding line: " << line.substr(2) << std::endl;
    return false;
  }

  // Parse movie data
  if (!movie->parseData(iss)) {
    std::cerr << "Failed to parse movie data: " << line << std::endl;
    return false;
  }

  // Add to inventory
  return addMovie(std::move(movie));
}

bool Store::processCustomerLine(const std::string &line) {
  std::istringstream iss(line);
  auto customer = Customer::parseFromStream(iss);

  if (!customer) {
    std::cerr << "Failed to parse customer data: " << line << std::endl;
    return false;
  }

  return addCustomer(std::move(customer));
}

bool Store::processCommandLine(const std::string &line) {
  std::istringstream iss(line);
  char commandType;

  // Read command type
  if (!(iss >> commandType)) {
    return false;
  }

  // Create command using factory
  auto command = CommandFactory::getInstance().createCommand(commandType);
  if (!command) {
    std::cerr << "Unknown command type: " << commandType
              << ", discarding line: " << std::endl;
    return false;
  }

  // Set command parameters
  if (!command->setParameters(iss)) {
    // Error already reported by setParameters
    return false;
  }

  // Execute command
  return command->execute(*this);
}