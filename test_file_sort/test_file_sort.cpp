#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "../utils/utils.hpp"

int main(int argc, char *argv[]) {
    std::string postfix = argc >= 2 ? argv[1] : "100";

    std::string input_file_name = vectorToString(std::vector<std::string> { "../data/input/file_to_sort_", postfix, ".txt" });
    std::ifstream input_file(input_file_name);
    std::unordered_map<int, std::shared_ptr<FileLine>> input_lines;
    if (input_file.is_open()) {
        std::string one_line;

        while (getline(input_file, one_line)) {
            try {
                // std::cout << "Read one line: " << one_line << '\n';
                std::shared_ptr<FileLine> file_line = std::make_shared<FileLine>(one_line);
                file_line->parse();
                input_lines.insert({ {file_line->lineNumber(), file_line} });
            }
            catch (std::exception& e) {
                std::cout << "Failed to prepare FileLine object from input line " << one_line << '\n';
            }
        }
        
        input_file.close();
    }
    else {
        std::cout << "Failed to open input " << input_file_name << '\n';
        return 1;
    }

    std::ofstream report_file;
    try {
        report_file.open(vectorToString(std::vector<std::string> { "../data/report/file_to_sort_", postfix, ".txt" }));
    }
    catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
        return 2;
    }

    std::string output_file_name = vectorToString(std::vector<std::string> { "../data/output/file_sorted_", postfix, ".txt" });
    std::ifstream output_file(output_file_name);
    std::unordered_map<int, std::shared_ptr<FileLine>> output_lines;
    if (output_file.is_open()) {
        std::string one_line;
        std::shared_ptr<FileLine> current_line;

        while (getline(output_file, one_line)) {
            try {
                // std::cout << "Read one line: " << one_line << '\n';
                std::shared_ptr<FileLine> file_line = std::make_shared<FileLine>(one_line);
                file_line->parse();
                if (current_line) {
                    if (file_line->key() < current_line->key()) {
                        report_file << "Error: " << file_line->toString() << " should locate after " << current_line->key() << '\n';
                    }
                }
                current_line = file_line;
                output_lines.insert({ {file_line->lineNumber(), file_line} });
            }
            catch (std::exception& e) {
                std::cout << "Failed to prepare FileLine object from output line " << one_line << '\n';
            }
        }
        
        output_file.close();
    }
    else {
        std::cout << "Failed to open output " << output_file_name << '\n';
        return 3;
    }

    if (input_lines.size() == output_lines.size()) {
        report_file << "Ok: output size " << output_lines.size() << " matches input size " << input_lines.size() << '\n';
    }
    else {
        report_file << "Error: output size " << output_lines.size() << " does not match to input size " << input_lines.size() << '\n';
    }

    for (const auto& [key, value] : input_lines) {
        if (output_lines.find(key) == output_lines.end()) {
            report_file << "Error: input " << value->toString() << " is missing from output.\n";
        }
    }

    report_file.close();
}