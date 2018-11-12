#include "tiny-02.hpp"

#include <set>
#include <regex>
#include <exception>
#include <tuple>
#include <sstream>

std::pair<size_t, int*> parse_line(std::string const& line) {
    std::stringstream sstr(line);
    size_t line_size;
    sstr >> line_size;
    int* data = new int[line_size];
    for (size_t i = 0; i < line_size; ++i) {
        sstr >> data[i];
    }
    return {line_size, data};
}

bool validate_data(int* data, size_t sz) {
    for (size_t i = 0; i < sz; ++i) {
        if (data[i] < 0 || data[i] > 255) {
            return false;
        }
    }
    return true;
}

// funcs:
// 1) Get line istream, returns allocated int[] (and size?) 
// 2) Verify line, takes array and a size
// 3) Summarize that might or might not drop the int[], deletes with `delete`

std::vector<statistics> summarize_data(std::istream& in) {
    std::vector<statistics> summary;
    
    std::string line;
    while (std::getline(in, line)) {
        size_t num_data;
        int* data;
        std::tie(num_data, data) = parse_line(line);
        if (validate_data(data, num_data)) {
            std::sort(data, data+num_data);
            auto median = (num_data % 2 == 1)? data[num_data/2] : (data[(num_data - 1) /2] + data[num_data/2]) / 2;
            summary.push_back({num_data, data[0], data[num_data-1], median});
        }
        delete [] data;
    }
    
    return summary;
}
