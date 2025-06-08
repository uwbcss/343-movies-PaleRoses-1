/**
 * @file classic.cpp
 * @location src/classic.cpp
 * @brief Implementation of Classic movie type
 */

#include "classic.h"
#include "factory.h"
#include <iomanip>
#include <sstream>

// Self-registration with factory
namespace {
class ClassicRegistrar {
public:
  ClassicRegistrar() {
    MovieFactory::getInstance().registerMovieType(
        'C', []() { return std::make_unique<Classic>(); });
  }
};
// Static instance causes registration at program startup
ClassicRegistrar classicRegistrar;
} // namespace

Classic::Classic() : releaseMonth(0), releaseYear(0) {
  // Initialize classic-specific members
}

// Display format based on sample output: YYYY M, Actor, Director, Title (Stock)
// - Classics
void Classic::display(std::ostream &out) const {
  out << releaseYear << " " << releaseMonth << ", " << actorFirstName << " "
      << actorLastName << ", " << director << ", " << title << " (" << stock
      << ") - Classics";
}

// Sort by release date (YYYYMM), then major actor
std::string Classic::getSortingKey() const {
  return getReleaseDateSortKey() + " " + actorFirstName + " " + actorLastName;
}

std::string Classic::getReleaseDateSortKey() const {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(4) << releaseYear << std::setfill('0')
     << std::setw(2) << releaseMonth;
  return ss.str();
}

bool Classic::operator<(const Movie &other) const {
  // Only compare with same type
  if (other.getMovieType() != 'C') {
    return getMovieType() < other.getMovieType();
  }
  return getSortingKey() < other.getSortingKey();
}

// Parse format: C, Stock, Director, Title, Major actor Release date
// Example: C, 10, George Cukor, Holiday, Katherine Hepburn 9 1938
bool Classic::parseData(std::istream &input) {
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

  // Rest of line contains: FirstName LastName Month Year
  std::string remainder;
  std::getline(input, remainder);

  // Trim leading whitespace
  size_t start = remainder.find_first_not_of(" \t");
  if (start == std::string::npos) {
    return false;
  }
  remainder = remainder.substr(start);

  // Parse actor first name, last name, month, year from remainder
  std::istringstream iss(remainder);
  if (!(iss >> actorFirstName >> actorLastName >> releaseMonth >>
        releaseYear)) {
    return false;
  }

  // Validate month
  if (releaseMonth < 1 || releaseMonth > 12) {
    return false;
  }

  // Store year for base class (some commands may need it)
  year = releaseYear;

  return true;
}

// Command format: month year FirstName LastName
// Example: 9 1938 Katherine Hepburn
std::string Classic::createSearchKey(std::istream &input) const {
  int month;
  int year;
  std::string firstName;
  std::string lastName;

  if (!(input >> month >> year >> firstName >> lastName)) {
    return "";
  }

  std::stringstream ss;
  ss << month << " " << year << " " << firstName << " " << lastName;
  return ss.str();
}

// Search key matches command format: "M YYYY FirstName LastName"
std::string Classic::getSearchKey() const {
  std::stringstream ss;
  ss << releaseMonth << " " << releaseYear << " " << actorFirstName << " "
     << actorLastName;
  return ss.str();
}

char Classic::getMovieType() const { return 'C'; }

Movie *Classic::clone() const { return new Classic(*this); }