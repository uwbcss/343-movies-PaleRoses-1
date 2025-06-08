/**
 * @file movie.h
 * @location header/movie.h
 * @brief Abstract base class for all movie types in the store
 *
 * This class defines the interface that all concrete movie classes must
 * implement. It provides the polymorphic foundation for handling different
 * movie genres without using conditional statements.
 */

#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>

class Movie {
  public:
    virtual ~Movie() = default;

    // Display movie information for inventory output
    virtual void display(std::ostream &out) const = 0;

    // Get sorting key based on genre-specific criteria
    virtual std::string getSortingKey() const = 0;

    // Compare for BST ordering
    virtual bool operator<(const Movie &other) const = 0;

    // Parse movie data from file input stream
    virtual bool parseData(std::istream &input) = 0;

    // Create search key from command parameters
    virtual std::string createSearchKey(std::istream &input) const = 0;

    // Get this movie's search key
    virtual std::string getSearchKey() const = 0;

    // Return genre code: 'F', 'D', or 'C'
    virtual char getMovieType() const = 0;

    // Virtual constructor pattern
    virtual Movie *clone() const = 0;

    // Concrete methods shared by all movie types
    bool borrowMovie() {
        if (stock > 0) {
            stock--;
            return true;
        }
        return false;
    }

    void returnMovie() { stock++; }
    int getStock() const { return stock; }
    const std::string &getTitle() const { return title; }
    const std::string &getDirector() const { return director; }

  protected:
    // Protected constructor - only derived classes can be instantiated
    Movie() : stock(0), year(0) {}

    // Data members common to all movie types
    int stock;            // Number of copies available
    std::string director; // Director name
    std::string title;    // Movie title
    int year;             // Release year

    // Helper function for parsing comma-delimited fields
    std::string parseField(std::istream &input, char delimiter = ',') {
        std::string field;
        std::getline(input, field, delimiter);
        // Trim leading and trailing whitespace
        size_t start = field.find_first_not_of(" \t");
        size_t end = field.find_last_not_of(" \t");
        if (start != std::string::npos && end != std::string::npos) {
            return field.substr(start, end - start + 1);
        }
        return "";
    }
};

#endif // MOVIE_H