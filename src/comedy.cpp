/**
 * @location src/comedy.cpp
 */

#include "comedy.h"
#include "factory.h"
#include <iomanip>
#include <sstream>

// Self-registration with factory
namespace {
class ComedyRegistrar {
  public:
    ComedyRegistrar() {
        MovieFactory::getInstance().registerMovieType(
            'F', []() { return std::make_unique<Comedy>(); });
    }
};
// Static instance causes registration at program startup
ComedyRegistrar comedyRegistrar;
} // namespace

Comedy::Comedy() {
    // Base class constructor handles initialization
}

// Display format: Title, Year, Director (Stock) - Comedy
void Comedy::display(std::ostream &out) const {
    out << title << ", " << year << ", " << director << " (" << stock
        << ") - Comedy";
}

// Sort by Title, then Year
std::string Comedy::getSortingKey() const {
    std::stringstream ss;
    ss << title << " " << std::setfill('0') << std::setw(4) << year;
    return ss.str();
}

bool Comedy::operator<(const Movie &other) const {
    // Only compare with same type
    if (other.getMovieType() != 'F') {
        return getMovieType() < other.getMovieType();
    }
    return getSortingKey() < other.getSortingKey();
}

// Parse format: F, Stock, Director, Title, Year
bool Comedy::parseData(std::istream &input) {
    std::string stockStr;

    // Stock
    std::getline(input, stockStr, ',');
    try {
        stock = std::stoi(stockStr);
    } catch (...) {
        return false;
    }

    // Director
    director = parseField(input, ',');
    if (director.empty()) {
        return false;
    }

    // Title
    title = parseField(input, ',');
    if (title.empty()) {
        return false;
    }

    // Year
    std::string yearStr;
    std::getline(input, yearStr);
    try {
        year = std::stoi(yearStr);
    } catch (...) {
        return false;
    }

    return true;
}

// Command format: Title, Year
std::string Comedy::createSearchKey(std::istream &input) const {
    std::string searchTitle;
    std::string yearStr;

    // Read title until comma
    std::getline(input, searchTitle, ',');

    // Trim whitespace from title
    size_t start = searchTitle.find_first_not_of(" \t");
    size_t end = searchTitle.find_last_not_of(" \t");
    if (start != std::string::npos && end != std::string::npos) {
        searchTitle = searchTitle.substr(start, end - start + 1);
    }

    // Read year
    input >> yearStr;

    return searchTitle + "," + yearStr;
}

// Search key format: Title,Year
std::string Comedy::getSearchKey() const {
    return title + "," + std::to_string(year);
}

char Comedy::getMovieType() const { return 'F'; }

Movie *Comedy::clone() const { return new Comedy(*this); }