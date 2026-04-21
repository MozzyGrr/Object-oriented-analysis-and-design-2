#include "burger.h"


double BaseBurger::getPrice() const {
    return 5.0;
}

std::string BaseBurger::getDescription() const {
    return "Burger";
}


BurgerDecorator::BurgerDecorator(IBurger* b) : burger(b) {}

BurgerDecorator::~BurgerDecorator() {
    delete burger;
}

CheeseDecorator::CheeseDecorator(IBurger* b) : BurgerDecorator(b) {}

double CheeseDecorator::getPrice() const {
    return burger->getPrice() + 1.0;
}

std::string CheeseDecorator::getDescription() const {
    return burger->getDescription() + " + cheese";
}


BaconDecorator::BaconDecorator(IBurger* b) : BurgerDecorator(b) {}

double BaconDecorator::getPrice() const {
    return burger->getPrice() + 1.5;
}

std::string BaconDecorator::getDescription() const {
    return burger->getDescription() + " + bacon";
}

SauceDecorator::SauceDecorator(IBurger* b) : BurgerDecorator(b) {}

double SauceDecorator::getPrice() const {
    return burger->getPrice() + 0.5;
}

std::string SauceDecorator::getDescription() const {
    return burger->getDescription() + " + sauce";
}


OnionDecorator::OnionDecorator(IBurger* b) : BurgerDecorator(b) {}

double OnionDecorator::getPrice() const {
    return burger->getPrice() + 0.3;
}

std::string OnionDecorator::getDescription() const {
    return burger->getDescription() + " + onion";
}


DoublePattyDecorator::DoublePattyDecorator(IBurger* b) : BurgerDecorator(b) {}

double DoublePattyDecorator::getPrice() const {
    return burger->getPrice() + 2.0;
}

std::string DoublePattyDecorator::getDescription() const {
    return burger->getDescription() + " + double patty";
}
