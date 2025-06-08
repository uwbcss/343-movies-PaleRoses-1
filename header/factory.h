/**
 * @location header/factory.h
 */

#ifndef FACTORY_H
#define FACTORY_H

#include "command.h"
#include "movie.h"
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

// Factory for creating Movie objects based on genre code
class MovieFactory {
  public:
    using MovieCreator = std::function<std::unique_ptr<Movie>()>;

    // Singleton instance
    static MovieFactory &getInstance() {
        static MovieFactory instance;
        return instance;
    }

    // Register a movie type with its creation function
    bool registerMovieType(char movieType, MovieCreator creator) {
        if (creators.find(movieType) != creators.end()) {
            std::cerr << "Movie type " << movieType << " already registered\n";
            return false;
        }
        creators[movieType] = creator;
        return true;
    }

    // Create movie by type code
    std::unique_ptr<Movie> createMovie(char movieType) const {
        auto it = creators.find(movieType);
        if (it != creators.end()) {
            return it->second();
        }
        return nullptr;
    }

    bool isValidMovieType(char movieType) const {
        return creators.find(movieType) != creators.end();
    }

  private:
    MovieFactory() = default;
    MovieFactory(const MovieFactory &) = delete;
    MovieFactory &operator=(const MovieFactory &) = delete;

    std::unordered_map<char, MovieCreator> creators;
};

// Factory for creating Command objects based on command code
class CommandFactory {
  public:
    using CommandCreator = std::function<std::unique_ptr<Command>()>;

    // Singleton instance
    static CommandFactory &getInstance() {
        static CommandFactory instance;
        return instance;
    }

    // Register a command type with its creation function
    bool registerCommandType(char commandType, CommandCreator creator) {
        if (creators.find(commandType) != creators.end()) {
            std::cerr << "Command type " << commandType
                      << " already registered\n";
            return false;
        }
        creators[commandType] = creator;
        return true;
    }

    // Create command by type code
    std::unique_ptr<Command> createCommand(char commandType) const {
        auto it = creators.find(commandType);
        if (it != creators.end()) {
            return it->second();
        }
        return nullptr;
    }

    bool isValidCommandType(char commandType) const {
        return creators.find(commandType) != creators.end();
    }

  private:
    CommandFactory() = default;
    CommandFactory(const CommandFactory &) = delete;
    CommandFactory &operator=(const CommandFactory &) = delete;

    std::unordered_map<char, CommandCreator> creators;
};

// Template for automatic factory registration at startup
template <typename ConcreteType, typename FactoryType> class FactoryRegistrar {
  public:
    explicit FactoryRegistrar(char typeCode) {
        FactoryType::getInstance().registerType(
            typeCode, []() { return std::make_unique<ConcreteType>(); });
    }
};

#endif // FACTORY_H