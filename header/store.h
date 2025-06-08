/**
 * @file store.h
 * @location header/store.h
 * @brief Main store class that coordinates all operations
 */

#ifndef STORE_H
#define STORE_H

#include "command.h"
#include "customer.h"
#include "movie.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declarations
template <typename T> class BSTree;
template <typename K, typename V> class HashTable;

class Store {
public:
  Store();
  ~Store();

  // Initialize from data files
  bool initialize(const std::string &movieFile,
                  const std::string &customerFile);

  // Process commands from file
  bool processCommands(const std::string &commandFile);

  // Find customer by 4-digit ID
  Customer *findCustomer(const std::string &customerID);

  // Find movie by genre and search key
  Movie *findMovie(char movieType, const std::string &searchKey);

  // Display entire inventory sorted by genre
  void displayInventory(std::ostream &out) const;

  // Display customer transaction history
  bool displayCustomerHistory(const std::string &customerID, std::ostream &out);

  // Add movie to inventory (takes ownership)
  bool addMovie(std::unique_ptr<Movie> movie);

  // Add customer to database (takes ownership)
  bool addCustomer(std::unique_ptr<Customer> customer);

private:
  // Map of genre code to BST for that genre's movies
  std::unordered_map<char, std::unique_ptr<BSTree<Movie *>>> genreTrees;

  // Hash table for O(1) customer lookup by ID
  std::unique_ptr<HashTable<std::string, Customer *>> customers;

  // Ownership of all movies
  std::vector<std::unique_ptr<Movie>> movieInventory;

  // Ownership of all customers
  std::vector<std::unique_ptr<Customer>> customerList;

  // Load data from files
  int loadMovies(const std::string &filename);
  int loadCustomers(const std::string &filename);

  // Get BST for a specific genre
  BSTree<Movie *> *getGenreTree(char movieType);
  const BSTree<Movie *> *getGenreTree(char movieType) const;

  // Process single lines from input files
  bool processMovieLine(const std::string &line);
  bool processCustomerLine(const std::string &line);
  bool processCommandLine(const std::string &line);
};

#endif // STORE_H