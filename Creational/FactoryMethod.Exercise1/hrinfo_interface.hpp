#ifndef HRINFO_INTERFACE_HPP
#define HRINFO_INTERFACE_HPP

#include "employee_interface.hpp"

class Employee;

class HRInfo
{
protected:
	const Employee* employee_;
public:
	HRInfo(const Employee* e);
    virtual ~HRInfo() = default;

	virtual void info() const = 0;
};

#endif