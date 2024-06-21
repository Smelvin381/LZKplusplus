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

      static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type"); // Only allow number data types

      T value;

      std::istringstream iss(s);
      if (!(iss >> value) or iss.rdbuf()->in_avail() != 0) { // Check if the value is valid
        throw std::invalid_argument("Invalid value (" + std::string(typeid(T).name()) + "): " + s);
      }
      return value;
    }



    static std::string cut_decimal(const std::string& number, const unsigned short decimal_places = 2) {
      // Cut a number to a certain amount of decimal places
      // Example 1: "12.345", 2 -> "12.34"

      size_t dotPosition = number.find('.');
      if (dotPosition == std::string::npos) {
        return number;
      }
      return number.substr(0, dotPosition + decimal_places + 1);
    }



    template <typename T>
    static T average(const std::vector<T>& values) {
      // Get the average of a list of values
      // Example: {1, 2, 3} -> 2
      static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type"); // Only allow number data types

      double sum = 0;
      for (double value : values) {
        sum += value;
      }
      return sum / values.size();
    }
    static std::string average(const std::vector<std::string>& values) {
      // Get the average of a list of values
      // Example: {"1", "2", "3"} -> "2"
      std::vector<double> valuesAsDouble;
      for (const std::string& value : values) {
        try {
          valuesAsDouble.push_back(std::stod(value));
        } catch (...) {continue;}
      }
      return cut_decimal(std::to_string(average(valuesAsDouble)));
    }
};




class CSV { // Depedancies: itself
  // Read and handle CSV files
  private:
    // Get
    static std::vector<std::vector<std::string>> get_vector(const std::string& filename, const char delimiter = ',') {
      // Get the data of a CSV file
      // Example: "hello.csv" -> {{"1", "2", "3"}, {"4", "5", "6"}}

      std::vector<std::vector<std::string>> data;
      std::ifstream file(INPUT_PATH + filename);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
      }

      std::string line;
      while (std::getline(file, line)) {
        std::vector<std::string> innerVec;
        size_t start = 0;
        size_t end = line.find(delimiter);
        while (end != std::string::npos) {
          innerVec.push_back(line.substr(start, end - start));
          start = end + 1;
          end = line.find(delimiter, start);
        }
        innerVec.push_back(line.substr(start, end));
        data.push_back(innerVec);
      }
      file.close();
      return data;
    }

  public:
    const std::string_view filename;
    std::map<std::string, std::vector<std::string>> map;
    std::vector<std::vector<std::string>> data;

    // Constructor
    explicit CSV(const std::string& filename, const char delimiter) : filename(filename) {
      this->data = get_vector(filename, delimiter);
      this->map = convert(this->data);
    }



    // Convert 2d vector into 1d map + 1d vector
    static std::map<std::string, std::vector<std::string>> convert(const std::vector<std::vector<std::string>>& data) {
      // Convert a 2D vector into a 1D map + 1D vector
      // Example: {{"1", "2", "3"}, {"4", "5", "6"}}  -> {{"1", {"2", "3"}}, {"4", {"5", "6"}}}

      std::map<std::string, std::vector<std::string>> newData;
      for (const std::vector<std::string>& innerVec : data) {
        newData[innerVec[0]] = std::vector<std::string>(innerVec.begin() + 1, innerVec.end());
      }
      return newData;
    }



    // Print content
    void print_map() const {
      for (const auto& [key, value] : this->map) {
        std::cout << key << ": ";
        for (const std::string& innerValue : value) {
          std::cout << innerValue << " ";
        }
        std::cout << std::endl;
      }
    }
    void print_vector() const {
      for (const std::vector<std::string>& innerVec : this->data) {
        for (const std::string& value : innerVec) {
          std::cout << value << " ";
        }
        std::cout << std::endl;
      }
    }
};




static void create_txt(const std::string& data, const std::string& userFilename) {
  // Create a text file with data
  // Example: create_txt("Hello, World!", "hello.txt");

  std::string filename = OUTPUT_PATH + userFilename;

  if (std::filesystem::exists(filename)) {
    std::cout << "File \"" << filename << "\" already exists. Overwriting..." << std::endl;
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
    enum class alignment {
      LEFT,
      RIGHT
    };
    static std::string a(std::string message, const unsigned short spaces, alignment align = alignment::LEFT) {
      // Example: "test", 5, RIGHT -> " test"
      // Example: "test", 2 -> "te"

      switch (align) {
        case alignment::LEFT:
          if (message.size() >= spaces) {
          return message.substr(0, spaces);
          }
          return message + std::string(spaces - message.size(), ' ');

        case alignment::RIGHT:
          if (message.size() >= spaces) {
            return message.substr(0, spaces);
          }
          return std::string(spaces - message.size(), ' ') + message;
        
        default:
          return message;
    }
    }



    // Add a break line
    void break_line() {this->buildupMessage += "\n";} // Add a break line to the buildup message



    // Get the data
    const std::string& g() const {return this->buildupMessage;}
 };

}; // End of namespace cslib
