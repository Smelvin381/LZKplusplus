#include <iostream>
#include "resources/cslib.hpp"

cslib::CSV donors("donors.csv");
cslib::CSV population("population.csv");

cslib::MessageBuilding mb;

int main() {
    for (unsigned int i = 1; donors.data.size() > i; i++) {
        mb << donors.data[i][0] << std::endl;
    }
    std::cout << mb.g();
    return 0;
}