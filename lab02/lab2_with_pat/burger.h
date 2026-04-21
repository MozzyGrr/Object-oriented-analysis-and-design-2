#pragma once
#pragma once
#include <string>


class IBurger {
public:
    virtual double getPrice() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~IBurger() {}
};


class BaseBurger : public IBurger {
public:
    double getPrice() const override;
    std::string getDescription() const override;
};


class BurgerDecorator : public IBurger {
protected:
    IBurger* burger;
public:
    BurgerDecorator(IBurger* b);
    virtual ~BurgerDecorator();
};


class CheeseDecorator : public BurgerDecorator {
public:
    CheeseDecorator(IBurger* b);
    double getPrice() const override;
    std::string getDescription() const override;
};

class BaconDecorator : public BurgerDecorator {
public:
    BaconDecorator(IBurger* b);
    double getPrice() const override;
    std::string getDescription() const override;
};

class SauceDecorator : public BurgerDecorator {
public:
    SauceDecorator(IBurger* b);
    double getPrice() const override;
    std::string getDescription() const override;
};

class OnionDecorator : public BurgerDecorator {
public:
    OnionDecorator(IBurger* b);
    double getPrice() const override;
    std::string getDescription() const override;
};

class DoublePattyDecorator : public BurgerDecorator {
public:
    DoublePattyDecorator(IBurger* b);
    double getPrice() const override;
    std::string getDescription() const override;
};