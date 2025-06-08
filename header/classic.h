/**
 * @location header/classic.h
 */

#ifndef CLASSIC_H
#define CLASSIC_H

#include "movie.h"

class Classic : public Movie {
public:
  Classic();
  virtual ~Classic() = default;

  // Display for inventory: Release date, Major actor, Director, Title (Stock)
  // - Classics
  void display(std::ostream &out) const override;

  // Sort key: "YYYYMM ActorFirstName ActorLastName"
  std::string getSortingKey() const override;

  // Compare by release date, then major actor
  bool operator<(const Movie &other) const override;

  // Parse: C, Stock, Director, Title, Major actor Release date
  bool parseData(std::istream &input) override;

  // Parse command: month year FirstName LastName
  std::string createSearchKey(std::istream &input) const override;

  // Get search key: "MM YYYY FirstName LastName"
  std::string getSearchKey() const override;

  // Return 'C'
  char getMovieType() const override;

  // Virtual constructor
  Movie *clone() const override;

private:
  std::string actorFirstName;
  std::string actorLastName;
  int releaseMonth;
  int releaseYear;

  // Convert month/year to YYYYMM format for sorting
  std::string getReleaseDateSortKey() const;
};

#endif // CLASSIC_H