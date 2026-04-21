#include "burger.h"

Burger::Burger(bool cheese, bool bacon, bool sauce, bool doublePatty, bool onion)
    : cheese(cheese), bacon(bacon), sauce(sauce),
    doublePatty(doublePatty), onion(onion) {
}

double Burger::getPrice() const {
    double price = 5.0;
    if (cheese) price += 1.0;
    if (bacon) price += 1.5;
    if (sauce) price += 0.5;
    if (doublePatty) price += 2.0;
    if (onion) price += 0.3;
    return price;
}

std::string Burger::getDescription() const {
    std::string desc = "Burger";
    if (cheese) desc += " + cheese";
    if (bacon) desc += " + bacon";
    if (sauce) desc += " + sauce";
    if (doublePatty) desc += " + double patty";
    if (onion) desc += " + onion";
    return desc;
}