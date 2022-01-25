#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "../utils/utils.hpp"

auto compareFileLine = [](const std::shared_ptr<FileLine>& a, const std::shared_ptr<FileLine>& b) {
    return a->key() != b->key() ? a->key() < b->key() : a->lineNumber() < b->lineNumber();
};

struct CompareFileLine {
    bool operator() (const std::shared_ptr<FileLine>& a, const std::shared_ptr<FileLine>& b) {
        return !compareFileLine(a, b);
    }
};

typedef std::priority_queue<std::shared_ptr<FileLine>,
        std::vector<std::shared_ptr<FileLine>>,
        CompareFileLine> FileLineQueue;

void sortAndSaveChunk(std::vector<std::shared_ptr<FileLine> >& vect, const std::string& fileName) {
    std::sort(vect.begin(), vect.end(), compareFileLine);

    std::ofstream temp_file;
    try {
        temp_file.open(fileName);
        std::for_each(vect.begin(), vect.end(), [&temp_file](const std::shared_ptr<FileLine>& elem) {
            temp_file << elem->toString() << '\n';
        });
        std::cout << "Successfully writen " << vect.size() << " to " << fileName << '\n';
    }
    catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    temp_file.close();
}

int separateFileAndSortChunks(int linesPerFile, const std::string& postfix) {
    std::string file_name = vectorToString(std::vector<std::string> { "../data/input/file_to_sort_", postfix, ".txt" });
    std::ifstream file_to_sort(file_name);
    int file_index = 0;
    if (file_to_sort.is_open()) {
        std::vector<std::shared_ptr<FileLine>> file_lines;
        file_lines.reserve(linesPerFile);
        std::string one_line;
        int line_index = 0;

        while (getline(file_to_sort, one_line)) {
            try {
                // std::cout << "Read one line: " << one_line << '\n';
                std::shared_ptr<FileLine> file_line = std::make_shared<FileLine>(one_line);
                file_line->parse();
                file_lines.push_back(file_line);
            }
            catch (std::exception& e) {
                std::cout << "Failed to prepare FileLine object from line " << one_line << '\n';
            }
            line_index++;
            if (line_index >= linesPerFile) {
                file_index++;
                line_index = 0;
                sortAndSaveChunk(file_lines, vectorToString(std::vector<std::string> {
                    "../data/temp/file_to_sort_", postfix, "/", std::to_string(file_index), ".txt" }));
                file_lines.clear();
            }
        }
        if (file_lines.size() > 0) {
            file_index++;
            sortAndSaveChunk(file_lines, vectorToString(std::vector<std::string> {
            "../data/temp/file_to_sort_", postfix, "/", std::to_string(file_index), ".txt" }));
        }
        file_to_sort.close();

        return file_index;
    }
    else {
        std::cout << "Failed to open " << file_name << '\n';
        return 0;
    }
}
void pushOneLineToQueue(const std::string& line, int index, FileLineQueue& lineQ) {
    std::shared_ptr<FileLine> file_line = std::make_shared<FileLine>();
    file_line->fromString(line);
    file_line->fileIndex(index);
    lineQ.push(file_line);
    // std::cout << file_line->toString() << " from file " << index << " added to Q.\n";
}

void mergeSortChunks(int numChunks, const std::string& postfix) {
    std::vector<std::shared_ptr<std::ifstream>> temp_files;
    temp_files.reserve(numChunks);
    for (int i = 1; i <= numChunks; ++i) {
        std::shared_ptr<std::ifstream> temp_file = std::make_shared<std::ifstream>(
            vectorToString(std::vector<std::string> {
                "../data/temp/file_to_sort_", postfix, "/", std::to_string(i), ".txt" }));
        if (temp_file->is_open()) {
            temp_files.push_back(temp_file);
        }
    }

    FileLineQueue file_lines;

    int num_files = temp_files.size();
    std::string one_line;
    for (int i = 0; i < num_files; ++i) {
        if (getline(*temp_files[i], one_line)) {
            // std::cout << "Get one line: " << one_line << " from " << i << '\n';
            pushOneLineToQueue(one_line, i, file_lines);
        }
    }

    std::ofstream file_sorted;
    try {
        file_sorted.open(vectorToString(std::vector<std::string> { "../data/output/file_sorted_", postfix, ".txt" }));
        while (!file_lines.empty()) {
            const std::shared_ptr<FileLine>& file_line = file_lines.top();
            file_sorted << file_line->content() << '\n';
            // std::cout << file_line->toString() << " written to output.\n";
            int file_index = file_line->fileIndex();
            file_lines.pop();

            while (true) {
                if (getline(*temp_files[file_index], one_line)) {
                    pushOneLineToQueue(one_line, file_index, file_lines);
                    break;
                }
                else {
                    temp_files[file_index]->close();
                    if ((num_files = temp_files.size()) >= 2) {
                        temp_files[file_index] = temp_files[num_files - 1];
                        temp_files.resize(num_files - 1);
                    }
                    break;
                }
            }
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
    file_sorted.close();
}

int main(int argc, char *argv[]) {
    std::string postfix = argc >= 2 ? argv[1] : "100";
    int lines_per_file = SLICING_CONFIG.find(postfix) != SLICING_CONFIG.end() ? SLICING_CONFIG.at(postfix) : 25;

    int num_chunks = separateFileAndSortChunks(lines_per_file, postfix);

    mergeSortChunks(num_chunks, postfix);

    return 0;
}