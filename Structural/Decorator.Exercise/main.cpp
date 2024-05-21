#include "starbugs_coffee.hpp"

#include <cassert>
#include <memory>

void client(std::unique_ptr<Coffee> coffee)
{
    std::cout << "Description: " << coffee->get_description() << "; Price: " << coffee->get_total_price() << std::endl;
    coffee->prepare();
}

template <typename HeadDecorator, typename... TailDecorators>
struct DecorateWith
{
    static std::unique_ptr<Coffee> make_decorator(std::unique_ptr<Coffee> component)
    {
        auto decorator_component = std::make_unique<HeadDecorator>(std::move(component));

        return DecorateWith<TailDecorators...>::make_decorator(std::move(decorator_component));
    }
};

template <typename HeadDecorator>
struct DecorateWith<HeadDecorator>
{
    static std::unique_ptr<Coffee> make_decorator(std::unique_ptr<Coffee> component)
    {
        return std::make_unique<HeadDecorator>(std::move(component));
    }
};

class CoffeeBuilder
{
    std::unique_ptr<Coffee> coffee_;

public:
    template <typename BaseCoffee>
    CoffeeBuilder& create_base()
    {
        coffee_ = std::make_unique<BaseCoffee>();

        return *this;
    }

    template <typename... Condiments>
    CoffeeBuilder& add()
    {
        assert(coffee_);

        coffee_ = DecorateWith<Condiments...>::make_decorator(std::move(coffee_));

        return *this;
    }

    std::unique_ptr<Coffee> get_coffee()
    {
        return std::move(coffee_);
    }
};

int main()
{
    std::unique_ptr<Coffee> cf = std::make_unique<ExtraEspresso>(
        std::make_unique<Whisky>(
            std::make_unique<Whipped>(
                std::make_unique<Latte>())));
    client(std::move(cf));

    CoffeeBuilder cb;
    cb.create_base<Espresso>()
        .add<ExtraEspresso>()
        .add<Whisky, Whisky, Whisky, Whipped>();

    client(cb.get_coffee());
}
