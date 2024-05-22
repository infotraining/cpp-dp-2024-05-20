#include <cassert>
#include <iostream>
#include <memory>
#include <typeinfo>

class Engine
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~Engine() = default;

    std::unique_ptr<Engine> clone() const // template method
    {
        auto cloned_engine = do_clone();
        assert(typeid(*this) == typeid(*cloned_engine));
        return cloned_engine;
    }
protected:
    virtual std::unique_ptr<Engine> do_clone() const = 0; // must be implemented
};

template <typename TEngine, typename TEngineBase = Engine>
class CloneableEngine : public TEngineBase
{
protected:
    std::unique_ptr<Engine> do_clone() const override
    {
        return std::make_unique<TEngine>(static_cast<const TEngine&>(*this)); // cc
    }
};

class Diesel : public CloneableEngine<Diesel>  // CRTP
{
public:
    void start() override
    {
        std::cout << "Diesel starts\n";
    }

    void stop() override
    {
        std::cout << "Diesel stops\n";
    }
};

class TDI : public Diesel
{
public:
    virtual void start() override
    {
        std::cout << "TDI starts\n";
    }

    virtual void stop() override
    {
        std::cout << "TDI stops\n";
    }
};

class Hybrid : public CloneableEngine<Hybrid>
{
public:
    virtual void start() override
    {
        std::cout << "Hybrid starts\n";
    }

    virtual void stop() override
    {
        std::cout << "Hybrid stops\n";
    }
};

class Car
{
    std::unique_ptr<Engine> engine_;

public:
    Car(std::unique_ptr<Engine> engine)
        : engine_{std::move(engine)}
    {
    }

    Car(const Car& source) : engine_{source.engine_->clone()}
    {
    }

    Car& operator=(const Car& source)
    {
        if (this != &source)
        {
            engine_ = source.engine_->clone();
        }

        return *this;
    }

    void drive(int km)
    {
        engine_->start();
        std::cout << "Driving " << km << " kms\n";
        engine_->stop();
    }
};

int main()
{
    Car c1{std::make_unique<TDI>()};
    c1.drive(100);

    std::cout << "\n";

    Car c2 = c1;
    c2.drive(50);
}
