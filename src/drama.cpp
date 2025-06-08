/**
 * @location src/drama.cpp
 */

#include "drama.h"
#include "factory.h"
#include <sstream>

// Self-registration with factory
namespace {
class DramaRegistrar {
public:
  DramaRegistrar() {
    MovieFactory::getInstance().registerMovieType(
        'D', []() { return std::make_unique<Drama>(); });
  }
};
// Static instance causes registration at program startup
DramaRegistrar dramaRegistrar;
} // namespace

Drama::Drama() {
  // Base class constructor handles initialization
}

// Display format: Director, Title, Year (Stock) - Drama
void Drama::display(std::ostream &out) const {
  out << director << ", " << title << ", " << year << " (" << stock
      << ") - Drama";
}

// Sort by Director, then Title
std::string Drama::getSortingKey() const { return director + " " + title; }

bool Drama::operator<(const Movie &other) const {
  // Only compare with same type
  if (other.getMovieType() != 'D') {
    return getMovieType() < other.getMovieType();
  }
  return getSortingKey() < other.getSortingKey();
}

// Parse format: D, Stock, Director, Title, Year
bool Drama::parseData(std::istream &input) {
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

// Command format: Director, Title
std::string Drama::createSearchKey(std::istream &input) const {
  std::string searchDirector;
  std::string searchTitle;

  // Read director until comma
  std::getline(input, searchDirector, ',');

  // Trim whitespace
  size_t start = searchDirector.find_first_not_of(" \t");
  size_t end = searchDirector.find_last_not_of(" \t");
  if (start != std::string::npos && end != std::string::npos) {
    searchDirector = searchDirector.substr(start, end - start + 1);
  }

  // Read rest of line as title (may have trailing comma)
  std::getline(input, searchTitle);

  // Remove trailing comma if present
  if (!searchTitle.empty() && searchTitle.back() == ',') {
    searchTitle.pop_back();
  }

  // Trim whitespace from title
  start = searchTitle.find_first_not_of(" \t");
  end = searchTitle.find_last_not_of(" \t");
  if (start != std::string::npos && end != std::string::npos) {
    searchTitle = searchTitle.substr(start, end - start + 1);
  }

  return searchDirector + "," + searchTitle;
}

// Search key format: Director,Title
std::string Drama::getSearchKey() const { return director + "," + title; }

char Drama::getMovieType() const { return 'D'; }

Movie *Drama::clone() const { return new Drama(*this); }