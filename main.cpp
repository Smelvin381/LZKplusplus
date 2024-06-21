// Project specific functions, variables...

#include "resources/cslib.hpp" // Custom library (includes all used libraries)
using enum cslib::MessageBuilding::alignment;

cslib::CSV population("population.csv", ';');
cslib::CSV donors("donors.csv", ',');


class LZKplusplus {
    private: // Atributes
        static constexpr unsigned int PER_INDEX = 1000000;
        cslib::CSV population;
        cslib::CSV donors;

        cslib::MessageBuilding output;

        // Calculated values (all as strings to avoid precision errors)
        std::vector<std::string> countries;
        std::map<std::string, std::string> averageDonations;
        std::map<std::string, std::string> populationPerMio;
        std::map<std::string, std::string> donationsPerMio;
        std::map<std::string, std::string> trend;

    protected: // Helper functions


    public: // Methods
        LZKplusplus(cslib::CSV population, cslib::CSV donors) : population(population), donors(donors) {
            // Calculate attributes

            // Get countries
            for (unsigned int i = 1; donors.data.size() > i; i++) {
                std::string country = donors.data[i][0];
                this->countries.push_back(country);
            }

            // Get average donations
            for (const std::string& country : this->countries) {
                std::vector<std::string> donations = this->donors.map[country];
                this->averageDonations[country] = cslib::SwissKnife::average(donations);
            }

            // Get population per million
            const unsigned int vectorX = this->population.data[0].size();
            const unsigned int vectorY = this->population.data.size();
            for (unsigned int i = 1; vectorX > i; i++) {
                std::string country = this->population.data[0][i];
                std::string population = this->population.data[1][i];
                this->populationPerMio[country] = std::to_string(std::stod(population) / PER_INDEX);
            }

            // Get donations per million
            for (const std::string& country : this->countries) {
                this->donationsPerMio[country] = cslib::SwissKnife::cut_decimal(std::to_string(std::stod(this->averageDonations[country]) / std::stod(this->populationPerMio[country])));
            }

            // Determine the current course of donations
            for (const std::string& country : this->countries) {
                if (std::stod(this->donors.map[country][0]) < std::stod(this->donors.map[country][9])) {
                    this->trend[country] = "ascending";
                } else if (std::stod(this->donors.map[country][0]) > std::stod(this->donors.map[country][9])) {
                    this->trend[country] = "descending";
                } else {
                    this->trend[country] = "stable";
                }
            }
        }
        ~LZKplusplus() {cslib::create_txt(this->output.g(), "output.txt");}

        void main() {
            for (unsigned int i = 0; this->countries.size() > i; i++) {
                output << std::to_string(i + 1) << ". ";
                output << cslib::MessageBuilding::a(this->countries[i], 13);
                output << "(avg.: ";
                output << cslib::MessageBuilding::a(this->averageDonations[this->countries[i]], 5, RIGHT);
                output << " | ";
                output << "avg. per million population: ";
                output << cslib::MessageBuilding::a(this->donationsPerMio[this->countries[i]], 5, RIGHT);
                output << " | ";
                output << "trend: ";
                output << cslib::MessageBuilding::a(this->trend[this->countries[i]], 13, RIGHT);
                output << ")";


                output << std::endl;
            }
        }
};



int main() {
    LZKplusplus lzKplusplus(population, donors);
    lzKplusplus.main();
    return 0;
}