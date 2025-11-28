#pragma once
#include <map>
#include <string>
#include "Strategy.h"
#include <iostream>

class StrategyFactory {
private:
    std::map<std::string, Strategy* (*)()> creators;

public:
    StrategyFactory();
    template <class T>
    void add(const std::string& name) {
        creators[name] = []() -> Strategy* { return new T(); };
    }

    std::vector<std::string> getAvailableStrategies() const;
    
    Strategy* create(const std::string& name) const;
    
    void printAvailableStrategies() const;
};