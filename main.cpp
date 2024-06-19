// Project specific functions, variables...

#include <iostream>
#include "resources/cslib.hpp"

cslib::CSV donors("donors.csv");
cslib::CSV population("population.csv");


cslib::MessageBuilding mb;


class LZKplusplus {
    private:
        static constexpr unsigned int PER_INDEX = 1000000;

    public:

        

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
};



int main() {
    for (unsigned int i = 1; donors.data.size() > i; i++) {
        mb << std::to_string(i) << ". ";
        mb << cslib::MessageBuilding::a(donors.data[i][0], 13);
        mb << "(avg.: ";
        mb << cslib::MessageBuilding::a(cslib::SwissKnife::cut_decimal(LZKplusplus::calculateAverage(donors.data[i])),5);
        mb << " | ";
        mb << "avg. per million population: ";
        mb << std::endl;
    }
    cslib::create_txt(mb.g(),"output.txt");
    return 0;
}