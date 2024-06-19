// This file is part of the rework of the LZK++ project.
//
// LZK++ is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LZK++ is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LZK++. If not, see <https://www.gnu.org/licenses/>.
//
// Contact: @Smelvin381 on GitHub
//
// Context based functions,variables and classes


#pragma once // Include only once

#include <filesystem> // File system
#include <iostream> // Input and output
#include <fstream> // File stream
#include <vector> // Array-like containers
#include <memory> // Smart pointers

namespace cslib {
//-------------- HELPER FUNCTIONS/CONSTANTS --------------//
constexpr std::string_view INPUT_PATH = "resources/input"; // Dictionary with the input files
constexpr std::string_view OUTPUT_PATH = "resources/output"; // Dictionary with the output files

bool PROGRAM_RUNNING = true; // Check if the program is running



// Secondary functions
template<typename... Args>
void STOP_PROGRAM(std::unique_ptr<Args>&... args) {
  // Gracefully stop the program

  std::cout << "\033[31m \u001b[1m ";
  std::cout << " @ STOPPED without destructing objects.";
  std::cout << " \033[0m" << std::endl;

  PROGRAM_RUNNING = false;

  // Delete the dynamically allocated objects
  // Unique pointer made like this: std::unique_ptr<cslib::ThreadParty> threadPool(new cslib::ThreadParty());
  (args.reset(), ...);

  // Run std::exit(0) in a separate thread
  auto asyncResult = std::async(std::launch::async, [](){ std::exit(0); });
  asyncResult.wait(); // Wait for the async operation to complete
}
void STOP_PROGRAM() {
  // Gracefully stop the program without args

  std::cout << "\033[31m \u001b[1m ";
  std::cout << " @ STOPPED";
  std::cout << " \033[0m" << std::endl;

  PROGRAM_RUNNING = false;
  std::exit(0);
}





//-------------- CLASSES --------------//
class SwissKnife { // Depedancies: None
  // An highly specialized class to handle tasks
  public:
    template <typename T>
    static T get_true_value(const std::string& s) {
      // Convert string to value
      // Example: "12AB.34CD" -> 12.34
      // Example 2: "1 2 3 4" -> 1

      if (std::is_same<T, char>::value) {
        return s.empty() ? '\0' : s[0];
      }

      T value;
      if (std::is_same<T, bool>::value) {
        if (s == "true" or s == "True" or s == "TRUE" or s == "1") {
          return 1;
        } else if (s == "false" or s == "False" or s == "FALSE" or s == "0") {
          return 0;
        }
      }

      std::istringstream iss(s);
      if (!(iss >> value) or iss.rdbuf()->in_avail() != 0) { // Check if the value is valid
        throw std::invalid_argument("Invalid value ("+std::string(typeid(T).name())+"): " + s);
      }
      return value;
    }
};




class CSV { // Depedancies: itself
    // Read and handle CSV files
  private:
    std::vector<std::vector<std::string>> data; // as 2d map
    const std::string_view filename;
    const char delimiter;

  public:
    explicit CSV(const std::string& filename, const char delimiter = ',') : filename(filename), delimiter(delimiter) {
      // Read the CSV file
      // Example: CSV csv("data.csv");
      if (!std::filesystem::exists(filename)) {
        throw std::invalid_argument("File does not exist: " + filename);
      }

      std::ifstream file(filename);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      std::string line;
      
      while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::string cell;

        for (char c : line) {
          if (c == delimiter) {
            row.push_back(cell);
            cell.clear();
          } else {
            cell += c;
          }
        }

        row.push_back(cell);
        data.push_back(row);
      }
    }


    // Get
    const std::vector<std::vector<std::string>>& g() const {return data;}



    // Write
    static void w(const std::vector<std::vector<std::string>>& data) {
      // Write the data to a CSV file
      // Example: CSV::w(data);

      std::ofstream file("output.csv");
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: output.csv");
      }

      for (const std::vector<std::string>& row : data) {
        for (const std::string& cell : row) {
          file << cell << ',';
        }
        file << '\n';
      }
    }
};



class MessageBuilding { // Depedancies: itself (overloading)
  // A class to build messages and spill them out.
  private:
    std::string buildupMessage;

  public:
    // With operator overloading
    MessageBuilding& operator << (const std::string& message) {this->buildupMessage += message; return *this;}
    MessageBuilding& operator << (const std::string_view& message) {this->buildupMessage += message; return *this;}


    // Add a break line
    void break_line() {this->buildupMessage += "\n";} // Add a break line to the buildup message



    // Get the string
    const std::string& g() const {return this->buildupMessage;}
};

}; // End of namespace cslib
