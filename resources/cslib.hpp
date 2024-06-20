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
// Classes, Methods; Constants... are created on demand but not project-related.


#pragma once // Include only once

#include <filesystem> // File system
#include <iostream> // Input and output
#include <fstream> // File stream
#include <vector> // Array-like containers
#include <map> // Key-value containers


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




template <typename T>
class CSV { // Depedancies: itself
    // Read and handle CSV files
  public:
    const std::string_view filename;
    std::map<std::string, std::vector<T>> data;



    // Constructor
    // Example: CSV<std::string> donors("donors.csv");
    explicit CSV(const std::vector<std::vector<T>>& data) : data(switch_dimension(data)) {}
    explicit CSV(const char* filename, const char delimiter = ',') : filename(filename), data(g<T>(filename, delimiter)) {}



    // Get
    template <typename U>
    static std::map<std::string, std::vector<U>> g(const std::string& filename, const char delimiter = ',') {
      // Get the data of a CSV file
      // Example: "hello.csv" -> {{"1", {1, 2, 3}}, {"2", {4, 5, 6}}}

      std::map<std::string, std::vector<U>> data;
      std::ifstream file(INPUT_PATH + filename);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      std::string line;
      while (std::getline(file, line)) {
        std::string key;
        std::vector<U> values;
        std::istringstream iss(line);
        std::string value;
        std::getline(iss, key, delimiter);
        while (std::getline(iss, value, delimiter)) {
          values.push_back(SwissKnife::get_true_value<U>(value));
        }
        data[key] = values;
      }

      file.close();
      return data;
    }



    // Print
    void p() {p<std::string>(this->data);}
    static void p(const std::map<std::string, std::vector<std::string>>& data) {
      // Print the data of a CSV file
      // Example: {{"1", {1, 2, 3}}, {"2", {4, 5, 6}}} -> 1, 2, 3
      //                                                        4, 5, 6

      for (const auto& [key, values] : data) {
        std::cout << key;
        for (const std::string& value : values) {
          std::cout << ", " << value;
        }
        std::cout << std::endl;
      }
    }



    // Switch dimension of a vector and turn it into a map
    void switch_dimension() {this->data = switch_dimension<T>(this->data);}
    static std::map<std::string, std::vector<std::string>> switch_dimension(const std::vector<std::vector<U>>& data) {
      // Switch the dimension of a vector and turn it into a map (only supports 1D vectors)
      // Example: {{"1", "2", "3"}, {"4", "5", "6"}} -> {{"1", {"4"}}, {"2", {"5"}}, {"3", {"6"}}}

      std::map<std::string, std::vector<std::string>> newData;
      for (const std::vector<std::string>& innerVec : data) {
        for (size_t i = 0; i < innerVec.size(); i++) {
          newData[std::to_string(i)].push_back(innerVec[i]);
        }
      }
      return newData;
    }



    // Get data from 2D vector by filename
    static std::vector<std::vector<T>> get_data(const std::string& filename, const char delimiter = ',') {
      // Get the data of a CSV file
      // Example: "hello.csv" -> {{1, 2, 3}, {4, 5, 6}}

      std::vector<std::vector<T>> data;
      std::ifstream file(INPUT_PATH + filename);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      std::string line;
      while (std::getline(file, line)) {
        std::vector<T> values;
        std::istringstream iss(line);
        std::string value;
        while (std::getline(iss, value, delimiter)) {
          values.push_back(SwissKnife::get_true_value<T>(value));
        }
        data.push_back(values);
      }

      file.close();
      return data;
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
