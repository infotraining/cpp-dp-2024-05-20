#ifndef EMPLOYEE_INTERFACE_HPP
#define EMPLOYEE_INTERFACE_HPP

#include <iostream>
#include <string>
#include <memory>

#include "hrinfo_interface.hpp"

class HRInfo;

class Employee
{
private:
    std::string name_;    

protected:
    std::string name() const
    {
        return name_;
    }

public:
    Employee(const std::string& name);
    virtual ~Employee() = default;

    virtual void description() const = 0;
    virtual std::unique_ptr<HRInfo> get_info() const = 0;
};

#endif // include guard