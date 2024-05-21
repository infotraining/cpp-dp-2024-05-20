#ifndef COFFEEHELL_HPP_
#define COFFEEHELL_HPP_

#include <iostream>
#include <string>

class Coffee
{
public:
    virtual ~Coffee() = default;

    virtual float get_total_price() const = 0;
    virtual std::string get_description() const = 0;
    virtual void prepare() = 0;
};

class CoffeeBase : public Coffee
{
    float price_;
    std::string description_;

public:
    CoffeeBase(float price, const std::string& description)
        : price_{price}
        , description_{description}
    {
    }

    float get_total_price() const override
    {
        return price_;
    }

    std::string get_description() const override
    {
        return description_;
    }
};

class DecoratedCoffee : public Coffee
{
    std::unique_ptr<Coffee> m_coffee;

public:
    explicit DecoratedCoffee(std::unique_ptr<Coffee> coffeeSp)
        : m_coffee(std::move(coffeeSp))
    {
    }

protected:
    Coffee& coffee() { return *m_coffee; }
    const Coffee& coffee() const { return *m_coffee; }
};

class Whipped : public DecoratedCoffee
{
public:
    Whipped(std::unique_ptr<Coffee> coffeeSp)
        : DecoratedCoffee(std::move(coffeeSp))
    {
    }

    float get_total_price() const override
    {
        return coffee().get_total_price() + 2.5;
    }

    std::string get_description() const override
    {
        return std::string{"Whipped "} + coffee().get_description();
    };

    void prepare() override
    {
        coffee().prepare();
        std::cout << "Adding whipped cream...\n";
    }
};

class Whisky : public DecoratedCoffee
{
public:
    Whisky(std::unique_ptr<Coffee> coffeeSp)
        : DecoratedCoffee(std::move(coffeeSp))
    {
    }

    float get_total_price() const override
    {
        return coffee().get_total_price() + 6.0;
    }
    std::string get_description() const override
    {
        return std::string{"Whisky "} + coffee().get_description();
    };

    void prepare() override
    {
        coffee().prepare();
        std::cout << "Pouring good whisky\n";
    }
};

class Espresso : public CoffeeBase
{
public:
    Espresso(float price = 4.0, const std::string& description = "Espresso")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect espresso: 7 g, 15 bar and 24 sec.\n";
    }
};

class ExtraEspresso : public DecoratedCoffee
{
    Espresso espresso_;

public:
    ExtraEspresso(std::unique_ptr<Coffee> coffeeSp)
        : DecoratedCoffee(std::move(coffeeSp))
    {
    }

    float get_total_price() const override
    {
        return coffee().get_total_price() + espresso_.get_total_price();
    }

    std::string get_description() const override
    {
        return std::string{"ExtraEspressso "} + coffee().get_description();
    };

    void prepare() override
    {
        coffee().prepare();
        espresso_.prepare();
    }
};

class Cappuccino : public CoffeeBase
{
public:
    Cappuccino(float price = 6.0, const std::string& description = "Cappuccino")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect cappuccino.\n";
    }
};

class Latte : public CoffeeBase
{
public:
    Latte(float price = 8.0, const std::string& description = "Latte")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect latte.\n";
    }
};

// TO DO: Condiments: Whipped: 2.5$, Whisky: 6.0$, ExtraEspresso: 4.0$

// TO DO: Add CoffeeDecorator and concrete decorators for condiments

#endif /*COFFEEHELL_HPP_*/
