#pragma once
#include <vector>
#include <string>

struct statistics {
    size_t total_datapoints;
    int min;
    int max;
    int median;
};

/**
 * Returns statistics for each data line in the stream.
 *
 * Data format:
 * Each line is a set of records, consisting of numbers.
 * The first number is the expected size of this record, _n_.
 * The following _n_ numbers are in the range of [0, 255].
 *
 * A record where either a number is outside of the [0, 255] range is
 * invalid. Statistics for invalid lines are marked by data_points == -1.
 */
std::vector<statistics> summarize_data(std::istream& in);
