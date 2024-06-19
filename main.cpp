// Project specific functions, variables...

#include <iostream>
#include "resources/cslib.hpp"
#include "resources/json.hpp"




class LZKplusplus {
    private:
        static constexpr unsigned int PER_INDEX = 1000000;
        cslib::CSV donors;
        cslib::CSV population;
        cslib::MessageBuilding mb;
        std::vector<std::string> countries;
        std::vector<double> averageDonations; // Same order as this->countries
        std::vector<double> averageDonationsPerMio; // Same order as this->countries

    protected:
        static double calculateAverage(std::vector<std::string> values) {
        if (values.empty()) {return 0;}
            values.erase(values.begin()); // Remove header
        double sum = 0;
        for (const std::string& value : values) {
            try {
                sum += std::stoi(value);
            } catch (...) {return 0;}
            }
            return sum / values.size();
        }



        static double calculateAveragePerMio(std::vector<std::string> relationCalculationValues) {
        if (relationCalculationValues.empty()) {return 0;}

        relationCalculationValues.erase(relationCalculationValues.begin()); // Remove header

        return 0;
        }

    public:
        explicit LZKplusplus() : donors("donors.csv"), population("population.csv") {}



        void main() {
            for (unsigned int i = 1; donors.data.size() > i; i++) {
                mb << std::to_string(i) << ". ";
                mb << cslib::MessageBuilding::a(donors.data[i][0], 13);
                this->countries.push_back(donors.data[i][0]);
                mb << "(avg.: ";
                mb << cslib::MessageBuilding::a(cslib::SwissKnife::cut_decimal(LZKplusplus::calculateAverage(donors.data[i])),5);
                this->averageDonations.push_back(LZKplusplus::calculateAverage(donors.data[i]));
                mb << " | ";
                mb << "avg. per million population: ";
                mb << std::endl;
            }
            cslib::create_txt(mb.g(),"output.txt");
        }
};



int main() {
    LZKplusplus lzKplusplus;
    lzKplusplus.main();
    return 0;
}