/**
 * @file drama.h
 * @location header/drama.h
 * @brief Drama movie class - sorted by Director, then Title
 */

#ifndef DRAMA_H
#define DRAMA_H

#include "movie.h"

class Drama : public Movie {
  public:
    Drama();
    virtual ~Drama() = default;

    // Display for inventory: Director, Title, Year (Stock) - Drama
    void display(std::ostream &out) const override;

    // Sort key: "Director Title"
    std::string getSortingKey() const override;

    // Compare by director, then title
    bool operator<(const Movie &other) const override;

    // Parse: D, Stock, Director, Title, Year
    bool parseData(std::istream &input) override;

    // Parse command: Director, Title
    std::string createSearchKey(std::istream &input) const override;

    // Get search key: "Director,Title"
    std::string getSearchKey() const override;

    // Return 'D'
    char getMovieType() const override;

    // Virtual constructor
    Movie *clone() const override;
};

#endif // DRAMA_H