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
// Classes, Methods... are created on demand but not project-related.


#pragma once // Include only once

#include <filesystem> // File system
#include <iostream> // Input and output
#include <fstream> // File stream
#include <vector> // Array-like containers


namespace cslib {
//-------------- HELPER FUNCTIONS/CONSTANTS --------------//
constexpr char INPUT_PATH[17] = "resources/input/"; // Dictionary with the input files
constexpr char OUTPUT_PATH[18] = "resources/output/"; // Dictionary with the output files

bool PROGRAM_RUNNING = true; // Check if the program is running

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
    static T get_true_value(const std::string& s) { // Special thanks to @Co-Pilot
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



    static std::string cut_decimal(const double& number, const unsigned short decimal_places = 2) {
      // Cut a number to a certain amount of decimal places
      // Example 1: 12.345, 2 -> "12.34"

      std::string numberAsStr = std::to_string(number);
      size_t dotPosition = numberAsStr.find('.');
      if (dotPosition == std::string::npos) {
        return numberAsStr;
      }

      return numberAsStr.substr(0, dotPosition + decimal_places + 1);
    }
    static std::string cut_decimal(const float& number, const unsigned short decimal_places = 2) {
      // Cut a number to a certain amount of decimal places
      // Example 1: 12.345, 2 -> "12.34"

      std::string numberAsStr = std::to_string(number);
      size_t dotPosition = numberAsStr.find('.');
      if (dotPosition == std::string::npos) {
        return numberAsStr;
      }

      return numberAsStr.substr(0, dotPosition + decimal_places + 1);
    }



    static double average(const std::vector<double>& values) {
      // Get the average of a list of values
      // Example: {1, 2, 3} -> 2

      double sum = 0;
      for (double value : values) {
        sum += value;
      }
      return sum / values.size();
    }
};




class CSV { // Depedancies: itself
    // Read and handle CSV files
  public:
    const std::string_view filename;
    const std::vector<std::vector<std::string>> data; // as 2d map


    explicit CSV(const std::string& filename, const char delimiter = ',') : filename(filename), data(g(filename, delimiter)) {
    };



    // Get
    static const std::vector<std::vector<std::string>> g(const std::string& userFilename, const char delimiter = ',') {
      // Get map from a CSV file
      // Example: CSV::g("map.csv");

      std::string filename = INPUT_PATH + userFilename;

      if (!std::filesystem::exists(filename)) {
        throw std::invalid_argument("File does not exist: " + filename);
      }

      std::ifstream file(filename);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      std::vector<std::vector<std::string>> map;
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
        map.push_back(row);
      }

      return map;
    }



    // Write
    static void w(const std::vector<std::vector<std::string>>& data, const std::string& userFilename, const char delimiter = ',') {
      // Write data to a CSV file
      // Example: CSV::w(data, "data.csv");

      std::string filename = OUTPUT_PATH + userFilename;


      if (std::filesystem::exists(filename)) {
        std::cout << "File \" " << filename << " \" already exists. Overwriting..." << std::endl;
      }

      std::ofstream file(filename);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      for (const std::vector<std::string>& row : data) {
        for (size_t i = 0; i < row.size(); i++) {
          file << row[i];
          if (i < row.size() - 1) {
            file << delimiter;
          }
        }
        file << '\n';
      }
    }



    // Print
    void p() {p(data);}
    static void p(const std::vector<std::vector<std::string>>& data) {

      if (data.size() <= 0) {
        std::cout << "No data to print." << std::endl;
        return;
      }

      for (int i = -1; i < data.size(); i++) {
        if (i == -1) {
          std::cout << "Index: ";
          for (int j = 0; j < data[0].size(); j++) {
            std::cout << j;
            if (j < data[0].size() - 1) {
              std::cout << " | ";
            }
          }
          std::cout << std::endl;
        } else {
          std::cout << i << ": ";
          for (int j = 0; j < data[i].size(); j++) {
            std::cout << data[i][j];
            if (j < data[i].size() - 1) {
              std::cout << " | ";
            }
          }
          std::cout << std::endl;
        }
      }
    }
};




static void create_txt(const std::string& data, const std::string& userFilename) {
  // Create a text file with data
  // Example: create_txt("Hello, World!", "hello.txt");

  std::string filename = OUTPUT_PATH + userFilename;

  if (std::filesystem::exists(filename)) {
    std::cout << "File \" " << filename << " \" already exists. Overwriting..." << std::endl;
  }

  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  file << data;
  file.close();
}




class MessageBuilding { // Depedancies: itself (overloading)
  // A class to build messages and spill them out.
  private:
    std::string buildupMessage;

  public:
    // Add string(s)
    MessageBuilding& operator << (const char* message) {this->buildupMessage += message; return *this;}
    MessageBuilding& operator << (const std::string& message) {this->buildupMessage += message; return *this;}
    MessageBuilding& operator<<(std::ostream& (*func)(std::ostream&)) { // Special thanks to @Co-Pilot
      if (func == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
        this->buildupMessage += '\n';
      }
      return *this;
    }


    // Add a string in relation to a empty spaces
    static std::string a(std::string message, const unsigned short spaces) {
      // Example: "test", 5 -> "test "
      // Example: "test", 2 -> "te"

      if (message.size() >= spaces) {
        return message.substr(0, spaces);
      }

      return message + std::string(spaces - message.size(), ' ');
    }



    // Add a break line
    void break_line() {this->buildupMessage += "\n";} // Add a break line to the buildup message



    // Get the data
    const std::string& g() const {return this->buildupMessage;}
 };

}; // End of namespace cslib
