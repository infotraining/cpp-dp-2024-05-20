#ifndef EMPLOYEE_HPP_
#define EMPLOYEE_HPP_

#include <iostream>
#include <string>
#include <memory>

#include "hrinfo.hpp"
#include "employee_interface.hpp"

class Salary : public Employee
{
public:
    Salary(const std::string& name);

    void description() const override;
    std::unique_ptr<HRInfo> get_info() const override;
};

class Hourly : public Employee
{
public:
    Hourly(const std::string& name);

    void description() const override;
    std::unique_ptr<HRInfo> get_info() const override;
};

class Temp : public Employee
{
public:
    Temp(const std::string& name);

    void description() const override;
    std::unique_ptr<HRInfo> get_info() const override;
};

#endif /* EMPLOYEE_HPP_ */
