#pragma once
#include <string>

class Burger {
private:
    bool cheese, bacon, sauce, doublePatty, onion;

public:
    Burger(bool cheese, bool bacon, bool sauce, bool doublePatty, bool onion);
    double getPrice() const;
    std::string getDescription() const;
};