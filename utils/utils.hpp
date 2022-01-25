#ifndef UTILS_HPP
#define UTILS_HPP

#include <ctime>
#include <exception>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, int> SLICING_CONFIG = {
    {"10", 4},
    {"100", 25},
    {"10K", 1600},
    {"1M", 80000},
    {"100M", 800000}
};

std::string vectorToString(const std::vector<std::string>&);

void printTm(const tm& timeTm);

const char* timeInt2Str(int secondsSinceEpoch);

tm timeStr2Tm(const char* timeStr);

class FileLine final {
    std::string _content;
    int _lineNumber;
    time_t _key;
    int _fileIndex;
public:
    FileLine() : _content(""), _lineNumber(0), _key(0), _fileIndex(0) {}
    FileLine(const std::string& content) try : _content(content), _lineNumber(0), _key(0), _fileIndex(0) {}
    catch (const std::exception& e) {
        std::cout << "Failed to instantiate FileLine.\n";
        throw;
    }
    FileLine(const FileLine& other) try :
        _content(other._content), _lineNumber(other._lineNumber), _key(other._key), _fileIndex(other._fileIndex) {}
        catch (const std::exception& e) {
        std::cout << "Failed to copy construct FileLine.\n";
        throw;
    }
    FileLine& operator=(const FileLine& other) {
        FileLine temp(other);
        this->swap(temp);
        return *this;
    }

    void swap(FileLine& line) {
        std::swap(this->_content, line._content);
        std::swap(this->_lineNumber, line._lineNumber);
        std::swap(this->_key, line._key);
        std::swap(this->_fileIndex, line._fileIndex);
    }

    void parse() {
        size_t pos = _content.find('|');
        if (pos != std::string::npos) {
            _lineNumber = std::stoi(_content.substr(0, pos));
            tm time_tm = timeStr2Tm(_content.substr(pos + 1).c_str());
            time_tm.tm_isdst = 0;
            _key = mktime(&time_tm);
        }
        else {
            throw std::logic_error("FileLine failed to find | in " + _content);
        }
    }

    void fileIndex(int index) { _fileIndex = index; }
    
    std::string content() const { return _content; }
    int lineNumber() const { return _lineNumber; }
    time_t key() const { return _key; }
    int fileIndex() const { return _fileIndex; }

    std::string toString() const {
        std::stringstream ss;
        ss << _content << "#" << _lineNumber << "$" << _key;
        return ss.str();
    }

    void fromString(const std::string& line) {
        size_t pos = line.find('#');
        if (pos != std::string::npos) {
            _content = line.substr(0, pos);
            std::string meta_str = line.substr(pos + 1);
            pos = meta_str.find('$');
            if (pos != std::string::npos) {
                _lineNumber = std::stoi(meta_str.substr(0, pos));
                // tm time_tm = timeStr2Tm(meta_str.substr(pos + 1).c_str());
                // time_tm.tm_isdst = 0;
                // _key = mktime(&time_tm);
                _key = stoi(meta_str.substr(pos + 1));
            }
        }
    }
};

#endif