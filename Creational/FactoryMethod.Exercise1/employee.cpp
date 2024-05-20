#include "employee.hpp"

Employee::Employee(const std::string& name)
    : name_{name}
{
}

Salary::Salary(const std::string& name)
    : Employee{name}
{
}

void Salary::description() const
{
    std::cout << "Salaried Employee: " << name() << std::endl;
}

std::unique_ptr<HRInfo> Salary::get_info() const
{
    return std::make_unique<StdInfo>(this);
}

Hourly::Hourly(const std::string& name)
    : Employee{name}
{
}

std::unique_ptr<HRInfo> Hourly::get_info() const
{
    return std::make_unique<StdInfo>(this);
}

void Hourly::description() const
{
    std::cout << "Hourly Employee: " << name() << std::endl;
}

Temp::Temp(const std::string& name)
    : Employee(name)
{
}

std::unique_ptr<HRInfo> Temp::get_info() const
{
    return std::make_unique<TempInfo>(this);
}

void Temp::description() const
{
    std::cout << "Temporary Employee: " << name() << std::endl;
}
