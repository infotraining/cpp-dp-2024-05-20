#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string& desc, double val)
        : description(desc)
        , value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;

// enum StatisticsType
// {
//     avg,
//     min_max,
//     sum
// };

class CalculationStrategy
{
public:
    virtual ~CalculationStrategy() = default;
    virtual void calculate(const Data& data, Results& outResults) = 0;
};

class StatisticsGroup : public CalculationStrategy // Composite 
{
    std::vector<std::shared_ptr<CalculationStrategy>> stats_;
public:
    void calculate(const Data& data, Results& outResults) override
    {
        for(auto stat : stats_)
            stat->calculate(data, outResults);
    }

    void add(std::shared_ptr<CalculationStrategy> stat)
    {
        stats_.push_back(stat);
    }
};

class AvgCalculationStrategy : public CalculationStrategy
{
public:
    void calculate(const Data& data, Results& outResults) override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();

        StatResult result("Avg", avg);
        outResults.push_back(result);
    }
};

class MinMaxCalculationStrategy : public CalculationStrategy
{
public:
    void calculate(const Data& data, Results& outResults) override
    {
        double min = *(std::min_element(data.begin(), data.end()));
        double max = *(std::max_element(data.begin(), data.end()));

        outResults.push_back(StatResult("Min", min));
        outResults.push_back(StatResult("Max", max));
    }
};

class SumCalculationStrategy : public CalculationStrategy
{
public:
    void calculate(const Data& data, Results& outResults) override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);

        outResults.push_back(StatResult("Sum", sum));
    }
};

class DataAnalyzer
{
    Data data_;
    Results results_;
    std::shared_ptr<CalculationStrategy> calculation_strategy_;

public:
    DataAnalyzer(std::shared_ptr<CalculationStrategy> calculationStrategy)
        : calculation_strategy_{calculationStrategy}
    {
    }

    void load_data(const std::string& file_name)
    {
        data_.clear();
        results_.clear();

        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");

        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }

        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_calculation_strategy(std::shared_ptr<CalculationStrategy> calculationStrategy)
    {
        calculation_strategy_ = calculationStrategy;
    }

    void calculate()
    {
        calculation_strategy_->calculate(data_, results_);
    }

    const Results& results() const
    {
        return results_;
    }
};

namespace StdFunction
{
    class DataAnalyzer
    {
    public:
        using AnalyzeStrategy = std::function<Results(const Data&)>;

    private:
        AnalyzeStrategy strategy_;
        Data data_;
        Results results_;

    public:
        DataAnalyzer(AnalyzeStrategy strategy)
            : strategy_(std::move(strategy))
        {
        }

        void load_data(const std::string& file_name)
        {
            data_.clear();
            results_.clear();

            std::ifstream fin(file_name.c_str());
            if (!fin)
                throw std::runtime_error("File not opened");

            double d;
            while (fin >> d)
            {
                data_.push_back(d);
            }

            std::cout << "File " << file_name << " has been loaded...\n";
        }

        void set_statistics(AnalyzeStrategy strategy)
        {
            strategy_ = std::move(strategy);
        }

        void calculate()
        {
            auto newResults = strategy_(data_);
            std::move(newResults.begin(), newResults.end(),
                std::back_inserter(results_));
        }

        const Results& results() const { return results_; }
    };
}

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

void alternative_main()
{
    auto avg = [](const Data& data) -> Results
    {
        return Results{StatResult{
            "Avg", std::accumulate(data.begin(), data.end(), 0.0) / data.size()}};
    };

    auto min_max = [](const Data& data) -> Results
    {
        double min = *std::min_element(data.begin(), data.end());
        double max = *std::max_element(data.begin(), data.end());
        return Results{
            StatResult("min", min), StatResult("max", max)};
    };

    auto sum = [](const Data& data) -> Results
    {
        return Results{
            StatResult("min", std::accumulate(data.begin(), data.end(), 0.0))};
    };
    
    StdFunction::DataAnalyzer da{avg};
    da.load_data("stats_data.dat");

    da.calculate();

    da.set_statistics(min_max);
    da.calculate();

    da.set_statistics(sum);
    da.calculate();

    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_stats_data.dat");
    da.calculate();

    show_results(da.results());
}

int main()
{
    auto avg = std::make_shared<AvgCalculationStrategy>();
    auto min_max = std::make_shared<MinMaxCalculationStrategy>();
    auto sum = std::make_shared<SumCalculationStrategy>();

    auto std_stats = std::make_shared<StatisticsGroup>();
    std_stats->add(avg);
    std_stats->add(min_max);
    std_stats->add(sum);

    DataAnalyzer da{std_stats};
    da.load_data("stats_data.dat");
    da.calculate();

    // da.set_calculation_strategy(min_max);
    // da.calculate();

    // da.set_calculation_strategy(sum);
    // da.calculate();

    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_stats_data.dat");
    da.calculate();

    show_results(da.results());
}
