/**
 * @location header/comedy.h
 */

#ifndef COMEDY_H
#define COMEDY_H

#include "movie.h"

class Comedy : public Movie {
public:
  Comedy();
  virtual ~Comedy() = default;

  // Display for inventory: Title, Year, Director (Stock) - Comedy
  void display(std::ostream &out) const override;

  // Sort key: "Title YYYY"
  std::string getSortingKey() const override;

  // Compare by title, then year
  bool operator<(const Movie &other) const override;

  // Parse: F, Stock, Director, Title, Year
  bool parseData(std::istream &input) override;

  // Parse command: Title, Year
  std::string createSearchKey(std::istream &input) const override;

  // Get search key: "Title,Year"
  std::string getSearchKey() const override;

  // Return 'F'
  char getMovieType() const override;

  // Virtual constructor
  Movie *clone() const override;
};

#endif // COMEDY_H