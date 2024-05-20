#ifndef HRINFO_HPP_
#define HRINFO_HPP_

#include "hrinfo_interface.hpp"

class StdInfo : public HRInfo
{
public:
    using HRInfo::HRInfo;

    void info() const override;
};

class TempInfo : public HRInfo
{
public:
    using HRInfo::HRInfo;

    void info() const override;
};

#endif /* HRINFO_HPP_ */
