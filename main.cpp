// Project specific functions, variables...

#include <iostream>
#include "resources/cslib.hpp"


cslib::CSV donors("donors.csv");
cslib::CSV population("population.csv");


class LZKplusplus {
    private:
        static constexpr unsigned int PER_INDEX = 1000000;
        std::vector<std::vector<std::string>> donors;
        std::vector<std::vector<std::string>> population;
        cslib::MessageBuilding mb;
        std::vector<std::string> countries; // Excluding header (by donors)
        std::vector<double> averageDonations; // Same order as this->countries (by donors)
        std::vector<double> averageDonationsPerMio; // Same order as this->countries (self calculated using both)
        std::vector<double> populationPerMio; // Same order as this->countries Example: 8'000'000 -> 8 (by population)

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



        void calculatePopPerMio() {
            std::cout << "test";
            for (const std::string& searchingCountry : this->countries) {
                for (int i = 0; i < this->countries.size(); i++) {
                    std::cout << this->population[0][1];
                    if (searchingCountry == this->population[0][i]) {
                        this->populationPerMio.push_back(std::stod(this->population[1][i]) / LZKplusplus::PER_INDEX);
                        break;
                    }
                }
            }
        }

    public:
        explicit LZKplusplus(const std::vector<std::vector<std::string>>& donors, 
        const std::vector<std::vector<std::string>>& population) : donors(donors), population(population) {}

        void main() {
            for (unsigned int i = 1; donors.size() > i; i++) {
                mb << std::to_string(i) << ". ";
                mb << cslib::MessageBuilding::a(donors[i][0], 13);
                this->countries.push_back(donors[i][0]);
                mb << "(avg.: ";
                mb << cslib::MessageBuilding::a(cslib::SwissKnife::cut_decimal(LZKplusplus::calculateAverage(donors[i])),5);
                this->averageDonations.push_back(LZKplusplus::calculateAverage(donors[i]));
                mb << " | ";
                mb << "avg. per million population: ";
 
                mb << std::endl;
            }
            cslib::create_txt(mb.g(),"output.txt");
            // this->calculatePopPerMio();
            // for (const auto& test : this->populationPerMio) {
            //     std::cout << test << std::endl;
            // }
        }
};



int main() {
    LZKplusplus lzKplusplus(donors.g(), population.g()); // Depedancies: donors, population (both CSV)
    lzKplusplus.main();
    return 0;
}