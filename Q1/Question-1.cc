#include <iostream>
#include <fstream>
#include <string>

// Question 1: This is an extension task that requires you to decode sensor data from a CAN log file.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the "WheelSpeedRR" values
// from the candump.log file. Parse these values correctly and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the expected data output format.
// You do not need to use any external libraries. Use the resources below to understand how to extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro

int main() {
    std::ifstream inputFile("../Q1/candump.log");
    if (!inputFile) {
        std::cerr << "error opening file: candump.log" << std::endl;
        return 1;
    }

    std::ofstream outFile("output.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "error opening file: output.txt" << std::endl;
        inputFile.close();
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find("705#") != std::string::npos) {
            // hard coded values
            // repeat find | inefficient?
            std::string data = line.substr(line.find("705#") + 4 + 8, 4);

            // little -> big
            std::string first_byte = data.substr(0, data.length() / 2);
            std::string second_byte = data.substr(data.length() / 2);
            std::string big_e_hex = second_byte + first_byte;

            // scale + offset
            int16_t bit_value = std::stoi(big_e_hex, nullptr, 16);
            float value = bit_value * 0.1;
            
            std::string timestamp = line.substr(0, 19);

            // final
            outFile << timestamp + ": " << value << std::endl;
        }
    }

    outFile.close();
    inputFile.close();
    return 0;
}
