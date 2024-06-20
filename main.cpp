// Project specific functions, variables...

#include "resources/cslib.hpp"



cslib::CSV<int> population("population.csv", ';');
cslib::CSV<int> donors("donors.csv", ',');



template <typename T>
static void print_content(const std::map<std::string, std::vector<T>>& data) {
    for (const auto& [key, values] : data) {
        std::cout << key;
        for (const T& value : values) {
            std::cout << ", " << value;
        }
        std::cout << std::endl;
    }
}



template <typename T>
static void print_content(const std::vector<std::vector<T>>& vec) {
    for (const std::vector<T>& innerVec : vec) {
        for (const T& value : innerVec) {
            std::cout << value << std::endl;
        }
    }
}



template <typename T>
static void print_content(const std::vector<T>& vec) {
    for (const T& value : vec) {
        std::cout << value << std::endl;
    }
}




class LZKplusplus {
    private: // Attributes
        static constexpr unsigned int PER_INDEX = 1000000;
        std::map<std::string, std::vector<int>> donors; // "Country", {donations, for, each, year}
        std::map<std::string, int> population; // "Country", population
        std::map<std::string, double> averageDonations; // "Country", average donations
        std::map<std::string, double> populationPerMio; // "Country", population per million
        std::map<std::string, double> donationsPerMio; // "Country", donations per million
        std::vector<std::string> countries; // "Country", "Country", "Country", ...
        cslib::MessageBuilding mb; // For output

    protected: // Helper functions
        template <typename U, typename I> // U = output, I = input
        static U calc_avg(const std::map<std::string, std::vector<I>>& data) {
            double sum = 0;
            for (const auto& [key, values] : data) {
                if (key == "country") continue; // Skip the first element "country"
                for (const U& value : values) {
                    sum += value;
                }
            }
            return sum / data.size();
        }
        template <typename U, typename I> // U = output, I = input
        static U calc_avg(const std::vector<I>& vec) {
            double sum = 0;
            for (const U& value : vec) {
                sum += value;
            }
            return sum / vec.size();
        }



        void calc_avg_by_mio() {
            // Calculate the average donations per a million population
            for (const std::string& country : this->countries) {
                this->donationsPerMio[country] = this->averageDonations[country] / this->populationPerMio[country];
            }
        }



        void calc_pop_per_mio() {
            // Calculate the population per a million
            for (const std::string& country : this->countries) {
                this->populationPerMio[country] = this->population[country] / LZKplusplus::PER_INDEX;
            }
        }



        void setup() {
            // Setup the values
            this->calc_pop_per_mio();
            this->calc_avg_by_mio();
        }



    //     void main() {
    //         for (unsigned int i = 1; donors.size() > i; i++) {
    //             mb << std::to_string(i) << ". ";
    //             mb << cslib::MessageBuilding::a(donors[i][0], 13);
    //             this->countries.push_back(donors[i][0]);
    //             mb << "(avg.: ";
    //             mb << cslib::MessageBuilding::a(cslib::SwissKnife::cut_decimal(LZKplusplus::calculateAverage(donors[i])),5);
    //             this->averageDonations.push_back(LZKplusplus::calculateAverage(donors[i]));
    //             mb << " | ";
    //             mb << "avg. per million population: ";
 
    //             mb << std::endl;
    //         }
    //         cslib::create_txt(mb.g(),"output.txt");
    //         this->calculatePopPerMio();
    //         print_vector_content(this->populationPerMio);
    //     }
};



int main() {
    // donors.data[0].erase(donors.data[0].begin());
    // population.data[0].erase(population.data[0].begin());
    // population.data[1].erase(population.data[1].begin());
    // population.change_dimension();

    // LZKplusplus lzKplusplus(donors.data, population.data); // Depedancies: donors, population (both CSV)
    // lzKplusplus.main();
    population.switch_dimension();
    return 0;
}