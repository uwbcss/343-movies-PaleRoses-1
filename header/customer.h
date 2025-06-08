/**
 * @file customer.h
 * @location header/customer.h
 * @brief Customer and Transaction classes
 */

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "movie.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Single transaction record
class Transaction {
public:
  enum Type { BORROW, RETURN };

  Transaction(Type type, const Movie *movie)
      : transactionType(type), moviePtr(movie) {}

  void display(std::ostream &out) const {
    out << (transactionType == BORROW ? "Borrow " : "Return ");
    out << moviePtr->getTitle();
  }

  Type getType() const { return transactionType; }
  const Movie *getMovie() const { return moviePtr; }

private:
  Type transactionType;
  const Movie *moviePtr; // Non-owning pointer
};

// Customer in the movie store system
class Customer {
public:
  Customer(const std::string &id, const std::string &lastName,
           const std::string &firstName)
      : customerID(id), lastName(lastName), firstName(firstName) {}

  const std::string &getID() const { return customerID; }
  std::string getFullName() const { return firstName + " " + lastName; }
  std::string getDisplayName() const { return lastName + " " + firstName; }

  void addTransaction(Transaction::Type type, const Movie *movie) {
    transactions.emplace_back(type, movie);
  }

  void displayHistory(std::ostream &out) const {
    out << "History for " << customerID << " " << getDisplayName() << ":\n";

    if (transactions.empty()) {
      out << "No history for " << getDisplayName() << "\n";
      return;
    }

    for (const auto &transaction : transactions) {
      transaction.display(out);
      out << " " << getDisplayName() << " ";
      transaction.getMovie()->display(out);
      out << "\n";
    }
  }

  // Check if customer currently has this movie borrowed
  bool hasMovieBorrowed(const Movie *movie) const {
    int borrowCount = 0;
    int returnCount = 0;

    for (const auto &transaction : transactions) {
      if (transaction.getMovie() == movie) {
        if (transaction.getType() == Transaction::BORROW) {
          borrowCount++;
        } else {
          returnCount++;
        }
      }
    }

    return borrowCount > returnCount;
  }

  // Parse from input: ID LastName FirstName
  static std::unique_ptr<Customer> parseFromStream(std::istream &input) {
    std::string id;
    std::string lastName;
    std::string firstName;

    if (!(input >> id >> lastName >> firstName)) {
      return nullptr;
    }

    // Validate 4-digit ID
    if (id.length() != 4) {
      return nullptr;
    }
    for (char c : id) {
      if (std::isdigit(c) == 0) {
        return nullptr;
      }
    }

    return std::make_unique<Customer>(id, lastName, firstName);
  }

private:
  std::string customerID;
  std::string lastName;
  std::string firstName;
  std::vector<Transaction> transactions; // Chronological order
};

#endif // CUSTOMER_H